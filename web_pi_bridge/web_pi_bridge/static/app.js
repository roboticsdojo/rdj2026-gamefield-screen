// Device switcher: shows one panel at a time, remembers the choice in the URL hash
// (e.g. http://<laptop>:8000/#dispenser). To add another device later, add an
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

    select.addEventListener('change', () => show(select.value));
    const initial = location.hash.slice(1);
    show(PANELS[initial] ? initial : 'screen');
})();
