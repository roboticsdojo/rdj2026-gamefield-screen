"""Dispenser (loading station) side of the web bridge.

Kept separate from fastapi_node.py so the screen and dispenser code don't mix.
Provides a ROS 2 node plus a FastAPI router mounted under /api/dispenser.

ROS interface it talks to (all under /gamefield/loading_station):
  action   dispense  loading_station_interfaces/action/Dispense
  service  enable    std_srvs/srv/SetBool
  service  home      std_srvs/srv/Trigger
  service  reset     std_srvs/srv/Trigger
  topic    status    loading_station_interfaces/msg/StationStatus (latched)
"""
import asyncio
import threading
import time

from fastapi import APIRouter
from pydantic import BaseModel, Field

from action_msgs.msg import GoalStatus
from rclpy.action import ActionClient
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, HistoryPolicy, QoSProfile, ReliabilityPolicy
from std_srvs.srv import SetBool, Trigger

try:
    from loading_station_interfaces.action import Dispense
    from loading_station_interfaces.msg import StationStatus
    INTERFACES_OK = True
except ImportError:  # lets the screen page keep working if this package isn't built
    INTERFACES_OK = False

BASE = '/gamefield/loading_station'
STATUS_STALE_S = 3.0  # node publishes at 2 Hz; older than this = considered offline


class DispenserBridgeNode(Node):
    def __init__(self):
        super().__init__('dispenser_web_bridge')
        self._lock = threading.Lock()
        self._status = None          # dict from the last StationStatus
        self._status_time = 0.0
        self._goal = None            # dict describing the latest dispense goal
        self._goal_handle = None

        self.enable_cli = self.create_client(SetBool, f'{BASE}/enable')
        self.home_cli = self.create_client(Trigger, f'{BASE}/home')
        self.reset_cli = self.create_client(Trigger, f'{BASE}/reset')
        self.action_cli = ActionClient(self, Dispense, f'{BASE}/dispense')

        # must be TRANSIENT_LOCAL to match the latched publisher
        latched = QoSProfile(
            depth=1,
            history=HistoryPolicy.KEEP_LAST,
            reliability=ReliabilityPolicy.RELIABLE,
            durability=DurabilityPolicy.TRANSIENT_LOCAL)
        self.create_subscription(StationStatus, f'{BASE}/status', self._on_status, latched)
        self.get_logger().info('DispenserBridgeNode initialized')

    # ------------------------------------------------------------ status
    def _on_status(self, msg):
        with self._lock:
            self._status = {
                'enabled': bool(msg.enabled),
                'serial_connected': bool(msg.serial_connected),
                'state': msg.state,
                'remaining': [int(r) for r in msg.remaining],
                'last_message': msg.last_message,
            }
            self._status_time = time.monotonic()

    def snapshot(self):
        with self._lock:
            online = (self._status is not None and
                      time.monotonic() - self._status_time < STATUS_STALE_S)
            return {
                'online': online,
                'status': dict(self._status) if self._status else None,
                'goal': dict(self._goal) if self._goal else None,
            }

    # ---------------------------------------------------------- services
    async def _call(self, client, request, timeout_sec=12.0):
        # home/reset move servos and can take several seconds, hence the long timeout
        if not client.service_is_ready():
            return False, 'Loading station service unavailable'
        future = client.call_async(request)
        elapsed = 0.0
        while not future.done() and elapsed < timeout_sec:
            await asyncio.sleep(0.05)
            elapsed += 0.05
        if not future.done():
            return False, 'Service request timed out'
        try:
            res = future.result()
            return res.success, res.message
        except Exception as e:
            return False, f'Service execution failed: {e}'

    async def call_enable(self, enabled: bool):
        req = SetBool.Request()
        req.data = enabled
        return await self._call(self.enable_cli, req)

    async def call_home(self):
        return await self._call(self.home_cli, Trigger.Request())

    async def call_reset(self):
        return await self._call(self.reset_cli, Trigger.Request())

    # ------------------------------------------------------------ action
    def _goal_active(self):
        return self._goal is not None and self._goal['state'] in ('SENDING', 'RUNNING')

    def send_dispense(self, station_id: int, count: int):
        """Fire-and-forget: returns at once, progress is exposed via snapshot()."""
        if not self.action_cli.server_is_ready():
            return False, 'Loading station action server unavailable'
        with self._lock:
            if self._goal_active():
                return False, 'A dispense is already in progress'
            self._goal = {'station': station_id, 'count': count, 'state': 'SENDING',
                          'operation': '', 'pieces': 0, 'message': ''}
            self._goal_handle = None

        goal = Dispense.Goal()
        goal.station_id = station_id
        goal.count = count
        fut = self.action_cli.send_goal_async(goal, feedback_callback=self._on_feedback)
        fut.add_done_callback(self._on_goal_response)
        return True, f'Dispensing {count} from station {station_id}...'

    def _on_goal_response(self, fut):
        handle = fut.result()
        with self._lock:
            if not handle.accepted:
                self._goal.update(state='REJECTED', message='Goal rejected by loading station')
                return
            self._goal_handle = handle
            self._goal['state'] = 'RUNNING'
        handle.get_result_async().add_done_callback(self._on_result)

    def _on_feedback(self, fb_msg):
        fb = fb_msg.feedback
        with self._lock:
            if self._goal:
                self._goal.update(operation=fb.current_operation, pieces=int(fb.pieces_dispensed))

    def _on_result(self, fut):
        wrapped = fut.result()
        names = {GoalStatus.STATUS_SUCCEEDED: 'SUCCEEDED',
                 GoalStatus.STATUS_CANCELED: 'CANCELED',
                 GoalStatus.STATUS_ABORTED: 'ABORTED'}
        with self._lock:
            if self._goal:
                self._goal.update(state=names.get(wrapped.status, 'ABORTED'),
                                  pieces=int(wrapped.result.pieces_dispensed),
                                  message=wrapped.result.message)

    def cancel_dispense(self):
        with self._lock:
            handle = self._goal_handle if self._goal_active() else None
        if handle is None:
            return False, 'Nothing to cancel'
        handle.cancel_goal_async()
        return True, 'Cancel requested'


