// Robotics Dojo control page - all front-end logic in one file.
// Sections: screen, dispenser, obstacles, maps (judges), team (/team), device switcher.

// ======================================================================
// SCREEN PANEL (script.js)
// ======================================================================
async function sendDisplayCommand(showState) {
    const statusDiv = document.getElementById('status');
    statusDiv.innerText = "Sending request over ROS 2...";
    try {
        const response = await fetch('/api/set_display', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ state: showState })
        });
        const data = await response.json();
        statusDiv.innerText = `Result: ${data.message}`;
    } catch (err) {
        statusDiv.innerText = `Error connecting to web bridge`;
    }
}

async function sendClassCommand(category) {
    const statusDiv = document.getElementById('status');
    statusDiv.innerText = `Requesting '${category}' image...`;
    try {
        const response = await fetch('/api/set_display_class', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ category: category })
        });
        const data = await response.json();
        statusDiv.innerText = `Result: ${data.message}`;
    } catch (err) {
        statusDiv.innerText = `Error connecting to web bridge`;
    }
}

// ======================================================================
// DISPENSER PANEL (dispenser.js)
// ======================================================================
// Dispenser panel logic. Talks to /api/dispenser/* (see dispenser_bridge.py).
// Polls /status once a second while the panel is visible (started/stopped by app.js).
(function () {
    const CAPACITY = 4;          // compartments per station (loading_station.yaml: capacity)
    const POLL_MS = 1000;
    const $ = (id) => document.getElementById(id);

    let timer = null;
    let enabled = true;          // last known state, used by the enable/disable toggle
    let lastState = 'OFFLINE';

    // ---- build the 3 station cards once
    document.querySelectorAll('.station').forEach((card) => {
        const n = card.dataset.station;
        card.innerHTML = `
            <h3>STATION ${n}</h3>
            <div class="slots">${'<div class="slot"></div>'.repeat(CAPACITY)}</div>
            <div class="left">- left</div>
            <div class="station-actions">
                <select aria-label="Pieces to dispense from station ${n}">
                    ${[1, 2, 3, 4].map((c) => `<option value="${c}">${c} pc</option>`).join('')}
                </select>
                <button class="btn-class">Dispense</button>
            </div>`;
        const sel = card.querySelector('select');
        card.querySelector('button').addEventListener('click', () => dispense(+n, +sel.value));
    });

    function setStatus(text) { $('disp-status').textContent = text; }

    async function api(path, body) {
        const res = await fetch('/api/dispenser/' + path, {
            method: body === undefined ? 'GET' : 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: body === undefined ? undefined : JSON.stringify(body),
        });
        return res.json();
    }

    async function act(label, path, body) {
        setStatus(label);
        try {
            const data = await api(path, body);
            setStatus(`Result: ${data.message}`);
        } catch (e) {
            setStatus('Error connecting to web bridge');
        }
        poll();
    }

    // ---- actions (global so the inline onclick="" in index.html can reach them)
    function dispense(station, count) {
        return act(`Dispensing ${count} from station ${station}...`, 'dispense',
                   { station_id: station, count });
    }
    window.dispEnableToggle = () => act(enabled ? 'Disabling...' : 'Enabling...', 'enable', { enabled: !enabled });
    window.dispHome = () => act('Homing (takes a few seconds)...', 'home', {});
    window.dispReset = () => act('Resetting (takes a few seconds)...', 'reset', {});
    window.dispCancel = () => act('Cancelling...', 'cancel', {});

    // ---- rendering
    function render(snap) {
        const st = snap.online && snap.status ? snap.status : null;
        const state = st ? st.state : 'OFFLINE';
        lastState = state;
        enabled = st ? st.enabled : enabled;

        const badge = $('disp-badge');
        badge.textContent = state;
        badge.className = 'badge badge-' + state.toLowerCase();

        $('disp-conn').textContent = !st
            ? (snap.error || 'waiting for loading station...')
            : (st.serial_connected ? 'Arduino connected' : 'Arduino NOT connected')
              + (st.last_message ? ' - ' + st.last_message : '');

        const idle = state === 'IDLE';
        const busy = state === 'DISPENSING' || state === 'HOMING';
        document.querySelectorAll('.station').forEach((card, i) => {
            const left = st && st.remaining[i] !== undefined ? st.remaining[i] : null;
            card.querySelectorAll('.slot').forEach((slot, k) => {
                slot.classList.toggle('full', left !== null && k >= CAPACITY - left);
            });
            card.querySelector('.left').textContent = left === null ? '- left' : `${left} left`;
            card.querySelector('button').disabled = !idle || left === 0;
        });

        $('disp-enable').textContent = enabled ? 'Disable' : 'Enable';
        $('disp-enable').disabled = !st;
        $('disp-home').disabled = !st || !st.serial_connected || busy;
        $('disp-reset').disabled = !st;

        const g = snap.goal;
        const active = g && (g.state === 'SENDING' || g.state === 'RUNNING');
        $('disp-cancel').disabled = !active;
        if (!g) {
            $('disp-goal').textContent = 'No dispense yet';
        } else if (active) {
            $('disp-goal').textContent = `Station ${g.station}: ${g.operation || 'starting...'} (${g.pieces}/${g.count})`;
        } else {
            $('disp-goal').textContent = `Last dispense - station ${g.station}: ${g.state}, ${g.pieces}/${g.count}` +
                (g.message ? ` (${g.message})` : '');
        }
    }

    async function poll() {
        try {
            render(await api('status'));
        } catch (e) {
            render({ online: false, status: null, goal: null, error: 'web bridge unreachable' });
        }
    }

    window.dispenserStart = () => { if (!timer) { poll(); timer = setInterval(poll, POLL_MS); } };
    window.dispenserStop = () => { clearInterval(timer); timer = null; };
})();

