"""Map exchange between the judges (this web page) and the competing teams.

Pure FastAPI, no ROS: competitors reach it over plain HTTP, so their machines only
need Python 3 (see static/dojo_map.py).

Flow
  1. A judge clicks "Request" on the Map Request panel     -> POST /api/maps/request
  2. A team's script (or the /team page) sees it            -> GET  /api/maps/pending
  3. The team uploads a PGM map or an RViz screenshot       -> POST /api/maps/upload
  4. The judges' page picks it up while polling             -> GET  /api/maps/status
     PGM files are converted to PNG for display (browsers can't show PGM); the
     original is kept and downloadable.

Environment
  DOJO_MAP_DIR  where received files are stored (default ~/dojo_maps)
  DOJO_TOKEN    if set, teams must send it as header X-Dojo-Token to upload
"""
import os
import re
import struct
import time
import uuid
import zlib
from pathlib import Path
from typing import Literal, Optional

from fastapi import APIRouter, Header, HTTPException, Request
from fastapi.responses import FileResponse
from pydantic import BaseModel

MAX_BYTES = 25 * 1024 * 1024     # largest accepted upload
REQUEST_TTL_S = 600              # an unanswered request expires after 10 minutes
KEEP = 50                        # how many requests / maps to remember
PNG_MAGIC = b'\x89PNG\r\n\x1a\n'

router = APIRouter(prefix='/api/maps', tags=['maps'])

_requests = []   # oldest first
_maps = []       # oldest first


def map_dir() -> Path:
    return Path(os.environ.get('DOJO_MAP_DIR') or Path.home() / 'dojo_maps')


# ---------------------------------------------------------------- file formats
def sniff(data: bytes):
    """Return (kind, ext): kind is 'pgm' or 'screenshot'; None if unsupported."""
    if data[:2] in (b'P5', b'P2'):
        return 'pgm', 'pgm'
    if data[:8] == PNG_MAGIC:
        return 'screenshot', 'png'
    if data[:3] == b'\xff\xd8\xff':
        return 'screenshot', 'jpg'
    return None


def _pgm_tokens(data, count, start=2):
    toks, i, n = [], start, len(data)
    while len(toks) < count and i < n:
        c = data[i:i + 1]
        if c == b'#':
            while i < n and data[i:i + 1] not in (b'\n', b'\r'):
                i += 1
        elif c.isspace():
            i += 1
        else:
            j = i
            while j < n and not data[j:j + 1].isspace():
                j += 1
            toks.append(data[i:j])
            i = j
    return toks, i


def parse_pgm(data: bytes):
    """Parse P5 (binary) or P2 (ascii) PGM -> (width, height, 8-bit pixel bytes)."""
    toks, i = _pgm_tokens(data, 3)
    if len(toks) < 3:
        raise ValueError('truncated PGM header')
    w, h, maxval = (int(t) for t in toks)
    if w <= 0 or h <= 0 or not 0 < maxval <= 65535:
        raise ValueError('bad PGM header')
    count = w * h
    if data[:2] == b'P5':
        raw = data[i + 1:]                       # exactly one whitespace byte after maxval
        if maxval > 255:
            if len(raw) < count * 2:
                raise ValueError('PGM raster is shorter than its header says')
            raw = raw[0:count * 2:2]             # keep the high byte
            maxval = 255 if maxval == 65535 else (maxval >> 8)
        elif len(raw) < count:
            raise ValueError('PGM raster is shorter than its header says')
        px = raw[:count]
    else:
        vals = data[i:].split()
        if len(vals) < count:
            raise ValueError('PGM raster is shorter than its header says')
        px = bytes(min(255, int(v)) for v in vals[:count])
    if maxval not in (255, 0):
        table = bytes(min(255, v * 255 // maxval) for v in range(256))
        px = px.translate(table)
    return w, h, px


def gray_png(w: int, h: int, px: bytes) -> bytes:
    """Minimal 8-bit grayscale PNG encoder (no Pillow needed)."""
    def chunk(tag, body):
        return (struct.pack('>I', len(body)) + tag + body +
                struct.pack('>I', zlib.crc32(tag + body) & 0xffffffff))
    raw = b''.join(b'\x00' + px[y * w:(y + 1) * w] for y in range(h))
    return (PNG_MAGIC + chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 0, 0, 0, 0)) +
            chunk(b'IDAT', zlib.compress(raw, 6)) + chunk(b'IEND', b''))


