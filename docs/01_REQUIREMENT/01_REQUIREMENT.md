# 01 - Requirements

## 1. Project Overview

**Project Title:**
Nghiên cứu, phát triển hệ thống quan trắc chất lượng không khí tích hợp xử lý dữ liệu và AI tại thiết bị biên (Edge Node)

**Project Type:**
Graduation Thesis

**Domain:**
IoT / Embedded Systems / Edge Computing / AI

## 2. Project Objectives

Dự án này nhằm mục đích:

1. Phát triển thiết bị IoT để thu thập dữ liệu chất lượng không khí.
2. Thu thập dữ liệu môi trường từ nhiều cảm biến.
3. Lưu trữ dữ liệu đo lường cục bộ.
4. Truyền dữ liệu bằng giao thức MQTT.
5. Triển khai điện toán biên trên Raspberry Pi.
6. Xử lý và xác thực dữ liệu cục bộ.
7. Triển khai phát hiện hoặc phân tích bất thường dựa trên trí tuệ nhân tạo (AI).
8. Cung cấp dịch vụ dữ liệu phía máy chủ.
9. Phát triển giao diện người dùng để giám sát dữ liệu.
10. Đánh giá toàn bộ hệ thống.

## 3. Functional Requirements

### FR-01 — Thu thập dữ liệu cảm biến
Hệ thống phải thu thập:

- PM1.0
- PM2.5
- PM10
- Nhiệt độ
- Độ ẩm
- eCO2
- TVOC

### FR-02 — Timestamp
Hệ thống phải liên kết dữ liệu đo với Timestamp.

### FR-03 — Ghi nhật ký dữ liệu cục bộ
Thiết bị IoT phải hỗ trợ lưu trữ dữ liệu đo cục bộ.

### FR-04 — Giao tiếp MQTT
Thiết bị IoT phải publish dữ liệu đo thông qua MQTT.

### FR-05 — Xử lý dữ liệu tại thiết bị biên
Thiết bị biên phải nhận và xử lý dữ liệu đo cục bộ.

### FR-06 — Xác thực dữ liệu
Thiết bị biên phải xác thực dữ liệu cảm biến đến.

### FR-07 — Phân tích AI
Hệ thống phải cung cấp phân tích dựa trên AI/ML để phát hiện các điều kiện chất lượng không khí bất thường.

### FR-08 — Trực quan hóa dữ liệu
Hệ thống phải cung cấp giao diện người dùng để giám sát dữ liệu chất lượng không khí.

## 4. Non-Functional Requirements

### NFR-01 — Độ tin cậy
Hệ thống cần tiếp tục thu thập dữ liệu cục bộ khi kết nối Internet tạm thời bị gián đoạn.

### NFR-02 — Hiệu năng
Hệ thống cần xử lý dữ liệu cảm biến đến với độ trễ đủ thấp cho mục đích giám sát.

### NFR-03 — Khả năng bảo trì
Phần mềm cần có cấu trúc mô-đun và dễ bảo trì.

### NFR-04 — Khả năng mở rộng
Kiến trúc cần cho phép bổ sung thêm thiết bị IoT hoặc nguồn dữ liệu.

### NFR-05 — Khả năng sử dụng
Giao diện giám sát cần hiển thị thông tin cảm biến một cách rõ ràng.

### NFR-06 — Hiệu quả sử dụng tài nguyên
Việc xử lý dữ liệu tại biên cần phù hợp với phần cứng Raspberry Pi hiện có.

## 5. System Requirements

Hệ thống sẽ bao gồm:
1. IoT Device
2. Communication Layer
3. Edge Device
4. Application / Web Interface

IoT Device
    ↓
Communication
    ↓
Edge
    ↓
Application (Node-RED Website)

## 6. Hardware Requirements

### IoT Device
IoT Device cần cung cấp:
- Vi điều khiển có kết nối Wi-Fi
- Cảm biến bụi mịn (PM).
- Cảm biến nhiệt độ/độ ẩm.
- Cảm biến khí/chất lượng không khí.
- Đồng hồ thời gian thực (RTC).
- Bộ nhớ cục bộ.

### Phần cứng đã chọn hiện tại

| Thành phần | Lựa chọn hiện tại |
|---|---|
| Vi điều khiển (MCU) | ESP32 |
| Cảm biến bụi mịn (PM) | PMS7003 |
| Cảm biến khí | CCS811 |
| Cảm biến nhiệt độ/độ ẩm | DHT22 |
| RTC | DS3231 |
| Bộ nhớ | MicroSD |

**Note:** Current hardware selection is an implementation decision
and may be revised if necessary.

## 7. Software & Technology Requirements

Current technologies:
- ESP32 firmware: C/C++
- Communication: MQTT
- Edge: Raspberry Pi
- Edge programming: Python
- AI/ML: TBD
- Web application: TBD

## 8. Data Requirements

Hệ thống sẽ thu thập và xử lý:
### Bụi mịn
- PM1.0
- PM2.5
- PM10

### Dữ liệu môi trường
- Nhiệt độ
- Độ ẩm

### Khí/Chất lượng không khí
- eCO2
- TVOC

### Siêu dữ liệu
- Dấu thời gian
- Mã định danh thiết bị
- Nguồn dữ liệu

## 9. AI Requirements
Chưa quyết định

