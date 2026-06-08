#include "DHT.h"

// Khai báo chân kết nối và loại cảm biến
#define DHTPIN 4     // Chân GPIO kết nối với chân DATA của DHT22
#define DHTTYPE DHT22   // Sử dụng cảm biến DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println(F("Khoi tao DHT22..."));

  dht.begin();
}

void loop() {
  // Đợi 2 giây giữa các lần đọc (DHT22 có chu kỳ đọc dữ liệu khoảng 2s)
  delay(2000);

  // Đọc độ ẩm
  float h = dht.readHumidity();
  // Đọc nhiệt độ theo độ C (mặc định)
  float t = dht.readTemperature();

  // Kiểm tra xem việc đọc dữ liệu có bị lỗi hay không
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Khong the doc du lieu tu cam bien DHT22!"));
    return;
  }

  // In kết quả ra Serial Monitor
  Serial.print(F("Do am: "));
  Serial.print(h);
  Serial.print(F("%  Nhiet do: "));
  Serial.print(t);
  Serial.println(F("°C "));
}