Final code
#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Defining Pins
#define DHTPIN 2
#define GAS_SENSOR_PIN A0
#define LIGHT_SENSOR_PIN A1
#define SOIL_SENSOR_PIN A2
#define MOTION_SENSOR_PIN A3
#define PUMP_PIN 15
#define EXTRACTOR_PIN 16

// DHT parameters
#define DHTTYPE    DHT22     // DHT 22
DHT_Unified dht(DHTPIN, DHTTYPE);

// MQTT Credentials
const char* ssid = "Wokwi-GUEST"; // Setting your AP SSID
const char* password = ""; // Setting your AP PSK
const char* mqttServer = "broker.hivemq.com";
const char* clientID = "ujaisldaalkjlasdfgh;laslksdja1"; 
const char* topic = "sensor/dht22"; // Publish topic
const char* gasTopic = "sensor/gas"; // Gas sensor topic
const char* lightTopic = "sensor/light"; // Light sensor topic
const char* soilTopic = "sensor/soil"; // Soil sensor topic
const char* pumpTopic = "sensor/pump"; // Water pump topic
const char* motionTopic = "sensor/motion"; // Motion sensor topic
const char* extractorTopic = "sensor/extractor"; // Air extractor topic

// Parameters for using non-blocking delay
unsigned long previousMillis = 0;
const long interval = 3600000; // 1 hour in milliseconds
String msgStr = "";      // MQTT message buffer
float temp, hum;
int gasValue, lightValue, soilValue, motionValue;
bool pumpStatus = false;
bool extractorStatus = false;

// Setting up WiFi and MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect(clientID)) {
      Serial.println("MQTT connected");
      client.subscribe(topic);
      Serial.println("Topic Subscribed");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);  // wait 5sec and retry
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle MQTT messages if needed
}

void setup() {
  Serial.begin(115200);
  pinMode(GAS_SENSOR_PIN, INPUT); // Initialize gas sensor pin
  pinMode(LIGHT_SENSOR_PIN, INPUT); // Initialize light sensor pin
  pinMode(SOIL_SENSOR_PIN, INPUT); // Initialize soil sensor pin
  pinMode(MOTION_SENSOR_PIN, INPUT); // Initialize motion sensor pin
  pinMode(PUMP_PIN, OUTPUT); // Initialize water pump pin
  pinMode(EXTRACTOR_PIN, OUTPUT); // Initialize air extractor pin
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);

  // Set up deep sleep
  esp_sleep_enable_timer_wakeup(3600000); // set timer to wake up
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // Read gas sensor value
    gasValue = analogRead(GAS_SENSOR_PIN);
    // Publish gas sensor data
    if (gasValue > 1000) {
      client.publish(gasTopic, "Attention sir!! Dangerous gas next to your plant, open the air extractor.");
    } else {
      client.publish(gasTopic, "There is no dangerous gas.");
    }
    // Read light sensor value
    lightValue = analogRead(LIGHT_SENSOR_PIN);
    // Publish light sensor data
    msgStr = String(lightValue);
    client.publish(lightTopic, msgStr.c_str());
    Serial.print("Light Intensity: ");
    Serial.println(lightValue);
    // Read soil sensor value
    soilValue = analogRead(SOIL_SENSOR_PIN);
    // Publish soil sensor data
    if (soilValue < 1000) {
      client.publish(soilTopic, "Soil is dry.");
    } else {
      client.publish(soilTopic, "Soil is moist.");
    }
    // Read motion sensor value
    motionValue = analogRead(MOTION_SENSOR_PIN);
    // Publish motion sensor data
    if (motionValue == 1) {
      client.publish(motionTopic, "Motion detected.");
    } else {
      client.publish(motionTopic, "No motion detected.");
    }
    // Control water pump based on received MQTT message
    if (pumpStatus) {
      digitalWrite(PUMP_PIN, HIGH);
    } else {
      digitalWrite(PUMP_PIN, LOW);
    }
    // Control air extractor based on received MQTT message
    if (extractorStatus) {
      digitalWrite(EXTRACTOR_PIN, HIGH);
    } else {
      digitalWrite(EXTRACTOR_PIN, LOW);
    }
    // Deep sleep
    Serial.println("Going to sleep now");
    esp_deep_sleep_start();
  }
}
