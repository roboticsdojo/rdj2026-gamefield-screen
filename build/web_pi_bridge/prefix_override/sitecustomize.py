import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/kitana/ros2_ws/src/rdj2026-gamefield-screen/install/web_pi_bridge'