// ======================================================================
// OBSTACLE LAYOUT PANEL (obstacles.js)
// ======================================================================
// Obstacle layout randomiser. Pure front-end: nothing is sent to ROS, it only tells
// the person which of the 6 grid cells to put the 3 movable blocks in.
(function () {
    // ---- grid geometry (from Gamefield.stl: the largest free area on the field) ----
    // To flip the grid to 2 columns x 3 rows, swap COLS/ROWS (cell sizes follow).
    const COLS = 3, ROWS = 2, OBSTACLES = 3;
    const AREA = { x0: -151, x1: 1489, y0: 845, y1: 2265 };   // mm, STL coordinates

    const $ = (id) => document.getElementById(id);
    const grid = $('obs-grid');
    const cellW = (AREA.x1 - AREA.x0) / COLS;
    const cellH = (AREA.y1 - AREA.y0) / ROWS;

    // Cell names: columns A,B,C left->right, rows 1,2 top->bottom (top = larger y).
    const cells = [];
    for (let r = 0; r < ROWS; r++) {
        for (let c = 0; c < COLS; c++) {
            cells.push({
                name: String.fromCharCode(65 + c) + (r + 1),
                cx: Math.round(AREA.x0 + (c + 0.5) * cellW),
                cy: Math.round(AREA.y1 - (r + 0.5) * cellH),
            });
        }
    }

    grid.style.gridTemplateColumns = `repeat(${COLS}, 1fr)`;
    grid.style.gridTemplateRows = `repeat(${ROWS}, 1fr)`;
    grid.style.aspectRatio = `${AREA.x1 - AREA.x0} / ${AREA.y1 - AREA.y0}`;
    const els = cells.map((cell) => {
        const el = document.createElement('div');
        el.className = 'obs-cell';
        el.innerHTML = `<span class="obs-name">${cell.name}</span>
                        <span class="obs-block">&#9632;</span>
                        <span class="obs-coord">x${cell.cx} y${cell.cy}</span>`;
        grid.appendChild(el);
        return el;
    });

    let current = [];   // indices of cells holding an obstacle, sorted

    function pick() {
        // Fisher-Yates shuffle, take the first OBSTACLES; retry so we never repeat the last layout
        let next;
        do {
            const idx = cells.map((_, i) => i);
            for (let i = idx.length - 1; i > 0; i--) {
                const j = Math.floor(Math.random() * (i + 1));
                [idx[i], idx[j]] = [idx[j], idx[i]];
            }
            next = idx.slice(0, OBSTACLES).sort((a, b) => a - b);
        } while (next.join() === current.join());
        return next;
    }

    function describe() {
        return current.map((i) => cells[i].name).join(', ');
    }

    function render() {
        els.forEach((el, i) => el.classList.toggle('has-obstacle', current.includes(i)));
        $('obs-status').textContent = `Obstacles at: ${describe()}`;
    }

    window.obstaclesRandomize = () => { current = pick(); render(); };

    window.obstaclesCopy = async () => {
        const text = 'Obstacles: ' + current.map((i) => `${cells[i].name} (x ${cells[i].cx}, y ${cells[i].cy})`).join('; ');
        try {
            await navigator.clipboard.writeText(text);
            $('obs-status').textContent = `Copied: ${describe()}`;
        } catch (e) {
            // clipboard API needs https/localhost; fall back to showing the text
            $('obs-status').textContent = text;
        }
    };

    window.obstaclesRandomize();   // start with a layout already shown
})();

