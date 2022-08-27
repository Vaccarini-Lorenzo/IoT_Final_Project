#include <Wire.h>
#include <ESP8266WiFi.h>
#include "MPUHeader.h"
#include "MQTTHeader.h"


void setup() {
  Serial.begin(9600);
  delay(100);
  wifiConnection();
  delay(100);
  Wire.begin(sda, scl);
  delay(100);
  initMPU();
  delay(100);
  setNominalRoll();
  delay(100);
  Serial.println("Init Okay...");
  delay(2000);

}

void loop() {
  if (!mqttClient.connected()) {
    mqttConnection();
  }
  
  readStatus();
  mqttClient.publish(topic, "TEST");
  
  if (dataReady == 0x01) {
    readGyroZ();
    if (moved()){
      Serial.println(roll);
      if (isHorizontal){
        mqttClient.publish(topic, "Horizontal");
      } else {
        mqttClient.publish(topic, "Vertical");
      }
    }
  }
  
  delay(1000);
  mqttClient.loop();

}
