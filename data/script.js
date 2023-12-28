
var gateway = `ws://${window.location.hostname}`;
var websocket;
window.addEventListener('load', onload);

function onload(event) {
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(event){
    websocket.send("Ciao!");
}

function onClose(event) {
    websocket.send("Adio!");
}

function onMessage(event) {
    alert(event.data);
}