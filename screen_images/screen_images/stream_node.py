#!/usr/bin/env python3
"""Streams the Pi's framebuffer (/dev/fb0) to laptops as MJPEG over HTTP."""
import http.server
import socketserver
import threading
import signal
import sys
import time
import io
import logging
import numpy as np
from PIL import Image

PORT = 8080
FB_PATH = '/dev/fb0'
TARGET_FPS = 20
FRAME_INTERVAL = 1.0 / TARGET_FPS
JPEG_QUALITY = 75

logging.basicConfig(
    level=logging.INFO,
    format='[%(asctime)s] [%(levelname)s] %(message)s',
    datefmt='%H:%M:%S',
)
log = logging.getLogger('stream_node')


def read_fb_resolution():
    with open('/sys/class/graphics/fb0/virtual_size', 'r') as f:
        w, h = map(int, f.read().strip().split(','))
        return w, h


WIDTH, HEIGHT = read_fb_resolution()
BYTES_PER_FRAME = WIDTH * HEIGHT * 2  # RGB565 = 2 bytes/pixel

INDEX_PAGE = f"""<!DOCTYPE html>
<html><head><title>Pi Screen Stream</title></head>
<body style="margin:0;background:#111;">
<img src="/stream" style="display:block;margin:0 auto;max-width:100%;">
</body></html>"""


class StreamHandler(http.server.BaseHTTPRequestHandler):
    # Quiet the default per-request stdout logging; we use our own logger.
    def log_message(self, fmt, *args):
        pass

    def do_GET(self):
        if self.path == '/stream':
            self._handle_stream()
        elif self.path in ('/', '/index.html'):
            self._handle_index()
        else:
            self.send_error(404, "Not found")

    def _handle_index(self):
        body = INDEX_PAGE.encode('utf-8')
        self.send_response(200)
        self.send_header('Content-type', 'text/html; charset=utf-8')
        self.send_header('Content-Length', str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def _handle_stream(self):
        client = self.client_address[0]
        log.info(f"Client connected: {client}")

        try:
            self.send_response(200)
            self.send_header('Age', '0')
            self.send_header('Cache-Control', 'no-cache, private')
            self.send_header('Pragma', 'no-cache')
            self.send_header('Content-type', 'multipart/x-mixed-replace; boundary=frame')
            self.end_headers()
        except (BrokenPipeError, ConnectionResetError):
            log.info(f"Client {client} disconnected before headers sent")
            return

        consecutive_errors = 0
        max_consecutive_errors = 20  # ~1s of bad reads at 20fps before giving up on fb0 itself

        while not server_shutdown_event.is_set():
            loop_start = time.monotonic()
            try:
                frame = self._grab_frame()
                if frame is None:
                    time.sleep(0.05)
                    continue

                self.wfile.write(b'--frame\r\n')
                self.send_header('Content-Type', 'image/jpeg')
                self.send_header('Content-Length', str(len(frame)))
                self.end_headers()
                self.wfile.write(frame)
                self.wfile.write(b'\r\n')
                consecutive_errors = 0

            except (BrokenPipeError, ConnectionResetError, ConnectionAbortedError):
                # Normal: client closed the tab / lost network. Not an error worth logging loudly.
                log.info(f"Client disconnected: {client}")
                return
            except Exception as e:
                consecutive_errors += 1
                log.warning(f"Frame error ({consecutive_errors}/{max_consecutive_errors}): {e}")
                if consecutive_errors >= max_consecutive_errors:
                    log.error("Too many consecutive frame errors, dropping this client")
                    return
                time.sleep(0.1)
                continue

            elapsed = time.monotonic() - loop_start
            sleep_for = FRAME_INTERVAL - elapsed
            if sleep_for > 0:
                time.sleep(sleep_for)

        log.info(f"Shutdown requested, closing stream to {client}")

    def _grab_frame(self):
        """Reads one frame from /dev/fb0 and returns JPEG bytes, or None if unavailable."""
        try:
            with open(FB_PATH, 'rb') as f:
                raw = f.read(BYTES_TO_READ)
        except (FileNotFoundError, OSError) as e:
            raise RuntimeError(f"Cannot read {FB_PATH}: {e}")

        if len(raw) < BYTES_TO_READ:
            return None

        raw_16 = np.frombuffer(raw, dtype=np.uint16).reshape((HEIGHT, WIDTH))
        r = ((raw_16 >> 11) & 0x1F) << 3
        g = ((raw_16 >> 5) & 0x3F) << 2
        b = (raw_16 & 0x1F) << 3
        rgb = np.dstack((r, g, b)).astype(np.uint8)

        img = Image.fromarray(rgb)
        buf = io.BytesIO()
        img.save(buf, format='JPEG', quality=JPEG_QUALITY)
        return buf.getvalue()


BYTES_TO_READ = BYTES_PER_FRAME
server_shutdown_event = threading.Event()


class ThreadingHTTPServer(socketserver.ThreadingMixIn, http.server.HTTPServer):
    """Handles each client on its own thread so multiple laptops can view at once,
    and one slow/dead client doesn't block the others."""
    daemon_threads = True
    allow_reuse_address = True  # avoids "Address already in use" on quick restarts


def main():
    httpd = ThreadingHTTPServer(("", PORT), StreamHandler)

    def handle_shutdown(signum, frame):
        log.info(f"Received signal {signum}, shutting down cleanly...")
        server_shutdown_event.set()
        threading.Thread(target=httpd.shutdown, daemon=True).start()

    signal.signal(signal.SIGTERM, handle_shutdown)
    signal.signal(signal.SIGINT, handle_shutdown)

    log.info(f"Streaming physical screen ({WIDTH}x{HEIGHT} RGB565) on port {PORT}")
    try:
        httpd.serve_forever()
    finally:
        httpd.server_close()
        log.info("Server closed.")


if __name__ == '__main__':
    main()