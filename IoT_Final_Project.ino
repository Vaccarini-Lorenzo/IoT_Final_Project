#include <Wire.h>
#include <ESP8266WiFi.h>
#include "MPUHeader.h"
#include "MQTTHeader.h"

char* screenID = "";    // Your screenID (xrandr -q)

void setup() {
  Serial.begin(9600);
  delay(100);

  Wire.begin(sda, scl);
  delay(100);

  initMPU();
  delay(100);

  setNominalRoll();
  Serial.print("Nominal roll = ");
  Serial.println(nominalRoll);
  delay(100);

  Serial.println("Init Okay...");
  
  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  Serial.println("Wifi is off...");
  delay(1000);

}

void loop() {
  readStatus();
  if (dataReady == 0x01) {
    readGyroZ();
    if (moved()){
      WiFi.forceSleepWake();
      delay(10);
      WiFi.mode(WIFI_STA);
      Serial.println("Wifi is on...");
      wifiConnection();
      mqttConnection();
      char command[45];
      strcpy(command, "-- output ");
      strcat(command, screenID);
      Serial.print("The screen has been moved. Roll = ");
      Serial.println(roll);
      if (isHorizontal){
        Serial.println("Asking to rotate normal");
        strcat(command, " --rotate normal");
        mqttClient.publish(topic, command);
      } else {
        Serial.println("Asking to rotate right");
        strcat(command, " --rotate right");
        mqttClient.publish(topic, command);
      }
      delay(100);
      WiFi.mode( WIFI_OFF );
      WiFi.forceSleepBegin();
      Serial.println("Wifi is off...");
    }
  }
  delay(1000);
  mqttClient.loop();
}
