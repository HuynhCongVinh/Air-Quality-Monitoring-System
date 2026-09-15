# 02 - System Architecture

## 1. Architecture Overview

The system follows a layered IoT architecture consisting of:

1. IoT Device
2. Communication Layer
3. Edge Computing Layer
4. Application Layer

The general data flow is:

┌─────────────────────┐
│     IoT Device      │
│       ESP32         │
│                     │
│ PMS7003             │
│ CCS811              │
│ DHT22               │
│ DS3231              │
│ MicroSD             │
└──────────┬──────────┘
           │
         MQTT
           │
           ▼
┌─────────────────────┐
│    Edge Device      │
│    Raspberry Pi     │
│                     │
│ Data Processing     │
│ Data Validation     │
│ Local Storage       │
│ AI / ML             │
└──────────┬──────────┘
           │
           │
         MQTT
           │
           ▼
┌─────────────────────┐
│    Application      │
│   Web Dashboard     |
|      (Node-RED)     │
└─────────────────────┘

## 2. Architecture Goals

The architecture aims to:
- Separate sensor data acquisition from data processing.
- Perform local processing at the Edge.
- Reduce dependence on cloud/server-side processing.
- Support local data storage when network connectivity is unavailable.
- Enable AI-based analysis near the data source.
- Provide a scalable architecture for future extensions.

## 3. System Components

### IoT Device

Responsible for:
- Sensor data acquisition.
- Timestamping.
- Local data logging.
- MQTT publishing.

### Edge Device

Responsible for:
- MQTT data reception.
- Data validation.
- Data preprocessing.
- Local storage.
- AI inference.
- MQTT publishing

### Application (Node-RED Website)

Responsible for:
- MQTT data reception.
- Data visualization.
- Monitoring.
- Historical data display.
- Alerts.

## 4. Data Flow

### Step 1 — Data Acquisition
ESP32 reads measurements from the connected sensors.

### Step 2 — Timestamping
DS3231 provides the timestamp associated with each measurement.

### Step 3 — Local Logging
The ESP32 stores measurement data on the MicroSD card.

### Step 4 — MQTT Publishing
The ESP32 publishes measurement data through MQTT.

### Step 5 — Edge Processing
The Raspberry Pi receives MQTT messages and performs:
- Data validation
- Data preprocessing
- Local storage
- AI analysis

### Step 6 — Server Communication

Processed data is transmitted to the server Node-RED through MQTT.

### Step 7 — Visualization

The web application retrieves and displays the data.

## 5. Edge Architecture

The Raspberry Pi acts as the Edge Computing device.

Main responsibilities:

1. Receive MQTT data.
2. Validate incoming data.
3. Detect invalid or abnormal sensor readings.
4. Preprocess measurement data.
5. Store data locally.
6. Perform AI inference.
7. Provide processed data to the Node-RED server through MQTT

## 6. Data Architecture

## 7. Error Handling & Failure Scenarios