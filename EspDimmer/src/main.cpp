#include <Arduino.h>
#include<ESP8266WiFi.h>
#include<ESP8266mDNS.h>
#include <ESPAsyncUDP.h>
#include "init.hpp"
#include "LED.hpp"
#include <ArduinoOTA.h>
#include <SoftwareSerial.h>

#include "mcuInterface.h"

#include "global.h"

LED systemLed(2,ACTIVE_LOW);
SoftwareSerial swSer(5,4, false, 256);

int led_toggle_ts=0;
int dimmer_set_ts=0;
Dimmer dim("DIMMER",100);

void setup(){

  swSer.begin(115200);
  DEBUG_PORT.begin(115200);
  DEBUG_PORT.println("Esp8266 Booting..");
  //Wire.begin();

  spiffsInit();
  wifiInit();
  gpioInit();
  udpServerInit();
  otaSetup();
  DEBUG_PORT.println("Esp8266 Boot complete..");
}

void loop(){
  if(millis() - led_toggle_ts>200){
    led_toggle_ts = millis();
    systemLed.toggle();
  }

  if(millis() - dimmer_set_ts>50){
    dimmer_set_ts = millis();
    dim.increment();
  }



  #if 0
  Wire.beginTransmission(10);
  Wire.write(5);
  int rc = Wire.endTransmission();

  DEBUG_PORT.println("Wire Transmission result: " + rc);

  DEBUG_PORT.print("Reading from 10 1 byte = ");
  DEBUG_PORT.println(Wire.requestFrom(10,1));
#endif

  ArduinoOTA.handle();

}
