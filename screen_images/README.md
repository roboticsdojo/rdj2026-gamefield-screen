# screen_images

Runs **on the Pi**. Draws to the physical screen's framebuffer (`/dev/fb0`)
and streams what's currently on screen over HTTP so a laptop can view it live.

- **Build type:** `ament_python`
- **License:** MIT (see `LICENSE` in this package)

## Nodes

### `screen_manager_node`

`screen_images/screen_manager_node.py`

- On startup, loads the idle logo from `logos/icon.png` (installed into the
  package's share directory) and writes it to the framebuffer.
- Discovers image categories dynamically from `test_images/<category>/*` —
  currently `organic`, `other`, `plastic` — no code change needed to add more,
  just drop a new folder of images in.
- Scales images with `PIL.ImageOps.fit`, center-cropped to the framebuffer's
  aspect ratio, so nothing gets stretched.
- Reverts to the idle logo on a `CLEAR` / `LOGO` / `STOP` command.
- Wraps `rclpy.spin()` in a retry loop so a transient spin error doesn't kill
  the node.

**ROS interfaces:**

| Type    | Name                             | Interface                                  |
|---------|-----------------------------------|----------------------------------------------|
| Service | `~/set_display`                   | `example_interfaces/srv/SetBool` (legacy)   |
| Service | `~/set_display_class`             | `screen_interfaces/srv/SetDisplayClass`     |
| Topic   | `~/display_command` (subscriber)  | `std_msgs/msg/String`                       |

### `stream_node`

`screen_images/stream_node.py`

- Reads `/dev/fb0` directly (RGB565) and re-encodes each frame as JPEG — no
  ROS involved, it just reads the same framebuffer the manager node writes to.
- Serves MJPEG over plain `http.server`, threaded so multiple viewers don't
  block each other.
- **Port 8080.** `/` — simple `<img>` viewer page. `/stream` — the raw feed.
- ~20 FPS target, JPEG quality 75.
- Handles `SIGTERM`/`SIGINT` for a clean shutdown.

## Data

- `logos/` — idle-state images (`icon.png` is the one actually used; the
  others are alternate branding assets).
- `test_images/organic/`, `test_images/other/`, `test_images/plastic/` — the
  images `set_display_class` picks randomly from.

## Dependencies

```xml
<depend>rclpy</depend>
<depend>std_msgs</depend>
<depend>example_interfaces</depend>
<depend>screen_interfaces</depend>
<exec_depend>python3-numpy</exec_depend>
<exec_depend>python3-pil</exec_depend>
```

Console scripts:
```python
'stream_node = screen_images.stream_node:main'
'screen_manager_node = screen_images.screen_manager_node:main'
```

## Building

```bash
cd ~/ros2_ws
colcon build --packages-select screen_interfaces && source install/setup.bash
colcon build --packages-select screen_images --symlink-install && source install/setup.bash
```

## Running

```bash
ros2 run screen_images screen_manager_node
ros2 run screen_images stream_node        # separate terminal
```

View the stream from a laptop at `http://<pi-tailscale-ip>:8080/stream`.

Test the service:
```bash
ros2 service list
ros2 service call /screen_manager_node/set_display_class screen_interfaces/srv/SetDisplayClass "{category: 'organic'}"
```

## Deployment (Pi only — not tracked in this repo)

Both nodes run as systemd services (`screen_manager.service`,
`screen_stream.service`) on top of a locally-running Zenoh router
(`zenohd-router.service`), with `ROS_DOMAIN_ID=62` and
`RMW_IMPLEMENTATION=rmw_zenoh_cpp` set in the environment. The unit files
live in `/etc/systemd/system/` on the Pi itself. Restart after a code change:

```bash
sudo systemctl restart screen_manager.service
sudo systemctl status screen_manager.service
```