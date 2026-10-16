# rdj2026-gamefield-screen

A gamefield display for Robotics Dojo: a screen driven by a Raspberry Pi that
a laptop can control over the network, plus the physical enclosure it lives in.

## What it does

- The Pi drives a physical screen (via its framebuffer) and shows a logo when
  idle, or a random image from a chosen category (`organic`, `other`,
  `plastic`) on command.
- A laptop hosts a small web page with buttons for those categories, plus a
  live view of what's currently on the Pi's screen.
- The two machines talk to each other over ROS 2, tunnelled through
  [Zenoh](https://zenoh.io/) over Tailscale, so it works whether they're on
  the same LAN or not.

## Architecture

```
 Browser
   |  loads page, clicks buttons
   v
 web_pi_bridge (laptop)  --ROS 2 service call, over Zenoh/Tailscale-->  screen_images (Pi)
   ^                                                                        |
   |  embeds <img> pointed at the Pi's stream                               v
   +-------------------------  MJPEG over HTTP  <----------------  /dev/fb0 (physical screen)
```

- **`web_pi_bridge`** — FastAPI web app on the laptop. Serves the control
  page and turns button clicks into ROS 2 service calls to the Pi.
- **`screen_images`** — runs on the Pi. Writes images to the framebuffer and
  streams what's on screen back out over plain HTTP (MJPEG) so the web page
  can show a live view.
- **`screen_interfaces`** — the ROS 2 service definition (`SetDisplayClass`)
  shared by the other two packages, so they agree on the request/response
  shape.

See each package's own README for nodes, endpoints, dependencies, and how to
build/run it:
- [`screen_interfaces/README.md`](./screen_interfaces/README.md)
- [`screen_images/README.md`](./screen_images/README.md)
- [`web_pi_bridge/README.md`](./web_pi_bridge/README.md)

## Repo layout

```
rdj2026-gamefield-screen/
├── screen_interfaces/   # shared ROS 2 srv definition
├── screen_images/       # Pi-side: framebuffer control + MJPEG stream
├── web_pi_bridge/       # laptop-side: web UI + ROS 2 bridge
├── cad_design/          # screen holder enclosure (STEP file)
└── jacque_cad_design/   # front/rear enclosure panels (STEP files + renders)
```

The three ROS 2 packages live together in one workspace `src/` on both
machines; each is built and run independently depending on which machine
it's on (`screen_images` on the Pi, `web_pi_bridge` on the laptop,
`screen_interfaces` on both).

# Launching
*** On the Pi ***
```
cd ~/ros2_ws
colcon build --packages-select screen_interfaces && source install/setup.bash
colcon build --packages-select screen_images --symlink-install && source install/setup.bash
ros2 run screen_images screen_manager_node

```

*** On the Pc ***
```
cd ~/ros2_ws
colcon build --packages-select screen_interfaces --cmake-args -DPython3_EXECUTABLE=/usr/bin/python3
colcon build --packages-select web_pi_bridge --symlink-install && source install/setup.bash
ros2 run web_pi_bridge fastapi_node
```