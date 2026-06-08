import paho.mqtt.client as mqtt
import json
import joblib
import pandas as pd

# 1. Load mô hình AI đa chỉ số
model = joblib.load('air_quality_multi_model.pkl')

# 2. Cấu hình Topic
MQTT_SERVER = "10.49.25.150"
MQTT_TOPIC_REAL = "sensor/air_quality"  # Topic để ĐỌC dữ liệu từ ESP32
MQTT_TOPIC_PRED = "sensor/prediction"  # Topic để GỬI kết quả AI (theo ý bạn)


def on_message(client, userdata, message):
    try:
        # Nhận dữ liệu thực tế
        payload = json.loads(message.payload.decode("utf-8"))

        # Chuẩn bị dữ liệu cho AI
        current_data = pd.DataFrame([[
            payload['temp'], payload['hum'], payload['eco2'],
            payload['tvoc'], payload['pm1.0'], payload['pm2.5'], payload['pm10']
        ]], columns=['temp', 'hum', 'eco2', 'tvoc', 'pm1.0', 'pm2.5', 'pm10'])

        # AI dự báo
        predictions = model.predict(current_data)[0]

        # Tạo gói tin dự báo JSON
        predict_payload = {
            "target_pm25": round(predictions[0], 2),
            "target_eco2": round(predictions[1], 2),
            "source": "AI_Model"
        }

        # Gửi lên MQTT
        client.publish(MQTT_TOPIC_PRED, json.dumps(predict_payload))

        print(f"--- Đã gửi dự báo lên {MQTT_TOPIC_PRED} ---")
        print(f"PM2.5 tiếp theo: {predict_payload['target_pm25']}")
        print(f"eCO2 tiếp theo: {predict_payload['target_eco2']}")

    except Exception as e:
        print(f"Lỗi: {e}")


# Khởi tạo và kết nối
client = mqtt.Client()
client.on_message = on_message
client.connect(MQTT_SERVER, 1883, 60)

# Quan trọng: Subscribe Topic của ESP32 để lấy dữ liệu đầu vào
client.subscribe(MQTT_TOPIC_REAL)

print(f"Đang đợi dữ liệu từ {MQTT_TOPIC_REAL}...")
client.loop_forever()