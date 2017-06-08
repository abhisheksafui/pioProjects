#include <Arduino.h>
#include<ESP8266WiFi.h>
#include<ESP8266mDNS.h>
#include <ESPAsyncUDP.h>
#include "init.hpp"
#include "LED.hpp"
#include <Wire.h>

LED systemLed(2,ACTIVE_LOW);

void setup(){

  Serial.begin(9600);
  Serial.println("Esp8266 Booting..");
  Wire.begin();
  spiffsInit();
  wifiInit();
  gpioInit();
  udpServerInit();
  Serial.println("Esp8266 Boot complete..");
}

void loop(){
  systemLed.toggle();
  delay(2000);
}
