#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include "Adafruit_CCS811.h"
#include <Wire.h>
#include "RTClib.h"

// Thông tin WiFi
const char* ssid = "KTX2-201";
const char* password = "6868686868";

// Địa chỉ IP của máy tính chạy Mosquitto Broker (ví dụ: 192.168.1.x)
const char* mqtt_server = "192.168.1.9"; 

// Khởi tạo các đối tượng
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
Adafruit_CCS811 ccs;
RTC_DS3231 rtc;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Dang ket noi toi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nDa ket noi WiFi!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Dang ket noi MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println(" Thanh cong!");
    } else {
      Serial.print("That bai, ma loi = ");
      Serial.print(client.state());
      Serial.println(" Thu lai sau 5 giay...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  // Khởi tạo các cảm biến
  dht.begin();
  if(!ccs.begin()){
    Serial.println("Khong tim thay CCS811!");
  }
  rtc.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // 1. Đọc dữ liệu DHT22
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // 2. Đọc dữ liệu CCS811
  float co2 = 0;
  float tvoc = 0;
  if(ccs.available()){
    if(!ccs.readData()){
      co2 = ccs.geteCO2();
      tvoc = ccs.getTVOC();
    }
  }

  // 3. Đọc dữ liệu thời gian từ RTC
  DateTime now = rtc.now();
  String timestamp = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " " +
                     String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

  // 4. Tạo chuỗi JSON gửi dữ liệu
  String jsonPayload = "{\"timestamp\":\"" + timestamp + "\"," +
                       "\"temperature\":" + String(t, 1) + "," +
                       "\"humidity\":" + String(h, 1) + "," +
                       "\"eCO2\":" + String(co2, 0) + "," +
                       "\"TVOC\":" + String(tvoc, 0) + "}";

  // In ra Serial Monitor để theo dõi
  Serial.print("Gui du lieu: ");
  Serial.println(jsonPayload);

  // Gửi lên MQTT Broker
  client.publish("sensor/air_quality", jsonPayload.c_str());

  // Đợi 10 giây cho lần gửi tiếp theo
  delay(10000);
}