#include <WiFi.h>
#include <HardwareSerial.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define RX2 16
// Adafruit IO credentials
#define AIO_USERNAME  "xxxxxx"
#define AIO_KEY       "xxxxxxx"
#define AIO_SERVER    "io.adafruit.com"
#define AIO_SERVER_PORT 1883  // Non-secure port
#define AIO_LOG_FEED "xxxxxx/feeds/default.password-auth-feed"

// Wi-Fi credentials
const char* ssid = "xxxxx";
const char* password = "xxxxx";



// Set up the WiFi client and MQTT client
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVER_PORT, AIO_USERNAME, AIO_KEY);

// Set up the Adafruit IO feed
Adafruit_MQTT_Publish logFeed = Adafruit_MQTT_Publish(&mqtt, AIO_LOG_FEED);

// Serial port for STM32 logs
HardwareSerial SerialPort(2);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...\n");

  // WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi \n");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi\n");
  Serial.println(WiFi.localIP());
  Serial.println("\n"); 

  // Begin UART2 (receiving from STM32)
  SerialPort.begin(115200, SERIAL_8N1, RX2, -1);

  // MQTT Connect
  Serial.println("Connecting to Adafruit IO (MQTT)...");
  connectToMQTT();
}

void connectToMQTT() {
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    Serial.print("MQTT connect failed, error: ");
    Serial.println(mqtt.connectErrorString(ret));
    delay(5000);
  }
  Serial.println("MQTT Connected!");
}

void loop() {
  static String incomingMsg = "";
   if (!mqtt.connected()) {
    connectToMQTT();
  }
  // Check MQTT connection
  mqtt.processPackets(10);
  mqtt.ping();

  // Read UART from STM32
  while (SerialPort.available()) {
    char c = SerialPort.read();
    if (c == '\n') {
      incomingMsg.trim(); // Remove trailing \r or whitespace
      Serial.println("[STM32] " + incomingMsg);
      // Publish to Adafruit IO
      if (!logFeed.publish(incomingMsg.c_str())) {
        Serial.println("Failed to publish message");
      }

      incomingMsg = "";  // Reset for next message
    } else {
      incomingMsg += c;
    }
  }
}

