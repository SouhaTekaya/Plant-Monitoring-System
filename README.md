# Plant-Monitoring-System
This project is a real-time plant environment monitoring system built using an ESP32, MQTT protocol, and Node-RED dashboard. It collects environmental data like temperature, humidity, soil moisture, light intensity, gas levels, and motion detection, then publishes them to MQTT topics and optionally triggers automated responses (like turning on a water pump or air extractor).

How It Works :
ESP32 connects to Wi-Fi and the MQTT broker (broker.hivemq.com).
Every hour, it reads data from all sensors.
It publishes the data as MQTT messages on the specified topics.
Based on conditions ( dangerous gas or dry soil), it can trigger actions like activating a water pump or air extractor.
It goes into deep sleep for 1 hour to save power.

🛠️ Setup Instructions
1. Flash the ESP32
Open the code in Arduino IDE.
=====
Install required libraries:
Adafruit Unified Sensor
DHT sensor library
WiFi
PubSubClient
======
Configure your Wi-Fi credentials in:
const char* ssid = "your-SSID";
const char* password = "your-PASSWORD";


3. Set up MQTT Broker
You can use a public broker like HiveMQ (already set in the code), or install your own Mosquitto).

4. Create Node-RED Dashboard
Install Node-RED on your local machine or server.
Use the MQTT-in nodes to subscribe to each topic.
Visualize data using gauges, charts, and text.


