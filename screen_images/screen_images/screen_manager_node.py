import os
import numpy as np
import random
import time
from PIL import Image, ImageOps

import rclpy
from rclpy.node import Node
from rclpy.executors import ExternalShutdownException
from rclpy.qos import QoSProfile, ReliabilityPolicy, DurabilityPolicy
from ament_index_python.packages import get_package_share_directory
from example_interfaces.srv import SetBool
from std_msgs.msg import String
from screen_interfaces.srv import SetDisplayClass


class ScreenManagerNode(Node):
    # Colour of the bars around a picture that doesn't match the screen's shape.
    #   None          -> use the picture's own edge colour (bars blend into the picture)
    #   (r, g, b)     -> one fixed colour for every picture, e.g. (255, 255, 255) white or (0, 0, 0) black
    PAD_COLOR = None

    def __init__(self):
        super().__init__('screen_manager_node')

        self.fb_path = '/dev/fb0'
        self.width, self.height = self._get_fb_resolution()

        pkg_share = get_package_share_directory('screen_images')
        self.logo_path = os.path.join(pkg_share, 'logos', 'icon.png')

        # Discover categories dynamically: {'organic': [paths...], 'metal': [...], 'plastic': [...]}
        test_images_root = os.path.join(pkg_share, 'test_images')
        self.image_classes = {}
        if os.path.isdir(test_images_root):
            for entry in sorted(os.listdir(test_images_root)):
                category_path = os.path.join(test_images_root, entry)
                if os.path.isdir(category_path):
                    files = [
                        os.path.join(category_path, f)
                        for f in sorted(os.listdir(category_path))
                        if f.lower().endswith(('.png', '.jpg', '.jpeg'))
                    ]
                    if files:
                        self.image_classes[entry] = files

        self.get_logger().info(f"Discovered image classes: { {k: len(v) for k, v in self.image_classes.items()} }")

        self.current_image_path = self.logo_path
        self.idle_timer = None

        # Legacy on/off service (kept for backward compatibility with existing web bridge)
        self.srv = self.create_service(SetBool, '~/set_display', self.handle_display_service)

        # New: pick a random image within a named class
        self.class_srv = self.create_service(
            SetDisplayClass, '~/set_display_class', self.handle_display_class_service
        )

        qos_profile = QoSProfile(
            depth=10,
            reliability=ReliabilityPolicy.RELIABLE,
            durability=DurabilityPolicy.TRANSIENT_LOCAL
        )
        self.sub = self.create_subscription(
            String, '~/display_command', self.handle_topic_command, qos_profile
        )

        self.show_logo()
        self.get_logger().info('ScreenManagerNode initialized and logo displayed.')

    def _get_fb_resolution(self):
        return 1024, 600

    def _fit_to_screen(self, img):
        """Scale the image to fit inside the screen without cropping or stretching.

        Uses Lanczos resampling (sharpest for photos) and pads the leftover space with PAD_COLOR
        (default: the image's own edge colour), so a square picture keeps its whole object visible.
        """
        img = ImageOps.exif_transpose(img).convert('RGB')      # honour camera/phone rotation
        lanczos = getattr(Image, 'Resampling', Image).LANCZOS
        img = ImageOps.contain(img, (self.width, self.height), method=lanczos)
        a = np.asarray(img)
        border = np.concatenate([a[0], a[-1], a[:, 0], a[:, -1]])
        bg = self.PAD_COLOR or tuple(int(v) for v in np.median(border, axis=0))
        canvas = Image.new('RGB', (self.width, self.height), bg)
        canvas.paste(img, ((self.width - img.width) // 2, (self.height - img.height) // 2))
        return canvas

    @staticmethod
    def _to_rgb565(img):
        """RGB image -> little-endian RGB565 bytes for /dev/fb0 (rounded, not truncated, for better colour)."""
        n = np.asarray(img, dtype=np.uint16)
        r = ((n[:, :, 0] * 31 + 127) // 255) << 11
        g = ((n[:, :, 1] * 63 + 127) // 255) << 5
        b = (n[:, :, 2] * 31 + 127) // 255
        return (r | g | b).astype('<u2').tobytes()

    def _write_to_framebuffer(self, img_path):
        if not os.path.exists(img_path):
            self.get_logger().error(f"Image path does not exist: {img_path}")
            return False
        try:
            img = self._fit_to_screen(Image.open(img_path))
            raw_565 = self._to_rgb565(img)

            with open(self.fb_path, 'wb') as fb:
                fb.write(raw_565)

            self.current_image_path = img_path
            return True
        except Exception as e:
            self.get_logger().error(f"Failed writing to framebuffer: {e}")
            return False

    def cancel_idle_timer(self):
        if self.idle_timer is not None:
            self.idle_timer.cancel()
            self.destroy_timer(self.idle_timer)
            self.idle_timer = None

    def show_logo(self):
        self.cancel_idle_timer()
        self._write_to_framebuffer(self.logo_path)
        self.get_logger().info("Display reverted to idle logo.")

    def render_custom_image(self, path):
        self.cancel_idle_timer()
        success = self._write_to_framebuffer(path)
        if success:
            self.get_logger().info(f"Displaying image indefinitely: {path}")
        return success

    def render_random_from_class(self, category):
        files = self.image_classes.get(category)
        if not files:
            return False, f"Unknown or empty category: '{category}'. Available: {list(self.image_classes.keys())}"
        chosen = random.choice(files)
        success = self.render_custom_image(chosen)
        if success:
            return True, f"Displaying '{os.path.basename(chosen)}' from class '{category}'"
        return False, f"Failed to render '{chosen}'"

    def handle_display_service(self, request, response):
        """Legacy: data=True shows a random 'organic' image (adjust default as you like), False reverts to logo."""
        if request.data:
            success, message = self.render_random_from_class(next(iter(self.image_classes), None))
            response.success = success
            response.message = message
        else:
            self.show_logo()
            response.success = True
            response.message = "Reverted to logo"
        return response

    def handle_display_class_service(self, request, response):
        success, message = self.render_random_from_class(request.category)
        response.success = success
        response.message = message
        return response

    def handle_topic_command(self, msg):
        cmd = msg.data.strip()
        if cmd.upper() in ['CLEAR', 'LOGO', 'STOP']:
            self.show_logo()
        elif cmd in self.image_classes:
            self.render_random_from_class(cmd)
        else:
            self.render_custom_image(cmd)


def main(args=None):
    rclpy.init(args=args)
    node = ScreenManagerNode()
    try:
        while rclpy.ok():
            try:
                rclpy.spin(node)
            except (KeyboardInterrupt, ExternalShutdownException):
                break
            except Exception as e:
                node.get_logger().error(f"spin() raised, retrying: {e}")
                time.sleep(1.0)
    finally:
        node.destroy_node()
        try:
            if rclpy.ok():
                rclpy.shutdown()
        except Exception:
            pass


if __name__ == '__main__':
    main()