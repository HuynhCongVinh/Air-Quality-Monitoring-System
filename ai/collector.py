import paho.mqtt.client as mqtt
import json
import pandas as pd
import os
from datetime import datetime

# --- Cấu hình ---
MQTT_SERVER = "localhost"  # Vì Mosquitto chạy trên cùng máy tính
MQTT_TOPIC = "sensor/air_quality"
CSV_FILE = "air_quality_data.csv"


def on_message(client, userdata, message):
    try:
        # Giải mã dữ liệu JSON từ ESP32
        data = json.loads(message.payload.decode("utf-8"))

        # Thêm thời gian hệ thống của máy tính để chính xác hơn
        data['timestamp_pc'] = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

        print(f"Đã nhận: {data}")

        # Chuyển sang DataFrame
        df = pd.DataFrame([data])

        # Ghi vào CSV (append mode)
        if not os.path.isfile(CSV_FILE):
            df.to_csv(CSV_FILE, index=False)
        else:
            df.to_csv(CSV_FILE, mode='a', header=False, index=False)

    except Exception as e:
        print(f"Lỗi xử lý dữ liệu: {e}")


# Khởi tạo MQTT Client
client = mqtt.Client()
client.on_message = on_message

print("Đang kết nối tới Broker...")
client.connect(MQTT_SERVER, 1883, 60)
client.subscribe(MQTT_TOPIC)

# Bắt đầu vòng lặp nhận dữ liệu
client.loop_forever()