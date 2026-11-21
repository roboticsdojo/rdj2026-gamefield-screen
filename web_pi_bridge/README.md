copy to home directory on PC
nano dojo_env.sh
```
SCREEN_REPO=$HOME/ros2_ws/src/rdj2026-gamefield-screen
DISP_WS=$HOME/ros2_ws/src/rdj2026-gamefield-dispenser/loading_zone_ws

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
launch
```
source ~/dojo_env.sh
cd ~/ros2_ws
colcon build --packages-select screen_interfaces && source install/setup.bash
colcon build --packages-select web_pi_bridge --symlink-install && source install/setup.bash
ros2 run web_pi_bridge fastapi_node
```
On laptop open `http://localhost:8000`