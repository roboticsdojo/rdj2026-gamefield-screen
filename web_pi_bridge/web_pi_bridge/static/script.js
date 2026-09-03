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