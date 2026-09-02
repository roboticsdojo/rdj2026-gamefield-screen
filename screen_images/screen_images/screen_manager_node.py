import os
import numpy as np
import time
from PIL import Image

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy, DurabilityPolicy
from ament_index_python.packages import get_package_share_directory
from example_interfaces.srv import SetBool
from std_msgs.msg import String


class ScreenManagerNode(Node):
    def __init__(self):
        super().__init__('screen_manager_node')
        
        # 1. Fetch FB0 screen resolution
        self.fb_path = '/dev/fb0'
        self.width, self.height = self._get_fb_resolution()
        
        # 2. Package share directory for logo & test images
        pkg_share = get_package_share_directory('screen_images')
        self.logo_path = os.path.join(pkg_share, 'logos', 'icon.png')
        self.test_img_path = os.path.join(pkg_share, 'test_images', 'banana.png')
        
        # 3. State & Timer variables
        self.current_image_path = self.logo_path
        self.idle_timer = None
        
        # 4. Service for explicit Request-Response state control
        self.srv = self.create_service(
            SetBool, 
            '~/set_display', 
            self.handle_display_service
        )
        
        # 5. Topic subscriber with Transient Local QoS (latching)
        qos_profile = QoSProfile(
            depth=10,
            reliability=ReliabilityPolicy.RELIABLE,
            durability=DurabilityPolicy.TRANSIENT_LOCAL
        )
        self.sub = self.create_subscription(
            String, 
            '~/display_command', 
            self.handle_topic_command, 
            qos_profile
        )
        
        # 6. Display logo on startup
        self.show_logo()
        self.get_logger().info('ScreenManagerNode initialized and logo displayed.')

    def _get_fb_resolution(self):
        """Reads resolution directly from sysfs driver."""
        try:
            with open('/sys/class/graphics/fb0/virtual_size', 'r') as f:
                w, h = map(int, f.read().strip().split(','))
                return w, h
        except Exception as e:
            self.get_logger().warn(f"Failed to read virtual_size: {e}. Defaulting to 480x320.")
            return 480, 320

    def _write_to_framebuffer(self, img_path):
        """Loads image, resizes to display, converts RGB888 -> RGB565, and writes to /dev/fb0."""
        if not os.path.exists(img_path):
            self.get_logger().error(f"Image path does not exist: {img_path}")
            return False

        try:
            img = Image.open(img_path).convert('RGB')
            img = img.resize((self.width, self.height))
            np_img = np.array(img, dtype=np.uint8)

            r = (np_img[:, :, 0] >> 3).astype(np.uint16) << 11
            g = (np_img[:, :, 1] >> 2).astype(np.uint16) << 5
            b = (np_img[:, :, 2] >> 3).astype(np.uint16)
            
            raw_565 = (r | g | b).astype('<u2').tobytes()

            with open(self.fb_path, 'wb') as fb:
                fb.write(raw_565)
            
            self.current_image_path = img_path
            return True
        except Exception as e:
            self.get_logger().error(f"Failed writing to framebuffer: {e}")
            return False

    def cancel_idle_timer(self):
        """Completely destroys active timers to prevent auto-reversion."""
        if self.idle_timer is not None:
            self.idle_timer.cancel()
            self.destroy_timer(self.idle_timer)
            self.idle_timer = None

    def show_logo(self):
        """Resets state and renders default logo indefinitely."""
        self.cancel_idle_timer()
        self._write_to_framebuffer(self.logo_path)
        self.get_logger().info("Display reverted to idle logo.")

    def render_custom_image(self, path):
        """Renders target image and holds it indefinitely until explicitly stopped."""
        self.cancel_idle_timer()
        success = self._write_to_framebuffer(path)
        if success:
            self.get_logger().info(f"Displaying image indefinitely: {path}")
        return success

    def handle_display_service(self, request, response):
        """Service callback: data=True displays sample image indefinitely; data=False reverts to logo."""
        if request.data:
            success = self.render_custom_image(self.test_img_path)
            response.success = success
            response.message = "Loaded target image indefinitely" if success else "Failed loading image"
        else:
            self.show_logo()
            response.success = True
            response.message = "Reverted to logo"
        return response

    def handle_topic_command(self, msg):
        """Topic callback: pass 'CLEAR', 'LOGO', or 'STOP' to revert, or an absolute image path."""
        cmd = msg.data.strip()
        if cmd.upper() in ['CLEAR', 'LOGO', 'STOP']:
            self.show_logo()
        else:
            self.render_custom_image(cmd)


def main(args=None):
    rclpy.init(args=args)
    node = ScreenManagerNode()
    try:
        while rclpy.ok():
            try:
                rclpy.spin(node)
            except KeyboardInterrupt:
                break
            except Exception as e:
                node.get_logger().error(f"spin() raised, retrying: {e}")
                time.sleep(1.0)
    finally:
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()



if __name__ == '__main__':
    main()