// ======================================================================
// MAP REQUEST PANEL, judges (maps.js)
// ======================================================================
// Judges' Map Request panel. Talks to /api/maps/* (see map_exchange.py).
// Polls /status once a second while the panel is visible (started/stopped by app.js).
// Team names come from competitors, so everything is inserted with textContent.
(function () {
    const POLL_MS = 1000;
    const $ = (id) => document.getElementById(id);
    const FORMAT_LABEL = { pgm: 'PGM map', screenshot: 'RViz screenshot', any: 'Either' };

    let timer = null;
    let maps = [];
    let known = null;          // ids already seen (null until first poll)
    let selectedId = null;
    let followLatest = true;
    let historySig = '';

    function setStatus(t) { $('maps-status').textContent = t; }

    async function api(path, body) {
        const res = await fetch('/api/maps/' + path, {
            method: body === undefined ? 'GET' : 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: body === undefined ? undefined : JSON.stringify(body),
        });
        return res.json();
    }

    function el(tag, cls, text) {
        const e = document.createElement(tag);
        if (cls) e.className = cls;
        if (text !== undefined) e.textContent = text;
        return e;
    }
    const ago = (s) => (s < 60 ? `${s}s` : `${Math.floor(s / 60)}m ${s % 60}s`);

    window.mapsRequest = async () => {
        setStatus('Sending request...');
        try {
            const data = await api('request', { format: $('maps-format').value, team: $('maps-team').value });
            setStatus(`Result: ${data.message}`);
        } catch (e) { setStatus('Error connecting to web bridge'); }
        poll();
    };

    async function cancel(id) {
        try { await api(`request/${id}/cancel`, {}); } catch (e) { /* next poll shows state */ }
        poll();
    }

    function renderRequests(reqs) {
        const box = $('maps-requests');
        box.replaceChildren();
        reqs.slice(0, 4).forEach((r) => {
            const row = el('div', 'req req-' + r.status);
            const who = r.team || 'any team';
            let text = `${FORMAT_LABEL[r.format]} · ${who} · `;
            if (r.status === 'pending') text += `WAITING ${ago(r.age_s)}`;
            else if (r.status === 'fulfilled') text += `received from ${r.answered_by}`;
            else if (r.status === 'failed') text += `FAILED (${r.answered_by}): ${r.message || 'no reason given'}`;
            else text += r.status;
            row.appendChild(el('span', '', text));
            if (r.status === 'pending') {
                const b = el('button', 'btn-hide req-cancel', 'Cancel');
                b.addEventListener('click', () => cancel(r.id));
                row.appendChild(b);
            }
            box.appendChild(row);
        });
    }

    function select(id, fromUser) {
        selectedId = id;
        if (fromUser) followLatest = maps.length > 0 && maps[0].id === id;
        renderViewer();
        renderHistory();
    }

    function renderViewer() {
        const m = maps.find((x) => x.id === selectedId);
        const img = $('maps-img');
        $('maps-empty').hidden = !!m;
        img.hidden = !m;
        const actions = $('maps-actions');
        actions.replaceChildren();
        if (!m) { $('maps-meta').textContent = '-'; return; }

        const url = m.display_url + '?v=' + m.id;
        if (img.dataset.id !== m.id) { img.src = url; img.dataset.id = m.id; }
        img.className = m.kind === 'pgm' ? 'pixelated' : '';
        img.onclick = () => window.open(m.display_url, '_blank');

        const lines = [`${m.team} · ${m.kind === 'pgm' ? 'PGM map' : 'RViz screenshot'}`,
                       new Date(m.time * 1000).toLocaleTimeString()];
        if (m.width) lines.push(`${m.width} × ${m.height} px`);
        if (m.resolution) lines.push(`${m.resolution} m/px`);
        if (m.origin_x !== null && m.origin_x !== undefined) lines.push(`origin (${m.origin_x}, ${m.origin_y})`);
        lines.push(m.request_id ? 'answered a request' : 'unrequested');
        const meta = $('maps-meta');
        meta.replaceChildren();
        lines.forEach((l, i) => meta.appendChild(el('div', i === 0 ? 'meta-title' : '', l)));

        const a1 = el('a', 'btn-class dl', m.kind === 'pgm' ? 'Download PNG' : 'Download');
        a1.href = m.display_url; a1.download = `${m.team}_map.${m.kind === 'pgm' ? 'png' : m.ext}`;
        actions.appendChild(a1);
        if (m.kind === 'pgm') {
            const a2 = el('a', 'btn-show dl', 'Download PGM');
            a2.href = m.original_url;
            actions.appendChild(a2);
        }
    }

    function renderHistory() {
        const box = $('maps-history');
        box.replaceChildren();
        maps.slice(0, 12).forEach((m) => {
            const b = el('button', 'thumb' + (m.id === selectedId ? ' active' : ''));
            const i = el('img'); i.src = m.display_url; i.alt = '';
            if (m.kind === 'pgm') i.className = 'pixelated';
            b.appendChild(i);
            b.appendChild(el('span', '', `${m.team} · ${m.kind === 'pgm' ? 'PGM' : 'shot'}`));
            b.addEventListener('click', () => select(m.id, true));
            box.appendChild(b);
        });
    }

    function render(s) {
        renderRequests(s.requests);
        maps = s.maps;
        const ids = new Set(maps.map((m) => m.id));
        const fresh = known === null ? [] : maps.filter((m) => !known.has(m.id));
        known = ids;
        if (fresh.length) {
            setStatus(`Map received from ${fresh[0].team}`);
            followLatest = true;
        }
        if (followLatest && maps.length) selectedId = maps[0].id;
        if (selectedId && !ids.has(selectedId)) selectedId = maps.length ? maps[0].id : null;
        const sig = maps.map((m) => m.id).join();
        if (sig !== historySig) { historySig = sig; renderHistory(); }
        renderViewer();
    }

    async function poll() {
        try { render(await api('status')); }
        catch (e) { setStatus('Error connecting to web bridge'); }
    }

    window.mapsStart = () => { if (!timer) { poll(); timer = setInterval(poll, POLL_MS); } };
    window.mapsStop = () => { clearInterval(timer); timer = null; };
})();

