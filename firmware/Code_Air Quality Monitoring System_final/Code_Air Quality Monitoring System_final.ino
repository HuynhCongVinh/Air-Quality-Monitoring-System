#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include "Adafruit_CCS811.h"
#include <Wire.h>
#include "RTClib.h"
#include "PMS.h"
#include <SPI.h>
#include <SD.h>

// --- CẤU HÌNH HỆ THỐNG ---
const char* ssid = "R0mj";
const char* password = "thoaimai";
const char* mqtt_server = "10.49.25.150"; 

// --- KHAI BÁO CHÂN VÀ THIẾT BỊ ---
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

Adafruit_CCS811 ccs;
RTC_DS3231 rtc;
float current_co2 = 0;
float current_tvoc = 0;

const int chipSelect = 5; 

PMS pms(Serial2);
PMS::DATA data;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
bool sdReady = false;

void setup_wifi() {
  delay(10);
  Serial.println("\n--- Ket noi WiFi ---");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi da ket noi!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Dang ket noi MQTT...");
    String clientId = "ESP32-AirQuality-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println(" Thanh cong!");
    } else {
      Serial.print("That bai, ma loi = ");
      Serial.print(client.state());
      Serial.println(" Thu lai sau 5 giay...");
      delay(5000);
    }
  }
}

void init_SD() {
  Serial.println("Dang khoi tao the SD...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Loi: Khong tim thay hoac loi the SD!");
    sdReady = false;
    return;
  }
  Serial.println("The SD san sang.");
  sdReady = true;

  if (!SD.exists("/datalog.csv")) {
    File logFile = SD.open("/datalog.csv", FILE_WRITE);
    if (logFile) {
      logFile.println("Time,Temperature(C),Humidity(%),eCO2(ppm),TVOC(ppb),PM1.0,PM2.5,PM10");
      logFile.close();
      Serial.println("Da tao file datalog.csv moi voi Header.");
    }
  }
}

// --- HÀM ĐỒNG BỘ DỮ LIỆU LỊCH SỬ ---
void sync_history() {
  if (!sdReady) return;
  
  Serial.println("\n=== DANG DONG BO DU LIEU LICH SU TU THE SD ===");
  File logFile = SD.open("/datalog.csv", FILE_READ);
  
  if (logFile) {
    // Đọc bỏ qua dòng tiêu đề thứ nhất (Header)
    if (logFile.available()) {
      logFile.readStringUntil('\n');
    }
    
    int count = 0;
    // Đọc từng dòng còn lại và gửi lên MQTT
    while (logFile.available()) {
      String line = logFile.readStringUntil('\n');
      line.trim(); // Xóa khoảng trắng thừa hoặc ký tự \r
      
      if (line.length() > 0) {
        // Gửi trực tiếp chuỗi CSV thô lên topic lịch sử, Node-RED sẽ tự tách sau
        client.publish("sensor/history", line.c_str());
        count++;
        delay(50); // Delay nhỏ (50ms) để tránh làm nghẽn băng thông MQTT Broker
      }
    }
    logFile.close();
    Serial.print("=== DONG BO HOAN TAT! DA GUI ");
    Serial.print(count);
    Serial.println(" DONG DU LIEU QUA KHI ===");
  } else {
    Serial.println("Loi: Khong the mo file de doc lich su!");
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  dht.begin();
  if(!ccs.begin()) Serial.println("Loi: Khong tim thay CCS811!");
  if (!rtc.begin()) Serial.println("Loi: Khong tim thay RTC!");

  init_SD();

  // Đảm bảo kết nối MQTT thành công trước khi đổ dữ liệu lịch sử
  if (!client.connected()) {
    reconnect();
  }
  
  // Gọi hàm đồng bộ lịch sử ngay khi khởi động
  sync_history();

  pms.wakeUp(); 
  Serial.println("He thong san sang!");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // 2. ĐỌC CCS811 LIÊN TỤC Ở MỖI VÒNG LẶP (Giúp giữ Bus I2C ổn định, không bị timeout)
  if (ccs.available() && !ccs.readData()) {
    current_co2 = ccs.geteCO2();
    current_tvoc = ccs.getTVOC();
  }

  unsigned long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;

    int pm1_0 = 0, pm2_5 = 0, pm10 = 0;
    if (pms.readUntil(data)) {
      pm1_0 = data.PM_AE_UG_1_0;
      pm2_5 = data.PM_AE_UG_2_5;
      pm10 = data.PM_AE_UG_10_0;
    }

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) { h = 0.0; t = 0.0; }

    float co2 = current_co2;
    float tvoc = current_tvoc;

    DateTime rtcTime = rtc.now();
    char timestamp[25];
    sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d", 
            rtcTime.year(), rtcTime.month(), rtcTime.day(), 
            rtcTime.hour(), rtcTime.minute(), rtcTime.second());

    String jsonPayload = "{";
    jsonPayload += "\"time\":\"" + String(timestamp) + "\",";
    jsonPayload += "\"temp\":" + String(t, 1) + ",";
    jsonPayload += "\"hum\":" + String(h, 1) + ",";
    jsonPayload += "\"eco2\":" + String(co2, 0) + ",";
    jsonPayload += "\"tvoc\":" + String(tvoc, 0) + ",";
    jsonPayload += "\"pm1.0\":" + String(pm1_0) + ",";
    jsonPayload += "\"pm2.5\":" + String(pm2_5) + ",";
    jsonPayload += "\"pm10\":" + String(pm10);
    jsonPayload += "}";

    Serial.print("Gui MQTT Realtime: ");
    Serial.println(jsonPayload);
    client.publish("sensor/air_quality", jsonPayload.c_str());

    if (sdReady) {
      // Thay FILE_WRITE bằng FILE_APPEND để dữ liệu xếp hàng nối đuôi nhau
      File logFile = SD.open("/datalog.csv", FILE_APPEND);
      if (logFile) {
        logFile.print(timestamp); logFile.print(",");
        logFile.print(t, 1);       logFile.print(",");
        logFile.print(h, 1);       logFile.print(",");
        logFile.print(co2, 0);     logFile.print(",");
        logFile.print(tvoc, 0);    logFile.print(",");
        logFile.print(pm1_0);      logFile.print(",");
        logFile.print(pm2_5);      logFile.print(",");
        logFile.println(pm10);
        logFile.close();
        Serial.println("Da luu thong so vao the SD Card.");
      } else {
        Serial.println("Loi: Khong the mo file datalog.csv de ghi!");
      }
    } else {
      init_SD();
    }
  }
}