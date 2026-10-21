# rdj2026 gamefield control

Code that runs the Robotics Dojo gamefield hardware (screen and loading station / dispenser)
and the judges' web page that controls it. The page also holds an obstacle-layout randomiser
and a **map request** tool that lets judges ask a competing robot for its map.

> **Housekeeping:** `web_pi_bridge` now serves four things (screen, dispenser, obstacles, maps),
> so it no longer belongs only to the screen repo. Consider moving it to its own repo
> (e.g. `gamefield-control`). Nothing in the code depends on where it lives.

## What runs where

| Machine | Runs | Package / file | Repo |
|---|---|---|---|
| **Judges' laptop** (WSL) | Web page and bridge, port 8000 | `web_pi_bridge` (`fastapi_node`) | screen repo |
| **Pi (screen)** | Screen drawing, live stream (port 8080) | `screen_images`, `screen_interfaces` | screen repo |
| **Pi (dispenser)** | Servo control over serial to the Arduino | `loading_station`, `loading_station_interfaces` | dispenser repo |
| **Competitor robot** | Sends its map to the judges | `dojo_map.py` | copied from the web page |

The dispenser Pi can be the same Pi as the screen Pi.

The two repos are **separate colcon workspaces and do not need to be merged**. The laptop only
needs the dispenser's message/action definitions (`loading_station_interfaces`), obtained by
building that one package and sourcing both workspaces in the same terminal. If it is missing,
the screen page still works and the dispenser panel says so.

```
rdj2026-gamefield-screen/                  <- workspace root (build/install/log live here)
  screen_images/  screen_interfaces/  web_pi_bridge/
rdj2026-gamefield-dispenser/
  loading_zone_ws/src/
    loading_station/  loading_station_interfaces/
```

## The web page

Open `http://localhost:8000` (or `http://<laptop-ip>:8000`). The dropdown in the header switches panel;
the choice is kept in the URL (`/#dispenser`, `/#obstacles`, `/#maps`).

