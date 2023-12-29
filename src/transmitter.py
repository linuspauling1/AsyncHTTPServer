import websocket
from time import sleep
 
ws = websocket.WebSocket()
ws.connect("ws://192.168.88.7/ws")
while True:
    ws.send('biggus dickus')
    sleep(5)