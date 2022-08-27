#include <Wire.h>
#include <ESP8266WiFi.h>
#include "MPUHeader.h"
#include "MQTTHeader.h"

char* screenID = "ScreenID";    // Your screenID (xrandr -q)

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
  delay(1000);

}

void loop() {
  if (!mqttClient.connected()) {
    mqttConnection();
  }

  readStatus();

  if (dataReady == 0x01) {
    readGyroZ();
    if (moved()){
      char command[45];
      strcpy(command, "-- output ");
      strcat(command, screenID);
      Serial.println(roll);
      if (isHorizontal){
        strcat(command, " --rotate right");
        mqttClient.publish(topic, command);
      } else {
        strcat(command, " --rotate left");
        mqttClient.publish(topic, command);
      }
    }
  }

  delay(1000);
  mqttClient.loop();

}
