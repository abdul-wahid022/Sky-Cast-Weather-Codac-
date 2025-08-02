# ☁️ Sky Cast — Weather Forecast System in C++

**Sky Cast** is a sleek, object-oriented weather forecasting application developed in **C++**, offering real-time weather data, 5-day forecasts, temperature-based searches, and educational weather content specifically tailored for **Pakistani cities**. It utilizes the **OpenWeatherMap API**, integrates **RapidJSON** for parsing, and **libcurl** for API communication.

---

## 📌 Features

- 🔎 **City-wise Weather Search**  
  Real-time weather updates for major Pakistani cities (temperature, humidity, conditions).

- 🌦️ **5-Day Forecast**  
  Extended forecast with hourly breakdowns and daily summaries.

- 🌡️ **Temperature-Based Filtering**  
  Search cities with temperature near a given value (±1°C tolerance).

- ☁️ **Condition-Based Search**  
  Search cities based on weather conditions like "rain", "clouds", etc.

- 🕘 **Weather History**  
  View and analyze past weather records using file-based storage.

- 🚨 **Safety Alerts & Precautions**  
  View weather-related alerts and emergency preparation guidelines.

- 📚 **Educational Content**  
  Learn about weather phenomena, terminology, and fun meteorological facts.

---

## 🎯 Objectives

- Apply **OOP concepts** in a real-world application.
- Fetch and process live data using **external APIs**.
- Implement robust **error handling**, input validation, and file management.
- Build an informative, interactive console application for weather analysis.

---

## 🛠️ Tech Stack

| Component         | Technology Used          |
|------------------|--------------------------|
| Programming      | C++                      |
| API Integration  | OpenWeatherMap API       |
| HTTP Requests    | libcurl                  |
| JSON Parsing     | RapidJSON                |
| File Handling    | C++ fstream              |
| OS Compatibility | Windows, Linux           |

---

## ⚙️ Setup Instructions

### 1. 🔧 Install Dependencies

```bash
sudo apt update
sudo apt install g++ libcurl4-openssl-dev pkg-config rapidjson-dev git

### 2. 📦 Clone RapidJSON (if needed)

git clone https://github.com/Tencent/rapidjson.git

