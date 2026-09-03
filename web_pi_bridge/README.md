# web_pi_bridge

Runs **on the laptop**. Serves the "Robotics Dojo Screen Control" web page
and bridges HTTP requests from the browser into ROS 2 service calls on the Pi.

- **Build type:** `ament_python`
- **License:** MIT (see `LICENSE` in this package)

## Node

### `fastapi_node`

`web_pi_bridge/fastapi_node.py`

- Wraps a ROS 2 node (`web_pi_bridge_node`) inside the FastAPI app's lifespan,
  so `rclpy.spin()` runs on a background thread alongside the web server.
- Holds two ROS 2 service clients pointed at the Pi's `screen_manager_node`:
  - `/screen_manager_node/set_display` (`example_interfaces/srv/SetBool`, legacy)
  - `/screen_manager_node/set_display_class` (`screen_interfaces/srv/SetDisplayClass`)
- Serves `templates/index.html` and mounts `static/` (CSS/JS) at `/static`,
  resolving paths from the installed package share directory first and
  falling back to the source tree if that's missing.
- Runs via `uvicorn` on **port 8000**, bound to `0.0.0.0`.

**HTTP endpoints:**

| Method | Path                     | Body                       | Does                                           |
|--------|--------------------------|------------------------------|--------------------------------------------------|
| GET    | `/`                      | —                             | Serves the control page                          |
| POST   | `/api/set_display`       | `{ "state": bool }`           | Legacy on/off display toggle                     |
| POST   | `/api/set_display_class` | `{ "category": string }`      | Shows a random image from that class on the Pi   |

## Frontend

`templates/index.html` + `static/style.css` + `static/script.js`:

- Embeds the Pi's live stream directly: `<img src="http://<pi-ip>:8080/stream">`.
  **This IP is hardcoded in `index.html`** — update it there if the Pi's
  Tailscale address changes.
- Buttons for the three image categories (`organic`, `other`, `plastic`) →
  `/api/set_display_class`.
- "Display Sample" / "Revert to Logo" buttons → legacy `/api/set_display`.
- Status line shows the JSON `message` from whichever endpoint was called.

## Dependencies

```xml
<depend>rclpy</depend>
<depend>std_msgs</depend>
<depend>screen_interfaces</depend>
```
Python runtime: `fastapi`, `uvicorn`.

Console script: `'fastapi_node = web_pi_bridge.fastapi_node:main'`

`setup.py` copies `templates/` and `static/` into the installed share
directory so the node doesn't depend on the source tree at runtime.

## Building

```bash
cd ~/ros2_ws
colcon build --packages-select screen_interfaces && source install/setup.bash
colcon build --packages-select web_pi_bridge --symlink-install && source install/setup.bash
```

## Running

```bash
ros2 run web_pi_bridge fastapi_node
```

Then open `http://<laptop-ip>:8000/`. Requires the Pi's `screen_manager`
service (and its Zenoh router) already up, and `ROS_DOMAIN_ID=62` /
`RMW_IMPLEMENTATION=rmw_zenoh_cpp` set in the environment — see the top-level
repo README for the shared network setup.

## Deployment

No systemd service for this one yet — it's started manually with `ros2 run`
when needed.