// ======================================================================
// TEAM UPLOAD PAGE, /team (team.js)
// ======================================================================
// Competitor fallback page: shows the judges' request and uploads a file.
(function () {
    const $ = (id) => document.getElementById(id);
    let pending = [];
    let tokenRequired = false;

    $('team').value = localStorage.getItem('dojoTeam') || '';
    $('token').value = localStorage.getItem('dojoToken') || '';

    const headers = () => ({ 'X-Dojo-Token': $('token').value });
    const team = () => $('team').value.trim();

    function kindOf(file) {
        const n = file.name.toLowerCase();
        if (n.endsWith('.pgm')) return 'pgm';
        if (/\.(png|jpe?g)$/.test(n)) return 'screenshot';
        return null;
    }

    function updateCmd() {
        const t = team() || 'YOUR_TEAM';
        $('cmd').textContent =
            `curl -fsSL ${location.origin}/static/dojo_map.py -o dojo_map.py\n` +
            `python3 dojo_map.py --server ${location.origin} --team "${t}" --watch`;
    }

    function updateSend() { $('send').disabled = !($('file').files[0] && team()); }

    async function refresh() {
        try {
            const st = await (await fetch('/api/maps/status')).json();
            tokenRequired = st.token_required;
            $('token-row').hidden = !tokenRequired;
            const res = await fetch('/api/maps/pending?team=' + encodeURIComponent(team()), { headers: headers() });
            const data = await res.json();
            pending = res.ok ? data.requests : [];
            const box = $('pending');
            if (!res.ok) { box.className = 'pending none'; box.textContent = data.detail || 'Not allowed'; }
            else if (!pending.length) { box.className = 'pending none'; box.textContent = 'No request right now.'; }
            else {
                const r = pending[0], labels = { pgm: 'a PGM map', screenshot: 'an RViz screenshot', any: 'a map or RViz screenshot' };
                box.className = 'pending wait';
                box.textContent = `Judges want ${labels[r.format]} (${r.age_s}s ago)`;
            }
        } catch (e) { $('pending').textContent = 'Cannot reach the judges server'; }
    }

    $('file').addEventListener('change', () => {
        const f = $('file').files[0], img = $('preview');
        img.hidden = !(f && kindOf(f) === 'screenshot');      // browsers can't preview PGM
        if (!img.hidden) img.src = URL.createObjectURL(f);
        updateSend();
    });
    $('team').addEventListener('input', () => { localStorage.setItem('dojoTeam', team()); updateCmd(); updateSend(); });
    $('token').addEventListener('input', () => localStorage.setItem('dojoToken', $('token').value));

    $('send').addEventListener('click', async () => {
        const f = $('file').files[0], kind = kindOf(f), out = $('result');
        const req = pending.find((r) => r.format === 'any' || r.format === kind);
        const q = new URLSearchParams({ team: team(), filename: f.name });
        if (req) q.set('request_id', req.id);
        out.className = ''; out.textContent = 'Sending...';
        try {
            const res = await fetch('/api/maps/upload?' + q, {
                method: 'POST', headers: { ...headers(), 'Content-Type': 'application/octet-stream' }, body: f });
            const data = await res.json();
            out.className = res.ok ? 'ok' : 'err';
            out.textContent = res.ok ? data.message : (data.detail || 'Upload failed');
        } catch (e) { out.className = 'err'; out.textContent = 'Cannot reach the judges server'; }
        refresh();
    });

    $('copy').addEventListener('click', async () => {
        try { await navigator.clipboard.writeText($('cmd').textContent); $('copy').textContent = 'Copied'; }
        catch (e) { $('copy').textContent = 'Select the text and copy'; }
    });

    updateCmd(); updateSend();

    // Polling only runs in team mode (the /team page); judges' view never starts it.
    let timer = null;
    window.teamStart = () => { if (!timer) { refresh(); timer = setInterval(refresh, 2000); } };
    window.teamStop = () => { clearInterval(timer); timer = null; };
})();

