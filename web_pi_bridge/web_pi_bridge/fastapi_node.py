import os
import asyncio
import threading
from pathlib import Path
from contextlib import asynccontextmanager
from fastapi import FastAPI
from fastapi.responses import FileResponse
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel
from ament_index_python.packages import get_package_share_directory

import rclpy
from rclpy.node import Node
from example_interfaces.srv import SetBool
from screen_interfaces.srv import SetDisplayClass


class WebBridgeNode(Node):
    def __init__(self):
        super().__init__('web_pi_bridge_node')
        self.cli = self.create_client(SetBool, '/screen_manager_node/set_display')
        self.class_cli = self.create_client(SetDisplayClass, '/screen_manager_node/set_display_class')
        self.get_logger().info('WebBridgeNode initialized, waiting for screen manager service...')

    async def _call(self, client, request, timeout_sec=5.0):
        if not client.wait_for_service(timeout_sec=2.0):
            return False, "Screen manager service unavailable"

        future = client.call_async(request)
        elapsed = 0.0
        while not future.done() and elapsed < timeout_sec:
            await asyncio.sleep(0.05)
            elapsed += 0.05

        if future.done():
            try:
                response = future.result()
                return response.success, response.message
            except Exception as e:
                return False, f"Service execution failed: {e}"

        return False, "Service request timed out"

    async def call_set_display(self, state: bool):
        req = SetBool.Request()
        req.data = state
        return await self._call(self.cli, req)

    async def call_set_display_class(self, category: str):
        req = SetDisplayClass.Request()
        req.category = category
        return await self._call(self.class_cli, req)


ros_node = None

@asynccontextmanager
async def lifespan(app: FastAPI):
    global ros_node
    if not rclpy.ok():
        rclpy.init()
    ros_node = WebBridgeNode()

    thread = threading.Thread(target=lambda: rclpy.spin(ros_node), daemon=True)
    thread.start()

    yield

    ros_node.destroy_node()
    if rclpy.ok():
        rclpy.shutdown()


app = FastAPI(lifespan=lifespan)

try:
    pkg_share = get_package_share_directory('web_pi_bridge')
    TEMPLATE_PATH = os.path.join(pkg_share, 'templates', 'index.html')
    STATIC_DIR = os.path.join(pkg_share, 'static')
    if not os.path.exists(TEMPLATE_PATH):
        raise FileNotFoundError
except Exception:
    PKG_ROOT = Path(__file__).parent.resolve()
    TEMPLATE_PATH = os.path.join(PKG_ROOT, 'templates', 'index.html')
    STATIC_DIR = os.path.join(PKG_ROOT, 'static')

# Serve CSS/JS assets from /static
app.mount("/static", StaticFiles(directory=STATIC_DIR), name="static")


class DisplayRequest(BaseModel):
    state: bool


class DisplayClassRequest(BaseModel):
    category: str


@app.get("/", response_class=FileResponse)
async def get_index():
    if not os.path.exists(TEMPLATE_PATH):
        return FileResponse("index.html")
    return FileResponse(TEMPLATE_PATH)


@app.post("/api/set_display")
async def handle_set_display(req: DisplayRequest):
    success, message = await ros_node.call_set_display(req.state)
    return {"success": success, "message": message}


@app.post("/api/set_display_class")
async def handle_set_display_class(req: DisplayClassRequest):
    success, message = await ros_node.call_set_display_class(req.category)
    return {"success": success, "message": message}


def main():
    import uvicorn
    uvicorn.run("web_pi_bridge.fastapi_node:app", host="0.0.0.0", port=8000, reload=False)


if __name__ == '__main__':
    main()