# Project Status

**Project:** Air Quality Monitoring using Edge Computing and AI
**Project Type:** Graduation Thesis
**Status:** Active
**Version:** 1.0
**Last Updated:** 2026-09-15

## 1. Project Objective

Phát triển hệ thống quan trắc chất lượng không khí sử dụng
IoT, Edge Computing và AI.

Hệ thống có khả năng:
- Thu thập dữ liệu từ các cảm biến.
- Lưu trữ dữ liệu cục bộ.
- Truyền dữ liệu thông qua MQTT.
- Xử lý dữ liệu tại Edge.
- Phân tích/phát hiện bất thường bằng AI.
- Gửi dữ liệu đến Server.
- Hiển thị và trực quan hóa dữ liệu.

## 2. Current Architecture

Current architecture:

ESP32 IoT Device
        ↓
      MQTT
        ↓
Node-RED Dashboard

**Architecture Status:** IN PROGRESS

## 4. Completed

- Đã xác định hướng đề tài.
- Đã xác định kiến trúc IoT Device → Edge → Server → Application.
- Đã lựa chọn ESP32 và kết nối với các cảm biến PMS7003, CCS811, DHT22, RTC DS3231 và SD card Module làm IoT Device.
- Đã hoàn thành nối dây IoT Device và test code gửi lên Node-RED qua MQTT
- Đã thực hiện chạy AI tạm thời trực tiếp trên Laptop để test tính năng

## 5. In Progress

- Hoàn thiện firmware ESP32.

## 6. Not Started

- Raspberry Pi Edge processing.
- Web Server.
- Phát hiện sự bất thường của AI.
- Kiểm tra cấp hệ thống.
- Đánh giá hiệu suất.