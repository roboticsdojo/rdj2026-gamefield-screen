import os
import time
import numpy as np
from PIL import Image

import rclpy
from rclpy.node import Node
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
        self.idle_timeout_sec = 10.0  # Auto-revert timeout
        self.idle_timer = None
        
        # 4. Service for explicit Request-Response state control
        # trigger=True -> Display custom image | trigger=False -> Revert to logo
        self.srv = self.create_service(
            SetBool, 
            '~/set_display', 
            self.handle_display_service
        )
        
        # 5. Topic subscriber for path-based string commands
        self.sub = self.create_subscription(
            String, 
            '~/display_command', 
            self.handle_topic_command, 
            10
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
            # Load and scale image to screen dimensions
            img = Image.open(img_path).convert('RGB')
            img = img.resize((self.width, self.height))
            np_img = np.array(img, dtype=np.uint8)

            # Convert RGB888 -> RGB565 bit-shifts
            r = (np_img[:, :, 0] >> 3).astype(np.uint16) << 11
            g = (np_img[:, :, 1] >> 2).astype(np.uint16) << 5
            b = (np_img[:, :, 2] >> 3).astype(np.uint16)
            
            raw_565 = (r | g | b).astype('<u2').tobytes()

            # Write buffer directly to framebuffer device
            with open(self.fb_path, 'wb') as fb:
                fb.write(raw_565)
            
            self.current_image_path = img_path
            return True
        except Exception as e:
            self.get_logger().error(f"Failed writing to framebuffer: {e}")
            return False

    def show_logo(self):
        """Resets state and renders default logo."""
        self.cancel_idle_timer()
        self._write_to_framebuffer(self.logo_path)
        self.get_logger().info("Display reverted to idle logo.")

    def render_custom_image(self, path):
        """Renders target image and schedules auto-revert timer."""
        success = self._write_to_framebuffer(path)
        if success:
            self.reset_idle_timer()
        return success

    def reset_idle_timer(self):
        """Resets the auto-revert timer safely."""
        self.cancel_idle_timer()
        self.idle_timer = self.create_timer(self.idle_timeout_sec, self._on_idle_timeout)

    def cancel_idle_timer(self):
        """Safely cancels active timer if present."""
        if self.idle_timer is not None:
            self.idle_timer.cancel()
            self.idle_timer = None

    def _on_idle_timeout(self):
        """Callback when idle timer expires."""
        self.get_logger().info("Idle timer expired.")
        self.show_logo()

    def handle_display_service(self, request, response):
        """Service callback: data=True displays sample image; data=False reverts to logo."""
        if request.data:
            success = self.render_custom_image(self.test_img_path)
            response.success = success
            response.message = "Loaded target image" if success else "Failed loading image"
        else:
            self.show_logo()
            response.success = True
            response.message = "Reverted to logo"
        return response

    def handle_topic_command(self, msg):
        """Topic callback: pass 'CLEAR' or 'LOGO' to revert, or absolute image path."""
        cmd = msg.data.strip()
        if cmd.upper() in ['CLEAR', 'LOGO', 'STOP']:
            self.show_logo()
        else:
            self.render_custom_image(cmd)


def main(args=None):
    rclpy.init(args=args)
    node = ScreenManagerNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()