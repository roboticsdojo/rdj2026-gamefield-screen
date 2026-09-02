import os
import asyncio
import threading
from pathlib import Path
from contextlib import asynccontextmanager
from fastapi import FastAPI
from fastapi.responses import FileResponse
from pydantic import BaseModel
from ament_index_python.packages import get_package_share_directory

import rclpy
from rclpy.node import Node
from example_interfaces.srv import SetBool


class WebBridgeNode(Node):
    def __init__(self):
        super().__init__('web_pi_bridge_node')
        # Service client matching screen_manager_node on Pi
        self.cli = self.create_client(SetBool, '/screen_manager_node/set_display')
        self.get_logger().info('WebBridgeNode initialized, waiting for screen manager service...')
    async def call_set_display(self, state: bool):
        if not self.cli.wait_for_service(timeout_sec=2.0):
            return False, "Screen manager service unavailable"
        
        req = SetBool.Request()
        req.data = state
        future = self.cli.call_async(req)
        
        # Non-blocking async loop to poll rclpy.task.Future safely
        timeout_sec = 5.0
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

# Robust template path resolution (checks share first, then source package tree)
try:
    pkg_share = get_package_share_directory('web_pi_bridge')
    TEMPLATE_PATH = os.path.join(pkg_share, 'templates', 'index.html')
    if not os.path.exists(TEMPLATE_PATH):
        raise FileNotFoundError
except Exception:
    # Fallback directly to the file relative to this script
    TEMPLATE_PATH = os.path.join(Path(__file__).parent.resolve(), 'templates', 'index.html')


class DisplayRequest(BaseModel):
    state: bool


@app.get("/", response_class=FileResponse)
async def get_index():
    if not os.path.exists(TEMPLATE_PATH):
        return FileResponse("index.html")  # Local directory fallback
    return FileResponse(TEMPLATE_PATH)


@app.post("/api/set_display")
async def handle_set_display(req: DisplayRequest):
    success, message = await ros_node.call_set_display(req.state)
    return {"success": success, "message": message}


def main():
    import uvicorn
    uvicorn.run("web_pi_bridge.fastapi_node:app", host="0.0.0.0", port=8000, reload=False)


if __name__ == '__main__':
    main()