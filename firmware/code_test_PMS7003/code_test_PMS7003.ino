#include "PMS.h"

// Khởi tạo PMS7003 trên Serial2
// Chân 16 là RX (Nối với TX của G7)
// Chân 17 là TX (Nối với RX của G7)
PMS pms(Serial2);
PMS::DATA data;

void setup() {
  // Serial Monitor để xem kết quả
  Serial.begin(115200);
  
  // Serial2 kết nối với PMS7003
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  Serial.println("--- Kiem tra cam bien PMS7003 ---");
  Serial.println("Dang khoi dong...");
  
  pms.wakeUp(); 
  delay(2000); // Đợi cảm biến ổn định nguồn
}

void loop() {
  Serial.println("Dang yeu cau doc du lieu...");
  
  // Thử đọc dữ liệu
  if (pms.readUntil(data)) {
    Serial.println("------ KET QUA ------");
    Serial.print("PM 1.0 (ug/m3): "); Serial.println(data.PM_AE_UG_1_0);
    Serial.print("PM 2.5 (ug/m3): "); Serial.println(data.PM_AE_UG_2_5);
    Serial.print("PM 10.0 (ug/m3): "); Serial.println(data.PM_AE_UG_10_0);
    Serial.println("---------------------");
  } else {
    Serial.println("Loi: Khong nhan duoc du lieu tu PMS7003. Dang kiem tra lai...");
  }

  delay(3000); // Đợi 3 giây rồi đọc lại
}