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
- Also contains the dispenser bridge node + `/api/dispenser/*` router and the
  map-exchange `/api/maps/*` router (previously `dispenser_bridge.py` and
  `map_exchange.py`; everything server-side now lives in this one file).
- Serves `templates/index.html` (for both `/` and `/team`) and mounts `static/` at `/static`,
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

One page, one stylesheet, one script: `templates/index.html` + `static/style.css` +
`static/app.js`. `app.js` is split into labelled sections (screen, dispenser, obstacles,
maps, team, device switcher); the switcher must stay last. `/team` serves the same
page, and `app.js` detects the path and shows only the team upload view.

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

## Dispenser panel (added)

The page header has a dropdown (**Gamefield Screen / Gamefield Dispenser**). The
choice is kept in the URL hash, e.g. `http://<laptop-ip>:8000/#dispenser`.

Files, all separate from the screen ones:

| Where | Role |
|-------|------|
| `web_pi_bridge/fastapi_node.py` (section 2) | ROS node + FastAPI router for `/api/dispenser/*` |
| `static/app.js` (dispenser section) | Panel logic, polls `/api/dispenser/status` every 1 s while visible |
| `static/style.css` (dispenser section) | Panel styling |
| `static/app.js` (device switcher) | Dropdown / panel switcher |

| Method | Path | Body | Does |
|--------|------|------|------|
| GET  | `/api/dispenser/status`  | - | Latest `StationStatus` + current/last dispense goal |
| POST | `/api/dispenser/dispense`| `{station_id: 1-3, count: 1-4}` | Sends a `Dispense` action goal (returns immediately) |
| POST | `/api/dispenser/cancel`  | - | Cancels the running goal |
| POST | `/api/dispenser/enable`  | `{enabled: bool}` | `/gamefield/loading_station/enable` |
| POST | `/api/dispenser/home`    | - | `/gamefield/loading_station/home` |
| POST | `/api/dispenser/reset`   | - | `/gamefield/loading_station/reset` |

Needs `loading_station_interfaces` built on the laptop. If it is missing, the
screen page still works and the dispenser panel shows an "interfaces not
installed" message.

```bash
colcon build --packages-select loading_station_interfaces && source install/setup.bash
```

## Obstacle layout panel (added)

Third dropdown entry, **Obstacle Layout** (`/#obstacles`). Front-end only, no ROS:
it randomises which 3 of the 6 cells of a 3 x 2 grid hold the movable blocks, so
you know how to set up the real gamefield. It never repeats the previous layout,
and "Copy Layout" copies the cell names and coordinates.

Code: the obstacles sections of `static/app.js` and `static/style.css`, plus the panel in `index.html`.

The grid's position comes from `Gamefield.stl`: the largest area with no walls or
pockets is x -151..1489 mm, y 845..2265 mm (about 1640 x 1420 mm). Cell centres
shown on the page are in that STL coordinate frame. Columns are A-C left to
right, rows 1-2 top to bottom (row 1 = larger y). To change the grid, edit the
constants at the top of the obstacles section of `app.js` (`COLS`, `ROWS`, `OBSTACLES`, `AREA`).

## Map request system (added)

Lets the judges ask a team for its map and see it on the page. Third-party
friendly: teams only need Python 3, not this ROS workspace.

```
judge page (Map Request)  --request-->  server queue  <--poll--  team script / /team page
judge page  <--shows image--  server  <--upload PGM or screenshot--  team script / /team page
```

**Judges:** dropdown -> *Map Request*. Pick *PGM map*, *RViz screenshot* or *Either*,
optionally a team name (blank = any team may answer), press *Request Map*. The
request shows as WAITING until answered, and can be cancelled. Unanswered requests
expire after 10 minutes. A team that cannot capture reports why, shown in red.
Received maps appear on the page automatically; PGMs are converted to PNG for
display, and both *Download PNG* and *Download PGM* (original) are offered.
Files are stored in `~/dojo_maps` (override with `DOJO_MAP_DIR`).

**Teams (Ubuntu 24.04, Pi or PC):**

```bash
curl -fsSL http://<laptop-ip>:8000/static/dojo_map.py -o dojo_map.py
source /opt/ros/jazzy/setup.bash          # only needed for PGM capture
python3 dojo_map.py --server http://<laptop-ip>:8000 --team Alpha            # answer the current request once
python3 dojo_map.py --server http://<laptop-ip>:8000 --team Alpha --watch    # answer every request automatically
python3 dojo_map.py --server ... --team Alpha --push pgm                     # send now, unrequested
python3 dojo_map.py --server ... --team Alpha --file ~/rviz.png              # send an existing file
```

- **PGM:** subscribes once to `/map` (`--topic` to change), writes it like nav2
  `map_saver` (free 254, occupied 0, unknown 205) and uploads it with resolution
  and origin. Needs the robot's ROS environment (same `ROS_DOMAIN_ID` / RMW).
- **Screenshot:** RViz has no screenshot command, so the script uses the first
  tool found (`gnome-screenshot`, `grim`, `scrot`, ImageMagick `import`, `maim`).
  On X11 with `xdotool` it captures just the RViz window, otherwise the whole
  screen, so keep RViz visible. If nothing works, take the screenshot manually and
  use `--file`. A headless Pi should use PGM.
- Teams without a terminal can use `http://<laptop-ip>:8000/team` (pick file, Send).

**Optional token:** start the server with `DOJO_TOKEN=secret` and teams must pass
`--token secret` (or `DOJO_TOKEN` in their environment; the `/team` page shows a
token box). Judges' actions on the page are not protected, like the rest of this page.

| Method | Path | Does |
|--------|------|------|
| GET  | `/api/maps/status` | Recent requests and received maps |
| POST | `/api/maps/request` | `{format: pgm\|screenshot\|any, team}` creates a request |
| POST | `/api/maps/request/{id}/cancel` | Cancel a pending request |
| GET  | `/api/maps/pending?team=` | Requests a team should answer |
| POST | `/api/maps/upload?team=&request_id=` | Raw file body (.pgm/.png/.jpg, max 25 MB) |
| POST | `/api/maps/request/{id}/fail` | Team reports it could not capture |
| GET  | `/api/maps/file/{id}[/original]` | Display image / original file |

Code: `fastapi_node.py` (section 3, server), the maps and team sections of `static/app.js`
and `static/style.css` (judges' panel + team page, the team page is the `#view-team` block in
`index.html`), and `static/dojo_map.py` (team script, kept as its own file because teams
download it with `curl` and it must run without this workspace). Pure Python 3 / FastAPI,
no new dependencies.