# --------------------------------------------------------------------- helpers
def _check_token(token: str):
    required = os.environ.get('DOJO_TOKEN', '')
    if required and token != required:
        raise HTTPException(401, 'Wrong or missing token (set DOJO_TOKEN / --token)')


def _clean_team(name: str) -> str:
    return re.sub(r'[^A-Za-z0-9 _.-]', '', (name or '').strip())[:32]


def _expire():
    now = time.time()
    for r in _requests:
        if r['status'] == 'pending' and now - r['created'] > REQUEST_TTL_S:
            r['status'] = 'expired'


def _find_request(rid: str):
    return next((r for r in _requests if r['id'] == rid), None)


def _public_request(r):
    return {**r, 'age_s': int(time.time() - r['created'])}


def _public_map(m):
    return {k: m[k] for k in ('id', 'team', 'kind', 'width', 'height', 'resolution',
                              'origin_x', 'origin_y', 'request_id', 'time', 'size', 'ext')} | {
        'display_url': f"/api/maps/file/{m['id']}",
        'original_url': f"/api/maps/file/{m['id']}/original",
    }


# ------------------------------------------------------------------ judge side
class NewRequest(BaseModel):
    format: Literal['pgm', 'screenshot', 'any'] = 'any'
    team: str = ''


@router.get('/status')
async def status():
    _expire()
    return {
        'token_required': bool(os.environ.get('DOJO_TOKEN')),
        'requests': [_public_request(r) for r in reversed(_requests[-15:])],
        'maps': [_public_map(m) for m in reversed(_maps[-30:])],
    }


@router.post('/request')
async def create_request(req: NewRequest):
    r = {'id': uuid.uuid4().hex[:6], 'format': req.format, 'team': _clean_team(req.team),
         'status': 'pending', 'created': time.time(), 'map_id': None,
         'answered_by': '', 'message': ''}
    _requests.append(r)
    del _requests[:-KEEP]
    return {'success': True, 'message': f"Requested {req.format} map"
            + (f" from {r['team']}" if r['team'] else ' from any team'), 'id': r['id']}


@router.post('/request/{rid}/cancel')
async def cancel_request(rid: str):
    r = _find_request(rid)
    if r is None or r['status'] != 'pending':
        return {'success': False, 'message': 'Request is not pending'}
    r['status'] = 'cancelled'
    return {'success': True, 'message': 'Request cancelled'}


def _map_file(map_id: str):
    m = next((m for m in _maps if m['id'] == map_id), None)   # lookup by id, never by path
    if m is None:
        raise HTTPException(404, 'Unknown map')
    return m


@router.get('/file/{map_id}')
async def display_file(map_id: str):
    m = _map_file(map_id)
    path = map_dir() / m['display_file']
    if not path.exists():
        raise HTTPException(404, 'File missing on disk')
    return FileResponse(path, media_type='image/jpeg' if m['ext'] == 'jpg' else 'image/png')


@router.get('/file/{map_id}/original')
async def original_file(map_id: str):
    m = _map_file(map_id)
    path = map_dir() / m['file']
    if not path.exists():
        raise HTTPException(404, 'File missing on disk')
    return FileResponse(path, media_type='application/octet-stream', filename=m['file'])


