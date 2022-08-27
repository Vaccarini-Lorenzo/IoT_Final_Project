#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient wiFiClient;
PubSubClient mqttClient(wiFiClient);
const char* ssid     = "";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "";
const char* broker = "test.mosquitto.org";
const char* topic = "ESP8266/test";
const int port = 1883;


void wifiConnection();
void mqttConnection();


void wifiConnection(){
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  while (WiFi.status() != WL_CONNECTED){
      delay(1000);
  }

  Serial.println("Connected");
  mqttClient.setServer(broker, port);
}

void mqttConnection() {
  delay(100);
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random mqttClient ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
      mqttClient.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