// ======================================================================
// DEVICE SWITCHER (app.js) - must stay last
// ======================================================================
// Device switcher: shows one panel at a time, remembers the choice in the URL hash
// (e.g. http://<laptop>:8000/#dispenser); /team shows only the team upload view. To add another device later, add an
// <option>, a <main id="panel-<name>"> and an entry in PANELS below.
(function () {
    const select = document.getElementById('device-select');
    const PANELS = {
        screen: {
            el: document.getElementById('panel-screen'),
            // blank the MJPEG stream while hidden so it doesn't keep eating bandwidth
            onShow() { const i = document.getElementById('live-stream'); i.src = i.dataset.src; },
            onHide() { document.getElementById('live-stream').removeAttribute('src'); },
        },
        dispenser: {
            el: document.getElementById('panel-dispenser'),
            onShow() { window.dispenserStart(); },
            onHide() { window.dispenserStop(); },
        },
        maps: {
            el: document.getElementById('panel-maps'),
            onShow() { window.mapsStart(); },
            onHide() { window.mapsStop(); },
        },
        obstacles: {
            el: document.getElementById('panel-obstacles'),
            onShow() {},
            onHide() {},
        },
    };

    let current = null;
    function show(name) {
        if (!PANELS[name] || name === current) return;
        // hide every other panel (on first load nothing is "current" yet)
        Object.keys(PANELS).forEach((k) => {
            if (k === name) return;
            PANELS[k].el.hidden = true;
            if (k === current || current === null) PANELS[k].onHide();
        });
        PANELS[name].el.hidden = false;
        PANELS[name].onShow();
        current = name;
        select.value = name;
        history.replaceState(null, '', '#' + name);
    }

    // /team = competitor upload page: same file, but only the team view is shown.
    if (location.pathname.replace(/\/+$/, '') === '/team') {
        document.body.classList.add('team-mode');
        document.title = 'Robotics Dojo - Team Map Upload';
        Object.keys(PANELS).forEach((k) => { PANELS[k].el.hidden = true; });
        PANELS.screen.onHide();                       // don't load the MJPEG stream for teams
        document.getElementById('view-team').hidden = false;
        window.teamStart();
        return;
    }

    select.addEventListener('change', () => show(select.value));
    const initial = location.hash.slice(1);
    show(PANELS[initial] ? initial : 'screen');
})();

