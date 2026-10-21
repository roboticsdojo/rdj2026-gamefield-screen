#  rdj2026-gamefield-screen
Contains code that controls the gamefield screen. It integrates with the web_pi_bridge running on the pc. 
This main folder lives on the gamefield-screen repo. (should probably move that). The web bridge on your laptop only needs the dispenser’s message and action definitions (loading_station_interfaces). You get those by building that one package and sourcing both workspaces in the same terminal. If it’s missing, the screen page still works and the dispenser panel says so.

Folders that run on the pi related to the gamefield screen: 
- screen_images
- screen_interfaces
Folders that run on the pi related to the loading station:
- loading_station
- loading_station_interfaces


## Testing without gamefield pi


## Launching