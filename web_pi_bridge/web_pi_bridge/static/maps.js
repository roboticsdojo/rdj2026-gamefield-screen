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
