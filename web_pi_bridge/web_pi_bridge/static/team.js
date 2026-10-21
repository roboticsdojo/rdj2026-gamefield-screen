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

    updateCmd(); updateSend(); refresh(); setInterval(refresh, 2000);
})();