| Panel | What it does | Needs the Pi? |
|---|---|---|
| Gamefield Screen | Live stream and buttons for the image categories | Yes |
| Gamefield Dispenser | State, pieces left per station, dispense / home / reset / enable / cancel | Yes |
| Obstacle Layout | Randomises 3 obstacles on a 3 x 2 grid | No |
| Map Request | Request a map from a team and view what comes back | No (needs a team's script) |

Details and the HTTP API are in `web_pi_bridge/README.md`.

## Setup

### 1. Environment script

Save [`tools/dojo_env.sh`](tools/dojo_env.sh) as `~/dojo_env.sh` and set the two paths at the top
(`SCREEN_REPO`, `DISP_WS`). It sources ROS and both workspaces and selects the network mode:

```bash
source ~/dojo_env.sh on     # real Pi: rmw_zenoh_cpp, ROS_DOMAIN_ID=62
source ~/dojo_env.sh off    # Pi off: plain DDS, isolated ROS_DOMAIN_ID=99
```

For `on`, also set whatever you already use to reach the Pi's Zenoh router.
Do this in every new terminal.

### 2. Build the laptop (WSL)

Prerequisites: ROS 2 Jazzy, plus `fastapi` and `uvicorn` for Python (already needed by the screen page).

```bash
source ~/dojo_env.sh on                     # sets SCREEN_REPO / DISP_WS (build output doesn't exist yet, that's fine)
cd $DISP_WS && colcon build --packages-select loading_station_interfaces
source install/setup.bash
cd $SCREEN_REPO && colcon build --packages-select screen_interfaces web_pi_bridge --symlink-install
source ~/dojo_env.sh on                     # re-source so both workspaces are picked up
```

Rebuild `web_pi_bridge` after adding or removing files in its `static/` or `templates/` folders
(they are installed from a glob at build time).

### 3. Build the Pi

The screen packages are already built if the screen services run. For the dispenser:

```bash
cd ~/path/to/rdj2026-gamefield-dispenser/loading_zone_ws
colcon build --symlink-install && source install/setup.bash

# one-time: stable device name for the Arduino
sudo cp udev/99-loading-station.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules && sudo udevadm trigger     # then replug the Arduino
ls -l /dev/loading_station
```

Make sure your user is in the `dialout` group (`groups`; add with `sudo usermod -aG dialout $USER`, then log in again).

## Running with the Pi ON

**On the Pi** (screen nodes are systemd services; the dispenser is started by hand until you add a service, see below):

```bash
sudo systemctl status zenohd-router screen_manager screen_stream

export ROS_DOMAIN_ID=62 RMW_IMPLEMENTATION=rmw_zenoh_cpp
source ~/path/to/loading_zone_ws/install/setup.bash
ros2 launch loading_station loading_station.launch.py
# if the udev link is missing:  ... serial_port:=/dev/ttyUSB0
```

**On the laptop:**

```bash
source ~/dojo_env.sh on
ros2 run web_pi_bridge fastapi_node         # then open http://localhost:8000
```

**Test from a second laptop terminal** (`source ~/dojo_env.sh on` first):

```bash
ros2 service list | grep -E "screen_manager|loading_station"
ros2 topic echo /gamefield/loading_station/status --once
ros2 service call /screen_manager_node/set_display_class screen_interfaces/srv/SetDisplayClass "{category: 'organic'}"

# through the web bridge (same path the buttons use)
curl localhost:8000/api/dispenser/status
curl -X POST -H 'Content-Type: application/json' -d '{"category":"organic"}' localhost:8000/api/set_display_class
curl -X POST localhost:8000/api/dispenser/home                  # moves servos
curl -X POST -H 'Content-Type: application/json' -d '{"station_id":1,"count":1}' localhost:8000/api/dispenser/dispense
```

**Expected:** the dispenser badge reads IDLE and shows the pieces left per station, and the
screen stream shows the Pi's display. **The dispenser commands move real servos, so keep the mechanism clear first.**

## Running with the Pi OFF

Obstacle Layout and Map Request work with no Pi. For the screen and dispenser panels,
[`tools/fake_pi.py`](tools/fake_pi.py) pretends to be both Pi nodes (same ROS names; nothing moves,
nothing is drawn). Run it **only when the real Pi is off**.

```bash
# terminal 1
source ~/dojo_env.sh off && python3 tools/fake_pi.py
# terminal 2
source ~/dojo_env.sh off && ros2 run web_pi_bridge fastapi_node
# terminal 3
source ~/dojo_env.sh off
curl localhost:8000/api/dispenser/status                         # online: true, remaining [4,4,4]
curl -X POST -H 'Content-Type: application/json' -d '{"station_id":1,"count":2}' localhost:8000/api/dispenser/dispense
```

**Expected:** dispensing 2 from station 1 takes about 3 s and leaves 2 pieces; Reset refills.
Screen buttons answer `[FAKE PI] showing organic`; the stream image stays broken (it points at the Pi).

Without `fake_pi.py` the page still loads: the dispenser shows OFFLINE and screen buttons report the service as unavailable.

## Map requests

Lets the judges ask a robot for its map. Teams only need Python 3: no ROS package, no pip installs.

```
Judges' page (Map Request)  --request-->  server queue  <--poll--  team's dojo_map.py (or /team page)
Judges' page  <-- shows image --  server  <-- upload PGM / RViz screenshot --  team
```

### Judges

1. Dropdown -> **Map Request**.
2. Choose **PGM map**, **RViz screenshot** or **Either**. Optionally type a team name (blank = any team may answer).
3. Press **Request Map**. The request shows WAITING until answered (it expires after 10 minutes; **Cancel** removes it).
4. The map appears by itself, with the team, size and resolution. For PGMs you get **Download PNG** and **Download PGM**.
   If a team could not capture a map, the reason shows in red on the request.

Files received are stored in `~/dojo_maps` (override with `DOJO_MAP_DIR`). The list shown on the page
is in memory and clears when the server restarts; the files stay on disk.

Optional token: start the server with `DOJO_TOKEN=secret` and teams must send it
(`--token secret`, or `export DOJO_TOKEN=secret` on their machine).

**Network:** teams must be able to reach `http://<laptop-ip>:8000`. WSL2's default network mode blocks
other machines: set `networkingMode=mirrored` in `C:\Users\<you>\.wslconfig` and run `wsl --shutdown`
(or add a `netsh interface portproxy` rule and a firewall rule for port 8000).

### Robot side (copy into the robot workspace)

Only one file is needed: [`robot/dojo_map.py`](robot/dojo_map.py). Teams can also download it from the judges:
`curl -fsSL http://<laptop-ip>:8000/static/dojo_map.py -o dojo_map.py`.

**Option A: no build changes.** Copy it into the package's `scripts/` folder and run it directly:

```bash
cp dojo_map.py ~/auracle_ws/src/auracle_bringup/scripts/
chmod +x ~/auracle_ws/src/auracle_bringup/scripts/dojo_map.py
source /opt/ros/jazzy/setup.bash && source ~/auracle_ws/install/setup.bash   # robot's ROS environment (for PGM capture)
python3 ~/auracle_ws/src/auracle_bringup/scripts/dojo_map.py --server http://<judges-ip>:8000 --team Auracle --watch
```

**Option B: installed, so `ros2 run` and launch work.** In `auracle_bringup/CMakeLists.txt` add
(or add the filename to your existing `install(PROGRAMS ...)` for `scripts/`):

```cmake
install(PROGRAMS scripts/dojo_map.py DESTINATION lib/${PROJECT_NAME})
```

Make sure the `launch/` folder is installed too, and copy [`robot/dojo_map.launch.py`](robot/dojo_map.launch.py) into it. Then:

```bash
cd ~/auracle_ws && colcon build --packages-select auracle_bringup --symlink-install && source install/setup.bash
ros2 run auracle_bringup dojo_map.py --server http://<judges-ip>:8000 --team Auracle            # answer the current request once
ros2 launch auracle_bringup dojo_map.launch.py server:=http://<judges-ip>:8000                  # answer every request while running
```

**Script modes:**

```bash
python3 dojo_map.py --server URL --team NAME             # answer the current request, once
python3 dojo_map.py --server URL --team NAME --watch     # keep running, answer every request
python3 dojo_map.py --server URL --team NAME --push pgm  # send now (or: --push screenshot)
python3 dojo_map.py --server URL --team NAME --file ~/rviz.png   # send an existing .png / .jpg / .pgm
# --topic /map  changes the map topic. Settings can come from DOJO_SERVER / DOJO_TEAM / DOJO_TOKEN.
```

- **PGM:** reads one message from the map topic (default `/map`; find yours with `ros2 topic list | grep -i map`)
  and writes it like nav2's `map_saver`. The robot's SLAM must be running and in the same ROS environment.
- **Screenshot:** RViz has no screenshot command, so the script uses the first tool it finds
  (`gnome-screenshot`, `grim`, `scrot`, ImageMagick `import`, `maim`). With `xdotool` on X11 it captures only the RViz
  window; otherwise the whole screen, so keep RViz visible. Install a tool with
  `sudo apt install gnome-screenshot` (or `imagemagick xdotool`). If nothing works, screenshot manually and use `--file`.
- Teams without a terminal can open `http://<laptop-ip>:8000/team` and upload a file.

### Testing the map request

Run in three levels; each needs only the previous one working.

**1. Page and script only (no ROS, no robot).** On the laptop:

```bash
source ~/dojo_env.sh off && ros2 run web_pi_bridge fastapi_node
```

Open `http://localhost:8000/#maps`, press **Request Map** (PGM, team `Test`), then in another terminal:

```bash
python3 dojo_map.py --server http://localhost:8000 --team Test --file some_map.pgm    # any .pgm/.png
```

Expected: the request turns green ("received from Test") and the image appears. Also try requesting a screenshot and sending a `.png`.

**2. PGM capture from ROS (fake map, no robot).**

```bash
source ~/dojo_env.sh off
ros2 topic pub -r 1 --qos-durability transient_local --qos-reliability reliable /map nav_msgs/msg/OccupancyGrid \
 "{header: {frame_id: map}, info: {resolution: 0.05, width: 4, height: 4, origin: {position: {x: 0.0, y: 0.0, z: 0.0}, orientation: {w: 1.0}}}, data: [100,100,100,100,100,0,0,100,100,0,-1,100,100,100,100,100]}"
# in another terminal: press Request Map (PGM) on the page, then
python3 dojo_map.py --server http://localhost:8000 --team Test
```

Expected: a tiny 4 x 4 map with a black border, a grey cell and white inside.

**3. The real robot.** On the robot with SLAM running (`ros2 topic list | grep -i map`), start `dojo_map.py --watch` as above
(use the laptop's IP), then press Request Map for team `Auracle`. For screenshots, open RViz on the robot, run
`python3 dojo_map.py ... --push screenshot` once to check the capture tool works, then request one from the page.

## Running the dispenser as a service (recommended)

Keep **one systemd unit per node** (a crash in one doesn't take the others down) and control them together with one command.
[`deploy/loading_station.service`](deploy/loading_station.service) is a template: first copy the `User=` and
`Environment=` lines from `systemctl cat screen_manager.service` so it matches your working units and paths, then:

```bash
sudo cp deploy/loading_station.service /etc/systemd/system/
sudo systemctl daemon-reload && sudo systemctl enable --now loading_station.service

# everything at once
sudo systemctl restart zenohd-router screen_manager screen_stream loading_station
sudo systemctl status  screen_manager screen_stream loading_station
journalctl -u loading_station -f                  # live log
```

## Troubleshooting

| Symptom | Likely cause / fix |
|---|---|
| Dispenser badge OFFLINE | Pi node not running, wrong `ROS_DOMAIN_ID` / RMW, or the laptop can't reach the Zenoh router. Check `ros2 topic echo /gamefield/loading_station/status --once` |
| "loading_station_interfaces not installed" | Build it on the laptop and `source ~/dojo_env.sh on` again (see Setup, step 2) |
| Screen buttons say service unavailable | `sudo systemctl status screen_manager` on the Pi; same domain/RMW on the laptop |
| Stream image broken | Pi's address is hardcoded in `web_pi_bridge/templates/index.html`; check the Pi's stream at `http://<pi-ip>:8080` |
| `/dev/loading_station` missing | udev rule not installed or Arduino not replugged; use `serial_port:=/dev/ttyUSB0` as a workaround |
| Map script: "cannot reach ..." | Wrong IP, server not running, or WSL network mode (see Network above) |
| Map script: "no map received on /map" | SLAM not running, different topic (`--topic`), or different `ROS_DOMAIN_ID` |
| Map script: 401 | Judges set `DOJO_TOKEN`; pass `--token` |
| Screenshot is black or empty | Wayland/WSL limitation; use `--file` with a manual screenshot |