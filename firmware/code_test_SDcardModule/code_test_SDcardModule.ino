#include <SPI.h>
#include <SD.h>

// Định nghĩa chân CS (Chip Select)
const int chipSelect = 5;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Đợi cổng Serial kết nối (chỉ dùng cho mục đích debug)
  }

  Serial.println("\n--- Bat dau kiem tra Module SD Card ---");

  // Khởi tạo thẻ SD
  if (!SD.begin(chipSelect)) {
    Serial.println("Khai tao the SD THAT BAI!");
    Serial.println("Vui long kiem tra: 1. Day noi | 2. The da cam chua | 3. Dinh dang FAT32?");
    return;
  }
  Serial.println("Khai tao the SD THANH CONG.");

  // Kiem tra loai the (Optional)
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("Khong tim thay the SD hop le");
    return;
  }

  // --- THỬ NGHIỆM GHI DỮ LIỆU ---
  Serial.println("Dang ghi file test.txt...");
  File myFile = SD.open("/test.txt", FILE_WRITE);

  if (myFile) {
    myFile.println("IoT Project: Test ket noi the SD voi ESP32.");
    myFile.println("Du lieu ghi vao luc: Thoi gian thuc.");
    myFile.close(); // Bat buoc phai dong file sau khi ghi de luu du lieu
    Serial.println("Ghi file hoan tat.");
  } else {
    Serial.println("Loi khi mo file de ghi!");
  }

  // --- THỬ NGHIỆM ĐỌC DỮ LIỆU ---
  Serial.println("Dang doc file test.txt de kiem tra...");
  myFile = SD.open("/test.txt");
  if (myFile) {
    while (myFile.available()) {
      Serial.write(myFile.read()); // In du lieu ra Serial Monitor
    }
    myFile.close();
    Serial.println("--- Ket thuc test doc file ---");
  } else {
    Serial.println("Loi khi mo file de doc!");
  }
}

void loop() {
  // De trong vi chi can chay test 1 lan o setup
}