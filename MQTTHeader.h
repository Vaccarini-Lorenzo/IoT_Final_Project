#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient wiFiClient;
PubSubClient mqttClient(wiFiClient);
const char* ssid     = "";                  // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "";                  // The Wi-Fi newtwork password
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
    if (!mqttClient.connected()) {
      delay(100);
      while (!mqttClient.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Init random mqttClient ID
        String clientId = "ClientID";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (mqttClient.connect(clientId.c_str())) {
          Serial.println("Connected");
          mqttClient.subscribe(topic);
        } else {
          Serial.print("Failed connection. Error Code: ");
          Serial.print(mqttClient.state());
          Serial.println("Trying again in 3 sec...");
          delay(3000);
          }
      }
  }
}