# ------------------------------------------------------------- competitor side
@router.get('/pending')
async def pending(team: str = '', x_dojo_token: str = Header(default='')):
    _check_token(x_dojo_token)
    _expire()
    t = _clean_team(team).lower()
    return {'requests': [_public_request(r) for r in _requests
                         if r['status'] == 'pending' and (not r['team'] or r['team'].lower() == t)]}


@router.post('/upload')
async def upload(request: Request, team: str = '', request_id: str = '', filename: str = '',
                 resolution: Optional[float] = None, origin_x: Optional[float] = None,
                 origin_y: Optional[float] = None, x_dojo_token: str = Header(default='')):
    _check_token(x_dojo_token)
    _expire()
    if int(request.headers.get('content-length') or 0) > MAX_BYTES:
        raise HTTPException(413, f'File too large (max {MAX_BYTES // 1024 // 1024} MB)')
    body = await request.body()
    if len(body) > MAX_BYTES:
        raise HTTPException(413, f'File too large (max {MAX_BYTES // 1024 // 1024} MB)')

    sniffed = sniff(body)
    if sniffed is None:
        raise HTTPException(422, 'Unsupported file: send a .pgm, .png or .jpg')
    kind, ext = sniffed
    team = _clean_team(team) or 'unknown'

    req = None
    if request_id:
        req = _find_request(request_id)
        if req is None or req['status'] != 'pending':
            raise HTTPException(409, f'Request {request_id} is not pending (answered, cancelled or expired)')
        if req['format'] != 'any' and req['format'] != kind:
            raise HTTPException(422, f"Request {request_id} wants a {req['format']}, but this file is a {kind}")
        if req['team'] and req['team'].lower() != team.lower():
            raise HTTPException(403, f"Request {request_id} is addressed to team {req['team']}")

    width = height = None
    display_ext = ext
    display_bytes = body
    if kind == 'pgm':
        try:
            width, height, px = parse_pgm(body)
        except ValueError as e:
            raise HTTPException(422, f'Bad PGM: {e}')
        display_bytes, display_ext = gray_png(width, height, px), 'png'
    elif ext == 'png' and len(body) >= 24:
        width, height = struct.unpack('>II', body[16:24])

    mid = uuid.uuid4().hex[:8]
    stem = f"{time.strftime('%H%M%S')}_{re.sub(r'[^A-Za-z0-9_-]', '_', team)}_{mid}"
    d = map_dir()
    try:
        d.mkdir(parents=True, exist_ok=True)
        (d / f'{stem}.{ext}').write_bytes(body)
        if display_ext != ext or display_bytes is not body:
            (d / f'{stem}_view.{display_ext}').write_bytes(display_bytes)
    except OSError as e:
        raise HTTPException(500, f'Could not store file in {d}: {e}')

    _maps.append({'id': mid, 'team': team, 'kind': kind, 'ext': ext, 'width': width,
                  'height': height, 'resolution': resolution, 'origin_x': origin_x,
                  'origin_y': origin_y, 'request_id': req['id'] if req else None,
                  'time': time.time(), 'size': len(body), 'file': f'{stem}.{ext}',
                  'display_file': f'{stem}_view.{display_ext}' if display_ext != ext else f'{stem}.{ext}'})
    del _maps[:-KEEP]
    if req:
        req.update(status='fulfilled', map_id=mid, answered_by=team)
    return {'success': True, 'map_id': mid,
            'message': f'Received {kind} from {team}' + (f' for request {req["id"]}' if req else ' (unrequested)')}


class Failure(BaseModel):
    team: str = ''
    message: str = ''


@router.post('/request/{rid}/fail')
async def fail_request(rid: str, f: Failure, x_dojo_token: str = Header(default='')):
    _check_token(x_dojo_token)
    r = _find_request(rid)
    if r is None or r['status'] != 'pending':
        raise HTTPException(409, 'Request is not pending')
    r.update(status='failed', answered_by=_clean_team(f.team) or 'unknown', message=f.message[:200])
    return {'success': True, 'message': 'Failure reported to judges'}
