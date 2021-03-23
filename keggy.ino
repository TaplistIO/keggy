/**
 * Configuration Section
 *
 * Update the values below prior to flashing. You shouldn't _need_ to
 * change anything else in this file.
 */

#define WIFI_SSID "MyWifi"
#define WIFI_PASSWORD "password"
#define MQTT_IP "192.168.1.2"
#define MQTT_TOPIC_NAME "keggy/ticks"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "keggy"
#define FLOW_METER_PIN 26
#define UPDATE_INTERVAL 1000

/*
 * Main program
 */

#include <M5StickC.h>
#include <MQTT.h>
#include <WiFi.h>

#define KEGGY_VERSION "0.0.1"

volatile unsigned long totalTicks = 0;
unsigned long lastMeterReportTime = 0;
unsigned long lastMeterReportValue = 0;
WiFiClient wifiClient;
MQTTClient mqttClient;

void connectToWifi() {
  Serial.println("Connecting to wifi ...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("WIFI connected, IP: ");
  Serial.println(WiFi.localIP());
}

void serviceWifi() {
  switch (WiFi.status()) {
    case WL_CONNECT_FAILED:
    case WL_CONNECTION_LOST:
    case WL_DISCONNECTED:
      connectToWifi();
      break;
  }
}

void serviceMQTT() {
  if (!mqttClient.connected()) {
    connectToMQTT();
  }
}

void connectToMQTT() {
  Serial.println("Connecting to MQTT ...");
  while (!mqttClient.connect(MQTT_CLIENT_ID)) {
    delay(500);
  }
  Serial.println("MQTT connected!");
}

void publishUpdates() {
  if (lastMeterReportValue == totalTicks) {
    return;
  }
  if ((millis() - lastMeterReportTime) < UPDATE_INTERVAL) {
    return;
  }
  lastMeterReportValue = totalTicks;
  lastMeterReportTime = millis();
  Serial.print("Publishing update: ");
  Serial.print(lastMeterReportValue);
  Serial.println();
  mqttClient.publish(MQTT_TOPIC_NAME, String(lastMeterReportValue));
}

void meterInterrupt() { totalTicks += 1; }

void onMQTTMessageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload + "\n");
}

void setup() {
  Serial.begin(115200);

  Serial.println("Initializing stick ...");
  M5.begin();
  M5.Lcd.setTextColor(RED, WHITE);
  M5.Lcd.println("Keggy!");

  pinMode(FLOW_METER_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_METER_PIN), meterInterrupt,
                  FALLING);

  Serial.println("Boot delay ...");
  delay(5000);

  Serial.println("Connecting to wifi and MQTT ...");
  connectToWifi();
  mqttClient.begin(MQTT_IP, wifiClient);
  mqttClient.onMessage(onMQTTMessageReceived);
  connectToMQTT();
}

void loop() {
  serviceWifi();
  serviceWifi();
  publishUpdates();
  delay(1000);
}
