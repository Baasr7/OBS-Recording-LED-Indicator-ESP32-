import time
import requests
from obsws_python import EventClient

ESP32_IP = "1.1.1.1"   # ← Replace with your ESP32's actual IP
OBS_HOST = "localhost"
OBS_PORT = 4455
OBS_PASS = "PASSWORD"   # ← From OBS WebSocket settings

def on_record_state_changed(data):
    state = data.output_state
    print(f"[OBS] Record state: {state}")

    if state == "OBS_WEBSOCKET_OUTPUT_STARTED":
        try:
            requests.get(f"http://{ESP32_IP}/record/on", timeout=2)
            print("→ LED ON (recording started)")
        except Exception as e:
            print(f"ESP32 unreachable: {e}")

    elif state == "OBS_WEBSOCKET_OUTPUT_STOPPED":
        try:
            requests.get(f"http://{ESP32_IP}/record/off", timeout=2)
            print("→ LED OFF (recording stopped)")
        except Exception as e:
            print(f"ESP32 unreachable: {e}")

client = EventClient(host=OBS_HOST, port=OBS_PORT, password=OBS_PASS)
client.callback.register(on_record_state_changed)

print("Listening for OBS recording events... (Ctrl+C to quit)")
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    client.disconnect()
    print("Disconnected.")
