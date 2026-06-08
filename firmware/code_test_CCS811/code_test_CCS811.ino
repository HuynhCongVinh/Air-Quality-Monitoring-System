#include "Adafruit_CCS811.h"

Adafruit_CCS811 ccs;

void setup() {
  // ESP32 thường dùng baudrate 115200
  Serial.begin(115200);
  Serial.println("Bắt đầu test CCS811 với ESP32");

  // Khởi tạo cảm biến (Mặc định địa chỉ I2C là 0x5A)
  // Nếu mạch của bạn dùng địa chỉ 0x5B, hãy đổi thành: ccs.begin(0x5B)
  if(!ccs.begin()){
    Serial.println("Lỗi: Không tìm thấy cảm biến CCS811!");
    Serial.println("1. Kiểm tra lại dây nối (SDA, SCL).");
    Serial.println("2. Đảm bảo chân WAKE đã được nối với GND.");
    while(1); // Dừng chương trình tại đây nếu không thấy cảm biến
  }

  // Đợi cảm biến sẵn sàng
  while(!ccs.available());
  Serial.println("CCS811 đã sẵn sàng!");
}

void loop() {
  if(ccs.available()){
    // readData() trả về 0 nếu đọc thành công
    if(!ccs.readData()){
      Serial.print("eCO2: ");
      Serial.print(ccs.geteCO2());
      Serial.print(" ppm, TVOC: ");
      Serial.print(ccs.getTVOC());
      Serial.println(" ppb");
    }
    else{
      Serial.println("Lỗi: Không thể đọc dữ liệu từ cảm biến!");
      ccs.readData(); // Thử reset lại thanh ghi
    }
  }
  
  // Đọc dữ liệu mỗi 1 giây
  delay(1000);
}