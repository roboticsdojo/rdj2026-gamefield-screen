#This node streams screen output to laptop
import http.server
import socketserver
import time
import numpy as np
from PIL import Image

PORT = 8080

# Load resolution parameters directly from sysfs
with open('/sys/class/graphics/fb0/virtual_size', 'r') as f:
    WIDTH, HEIGHT = map(int, f.read().strip().split(','))

class StreamHandler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/stream':
            self.send_response(200)
            self.send_header('Content-type', 'multipart/x-mixed-replace; boundary=frame')
            self.end_headers()
            
            bytes_to_read = WIDTH * HEIGHT * 2  # 16-bit color (RGB565 = 2 bytes/pixel)
            
            while True:
                try:
                    with open('/dev/fb0', 'rb') as f:
                        raw = f.read(bytes_to_read)
                    
                    if len(raw) < bytes_to_read:
                        time.sleep(0.05)
                        continue
                    
                    # Unpack RGB565 buffer via NumPy
                    raw_16 = np.frombuffer(raw, dtype=np.uint16).reshape((HEIGHT, WIDTH))
                    
                    r = ((raw_16 >> 11) & 0x1F) << 3
                    g = ((raw_16 >> 5) & 0x3F) << 2
                    b = (raw_16 & 0x1F) << 3
                    
                    rgb = np.dstack((r, g, b)).astype(np.uint8)
                    
                    # Encode to JPEG
                    img = Image.fromarray(rgb)
                    import io
                    buf = io.BytesIO()
                    img.save(buf, format='JPEG', quality=75)
                    frame = buf.getvalue()

                    self.wfile.write(b'--frame\r\n')
                    self.send_header('Content-Type', 'image/jpeg')
                    self.send_header('Content-Length', str(len(frame)))
                    self.end_headers()
                    self.wfile.write(frame)
                    self.wfile.write(b'\r\n')
                    time.sleep(0.05)  # Cap at ~20 FPS
                except Exception:
                    break
def main():
    with socketserver.TCPServer(("", PORT), StreamHandler) as httpd:
        print(f"Streaming physical screen ({WIDTH}x{HEIGHT} RGB565) on port {PORT}")
        httpd.serve_forever()