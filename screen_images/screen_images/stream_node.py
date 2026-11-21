#!/usr/bin/env python3
"""Streams the Pi's framebuffer (/dev/fb0) to laptops as MJPEG over HTTP.

One capture thread grabs and JPEG-encodes the framebuffer once per frame, and every
viewer is handed the newest frame. The capture thread only runs while somebody is
watching, so extra tabs cost nothing and an idle Pi stays idle.
"""
import http.server
import io
import logging
import os
import signal
import socket
import socketserver
import threading
import time
import zlib

import numpy as np
from PIL import Image

PORT = 8080
FB_PATH = os.environ.get('STREAM_FB', '/dev/fb0')
TARGET_FPS = 20
FRAME_INTERVAL = 1.0 / TARGET_FPS
JPEG_QUALITY = 75
KEEPALIVE_S = 1.0       # resend an unchanged screen this often so viewers can tell the stream is alive
CLIENT_TIMEOUT_S = 5.0  # a viewer that cannot take a frame for this long is dropped

logging.basicConfig(
    level=logging.INFO,
    format='[%(asctime)s] [%(levelname)s] %(message)s',
    datefmt='%H:%M:%S',
)
log = logging.getLogger('stream_node')


def read_fb_resolution():
    size = os.environ.get('STREAM_SIZE')  # "w,h" override, for testing without a framebuffer
    if size:
        w, h = map(int, size.split(','))
        return w, h
    with open('/sys/class/graphics/fb0/virtual_size', 'r') as f:
        w, h = map(int, f.read().strip().split(','))
        return w, h


WIDTH, HEIGHT = read_fb_resolution()
BYTES_PER_FRAME = WIDTH * HEIGHT * 2  # RGB565 = 2 bytes/pixel

INDEX_PAGE = """<!DOCTYPE html>
<html><head><title>Pi Screen Stream</title></head>
<body style="margin:0;background:#111;">
<img src="/stream" style="display:block;margin:0 auto;max-width:100%;">
</body></html>"""

server_shutdown_event = threading.Event()


def read_framebuffer():
    """Raw RGB565 bytes of the whole screen, or None if the framebuffer gave a short read."""
    with open(FB_PATH, 'rb') as f:
        raw = f.read(BYTES_PER_FRAME)
    return raw if len(raw) == BYTES_PER_FRAME else None


def encode_jpeg(raw):
    px = np.frombuffer(raw, dtype=np.uint16).reshape((HEIGHT, WIDTH))
    rgb = np.empty((HEIGHT, WIDTH, 3), dtype=np.uint8)
    rgb[..., 0] = (px >> 8) & 0xF8
    rgb[..., 1] = (px >> 3) & 0xFC
    rgb[..., 2] = (px << 3) & 0xF8
    buf = io.BytesIO()
    Image.fromarray(rgb).save(buf, format='JPEG', quality=JPEG_QUALITY)
    return buf.getvalue()


class FrameSource:
    """Newest JPEG frame, shared by all viewers."""

    def __init__(self):
        self.cond = threading.Condition()
        self.frame = None
        self.seq = 0
        self.viewers = 0
        self.thread = None

    def acquire(self):
        with self.cond:
            self.viewers += 1
            if self.thread is None:
                self.thread = threading.Thread(target=self._run, daemon=True)
                self.thread.start()

    def release(self):
        with self.cond:
            self.viewers -= 1

    def _run(self):
        last_crc = None
        last_emit = 0.0
        while True:
            with self.cond:
                if self.viewers <= 0 or server_shutdown_event.is_set():
                    self.thread = None
                    return
            started = time.monotonic()
            try:
                raw = read_framebuffer()
                if raw is not None:
                    crc = zlib.crc32(raw)
                    if crc != last_crc or started - last_emit >= KEEPALIVE_S:
                        jpeg = encode_jpeg(raw)
                        with self.cond:
                            self.frame = jpeg
                            self.seq += 1
                            self.cond.notify_all()
                        last_crc, last_emit = crc, started
            except Exception as e:
                log.warning(f"Frame error: {e}")
                time.sleep(0.2)
            time.sleep(max(0.0, FRAME_INTERVAL - (time.monotonic() - started)))


source = FrameSource()


class StreamHandler(http.server.BaseHTTPRequestHandler):
    def log_message(self, fmt, *args):
        pass

    def do_GET(self):
        path = self.path.split('?', 1)[0]
        if path == '/stream':
            self._handle_stream()
        elif path in ('/', '/index.html'):
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
        self.connection.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        self.connection.settimeout(CLIENT_TIMEOUT_S)   # a stuck viewer errors out instead of hanging a thread forever

        source.acquire()
        try:
            self.send_response(200)
            self.send_header('Age', '0')
            self.send_header('Cache-Control', 'no-cache, private')
            self.send_header('Pragma', 'no-cache')
            self.send_header('Content-type', 'multipart/x-mixed-replace; boundary=frame')
            self.end_headers()

            last = 0
            while not server_shutdown_event.is_set():
                with source.cond:
                    if not source.cond.wait_for(lambda: source.seq != last, timeout=CLIENT_TIMEOUT_S):
                        continue
                    frame, last = source.frame, source.seq
                self.wfile.write(
                    b'--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %d\r\n\r\n' % len(frame)
                    + frame + b'\r\n')
        except OSError:
            pass    # viewer closed the tab, lost the network, or stopped reading (timeout)
        finally:
            source.release()
            log.info(f"Client disconnected: {client}")


class ThreadingHTTPServer(socketserver.ThreadingMixIn, http.server.HTTPServer):
    """One thread per viewer, so a slow or dead viewer never blocks the others."""
    daemon_threads = True
    allow_reuse_address = True


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