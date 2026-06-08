#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);

  if (! rtc.begin()) {
    Serial.println("Khong tim thay RTC DS3231!");
    while (1);
  }

  // Nếu RTC bị mất nguồn (ví dụ: mới lắp pin), đặt lại thời gian theo thời gian biên dịch code
  if (rtc.lostPower()) {
    Serial.println("RTC bi mat nguon, thiet lap lai thoi gian...");
    // Dòng lệnh này sẽ lấy thời gian từ máy tính lúc bạn nhấn Upload
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DateTime now = rtc.now();

  Serial.print("Thoi gian: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  // Đợi 1 giây rồi đọc lại
  delay(1000);
}