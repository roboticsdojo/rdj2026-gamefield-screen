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
