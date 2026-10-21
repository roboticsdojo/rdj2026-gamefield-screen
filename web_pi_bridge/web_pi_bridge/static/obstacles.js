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
