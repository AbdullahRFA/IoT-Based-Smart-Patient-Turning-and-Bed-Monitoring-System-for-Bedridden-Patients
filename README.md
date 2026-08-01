# 🏥 IoT-Based Smart Patient Turning and Bed Monitoring System

An IoT-powered healthcare system designed to reduce the risk of pressure ulcers (bed sores) in bedridden patients by continuously monitoring body pressure, patient position, temperature, and humidity. The system automatically alerts caregivers and can optionally perform gentle bed tilting to assist patient repositioning. :contentReference[oaicite:0]{index=0}

---

## 📖 Overview

Bedridden patients are at high risk of developing pressure ulcers due to remaining in the same position for long periods. Continuous manual monitoring is difficult for caregivers, especially in hospitals with limited staff or home-care environments.

This project provides a low-cost IoT solution that continuously monitors:

- Body pressure
- Patient position duration
- Temperature
- Humidity

When the patient remains in the same position beyond a predefined threshold, the system:

- Sounds a buzzer
- Sends mobile notifications
- Uploads sensor data to the cloud
- Optionally tilts the bed using a servo motor

---

## ✨ Features

- 📍 Patient pressure monitoring using FSR sensor
- ⏱ Position duration tracking
- 🌡 Temperature monitoring
- 💧 Humidity monitoring
- 📱 Real-time caregiver notifications
- ☁ Cloud data logging using Blynk/Firebase
- 🔔 Local buzzer alert
- 💡 LED status indication
- 🔄 Automatic bed tilting using servo motor (optional)
- 📊 Remote monitoring dashboard

---

## 🛠 Hardware Components

| Component | Purpose |
|------------|---------|
| ESP32 | Main Controller |
| Force Sensitive Resistor (FSR) | Body Pressure Detection |
| DHT22 | Temperature & Humidity |
| OLED Display | Live Sensor Display |
| Servo Motor | Automatic Bed Tilt |
| Buzzer | Local Alert |
| LED | Status Indicator |
| Wi-Fi | Cloud Connectivity |
| Power Supply | System Power |

---

## 💻 Software & Technologies

- Arduino IDE
- ESP32 Framework
- Blynk IoT Platform
- Firebase (Optional)
- C++
- Embedded Systems
- IoT
- Wi-Fi Communication

---

## ⚙ How It Works

1. ESP32 continuously reads:
   - FSR pressure sensor
   - DHT22 temperature & humidity sensor

2. The system tracks how long the patient remains in the same position.

3. If the threshold is exceeded:

   - Buzzer turns ON
   - Mobile notification is sent
   - Sensor data is uploaded to the cloud
   - Servo motor gently tilts the bed (optional)

4. Once the patient's position changes, the timer resets automatically.

---

## 📂 Project Structure

```
Smart-Patient-Turning-System/
│
├── Arduino_Code/
│   ├── SmartPatientTurning.ino
│
├── Circuit_Diagram/
│
├── Images/
│
├── Documentation/
│   └── Project_Report.pdf
│
├── README.md
│
└── LICENSE
```

---

## 🔌 System Architecture

```
        +----------------------+
        |      FSR Sensor      |
        +----------+-----------+
                   |
                   |
        +----------v-----------+
        |       ESP32          |
        +----------+-----------+
                   |
      +------------+------------+
      |            |            |
      |            |            |
      v            v            v
  DHT22       Servo Motor    Buzzer
      |
      |
      v
  Wi-Fi Connection
      |
      v
 Blynk/Firebase Cloud
      |
      v
 Caregiver Mobile App
```

---

## 📱 Cloud Dashboard

The project supports remote monitoring through:

- Live Temperature
- Live Humidity
- FSR Values
- Patient Status
- Bed Position
- Alarm Status

using the **Blynk IoT Dashboard**.

---

## 🚀 Installation

### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/Smart-Patient-Turning-System.git
```

### 2. Open Arduino IDE

Install the following libraries:

- ESP32 Board Package
- DHT Sensor Library
- Adafruit GFX
- Adafruit SSD1306
- ESP32Servo
- Blynk Library

### 3. Configure Wi-Fi

```cpp
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";
```

### 4. Configure Blynk

```cpp
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""
```

### 5. Upload Code

Select the ESP32 board and upload the sketch.

---

## 📊 Future Improvements

- Multiple FSR sensors for full-body pressure mapping
- Machine Learning-based pressure ulcer prediction
- Hospital Management System integration
- Wearable vital-sign monitoring
- Battery backup system
- Mobile application development
- AI-powered caregiver recommendations

---

## 🎯 Applications

- Hospitals
- Nursing Homes
- Home Care
- Elderly Care
- Rehabilitation Centers
- ICU Monitoring

---

## 👨‍💻 Authors

- **Md. Khaled Mahmud**
- **Shah Sultan**
- **Abdullah Nazmus-Sakib**

Department of Computer Science and Engineering

Jahangirnagar University

---

## 📄 Project Report

The complete project report is available in the `Documentation` folder.

---

## 🤝 Contributing

Contributions are welcome!

1. Fork the repository
2. Create a new feature branch

```bash
git checkout -b feature-name
```

3. Commit your changes

```bash
git commit -m "Added new feature"
```

4. Push to GitHub

```bash
git push origin feature-name
```

5. Open a Pull Request

---

## ⭐ Support

If you found this project useful, please consider giving it a ⭐ on GitHub.

---

## 📜 License

This project is intended for educational and research purposes.

MIT License.
