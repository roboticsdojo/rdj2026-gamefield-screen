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

Folders that run on the pi related to the gamefield screen: 
- screen_images
- screen_interfaces
Folders that run on the pi related to the loading station:
- loading_station
- loading_station_interfaces


## Setup
Set the two paths to wherever the repos live, and save this as ~/dojo_env.sh
```bash
SCREEN_REPO=$HOME/rdj2026-gamefield-screen
DISP_WS=$HOME/rdj2026-gamefield-dispenser/loading_zone_ws

source /opt/ros/jazzy/setup.bash
source $DISP_WS/install/setup.bash 2>/dev/null     # underlay (dispenser interfaces)
source $SCREEN_REPO/install/setup.bash 2>/dev/null # overlay (screen + web bridge)

if [ "$1" = off ]; then
  unset RMW_IMPLEMENTATION; export ROS_DOMAIN_ID=99     # isolated, no zenoh needed
  echo "Pi OFF mode (DDS, domain 99)"
else
  export RMW_IMPLEMENTATION=rmw_zenoh_cpp; export ROS_DOMAIN_ID=62
  # plus whatever you already use to reach the Pi's zenoh router
  echo "Pi ON mode (zenoh, domain 62)"
fi
```
Build on WSL
```
source /opt/ros/jazzy/setup.bash
cd $DISP_WS && colcon build --packages-select loading_station_interfaces
source install/setup.bash
cd $SCREEN_REPO && colcon build --packages-select screen_interfaces web_pi_bridge --symlink-install
```
Build on the pi
```
cd ~/path/to/rdj2026-gamefield-dispenser/loading_zone_ws
colcon build --symlink-install && source install/setup.bash
sudo cp udev/99-loading-station.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules && sudo udevadm trigger   # then replug the Arduino
ls -l /dev/loading_station
```
## Launching
The screen nodes are already systemd services (can we merge these services, or have one for the dispenser too)

On the pi
```
sudo systemctl status zenohd-router screen_manager screen_stream
export ROS_DOMAIN_ID=62 RMW_IMPLEMENTATION=rmw_zenoh_cpp
source ~/path/to/loading_zone_ws/install/setup.bash
ros2 launch loading_station loading_station.launch.py
# if the udev link isn't there: ... serial_port:=/dev/ttyUSB0
```
On the laptop
```
source ~/dojo_env.sh on
ros2 run web_pi_bridge fastapi_node        # then open http://localhost:8000
```
Test from a second terminal on the laptop
```
ros2 service list | grep -E "screen_manager|loading_station"
ros2 topic echo /gamefield/loading_station/status --once
ros2 service call /screen_manager_node/set_display_class screen_interfaces/srv/SetDisplayClass "{category: 'organic'}"

# via the web bridge (same path the buttons use)
curl localhost:8000/api/dispenser/status
curl -X POST -H 'Content-Type: application/json' -d '{"category":"organic"}' localhost:8000/api/set_display_class
curl -X POST localhost:8000/api/dispenser/home                  # moves servos
curl -X POST -H 'Content-Type: application/json' -d '{"station_id":1,"count":1}' localhost:8000/api/dispenser/dispense
```
These move real servos, so keep the mechanism clear first. In the browser, the dispenser badge should read IDLE, and the screen stream should show the Pi’s display.