# ------------------------------------------------------------ FastAPI side
router = APIRouter(prefix='/api/dispenser', tags=['dispenser'])
node = None  # set by fastapi_node.lifespan


def make_node():
    """Create the node, or None if the interface package isn't installed."""
    return DispenserBridgeNode() if INTERFACES_OK else None


class EnableRequest(BaseModel):
    enabled: bool


class DispenseRequest(BaseModel):
    station_id: int = Field(ge=1, le=3)
    count: int = Field(default=1, ge=1, le=4)


def _unavailable():
    return {'success': False,
            'message': 'loading_station_interfaces not installed on this machine'}


@router.get('/status')
async def get_status():
    if node is None:
        return {'online': False, 'status': None, 'goal': None, 'error': _unavailable()['message']}
    return node.snapshot()


@router.post('/dispense')
async def dispense(req: DispenseRequest):
    if node is None:
        return _unavailable()
    ok, msg = node.send_dispense(req.station_id, req.count)
    return {'success': ok, 'message': msg}


@router.post('/cancel')
async def cancel():
    if node is None:
        return _unavailable()
    ok, msg = node.cancel_dispense()
    return {'success': ok, 'message': msg}


@router.post('/enable')
async def enable(req: EnableRequest):
    if node is None:
        return _unavailable()
    ok, msg = await node.call_enable(req.enabled)
    return {'success': ok, 'message': msg}


@router.post('/home')
async def home():
    if node is None:
        return _unavailable()
    ok, msg = await node.call_home()
    return {'success': ok, 'message': msg}


@router.post('/reset')
async def reset():
    if node is None:
        return _unavailable()
    ok, msg = await node.call_reset()
    return {'success': ok, 'message': msg}
