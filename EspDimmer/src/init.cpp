#include <Arduino.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include "config.hpp"
#include <ESPAsyncUDP.h>
#include <ArduinoOTA.h>

#include "global.h"

void gpioInit(){

}


#define CONFIG_FILE "/config.conf"
Config conf;
int critical_error;

WiFiEventHandler onConnectHandler;
WiFiEventHandler onDisconnectHandler;
bool disconnected = false;
void onDisconnected(const WiFiEventStationModeDisconnected& event)
{
  DEBUG_PORT.println("Wifi Disconnected from accesspoint " + event.ssid +
  ". Reason [" + event.reason +"].");
  //connectWifi(ssid,passwd);
  disconnected = true;
}

void onConnected(const WiFiEventStationModeConnected& event){
  disconnected = false;
  DEBUG_PORT.println("Wifi connected to accespoint.");
  DEBUG_PORT.println("  SSID: " + event.ssid);
  char bssid[100];
  sprintf(bssid,"%02x:%02x:%02x:%02x:%02x:%02x",event.bssid[0],
  event.bssid[1],
  event.bssid[2],
  event.bssid[3],
  event.bssid[4],
  event.bssid[5]);
  DEBUG_PORT.print("  BSSID: " ); DEBUG_PORT.println(bssid);
  DEBUG_PORT.println("  Channel: " + event.channel);
}

void connectWifi(String &ssid,String &passwd)
{
  DEBUG_PORT.println("Connecting to " + ssid);
  WiFi.begin(ssid.c_str(),passwd.c_str());

}
void wifiInit(){
  onDisconnectHandler = WiFi.onStationModeDisconnected(&onDisconnected);
  onConnectHandler = WiFi.onStationModeConnected(&onConnected);

  if(!conf.parse(CONFIG_FILE))
  {
    critical_error = true;
  }

  if(conf.mode == "STA")
  {
    WiFi.mode(WIFI_STA);
    WiFi.softAP(conf.softApSSID.c_str(),conf.softApPasswd.c_str());
    DEBUG_PORT.println("Soft AP started.");
    DEBUG_PORT.print(" SSID: " + conf.softApSSID + " Passwd: " + conf.softApPasswd);

  }else if(conf.mode == "AP"){
    WiFi.mode(WIFI_AP);
    connectWifi(conf.apSSID,conf.apPasswd);
  }else if(conf.mode == "STA_AP"){
    WiFi.mode(WIFI_AP_STA);
    connectWifi(conf.apSSID,conf.apPasswd);
    WiFi.softAP(conf.softApSSID.c_str(),conf.softApPasswd.c_str());
    DEBUG_PORT.println("Soft AP started.");
    DEBUG_PORT.print(" SSID: " + conf.softApSSID + " Passwd: " + conf.softApPasswd);
  }
}

void spiffsInit(){
  DEBUG_PORT.print("Mounting SPIFFS..");
  if (!SPIFFS.begin()) {
    DEBUG_PORT.println("[ERR]");
    return;
  }
  DEBUG_PORT.println("[OK]");
}
#define SERVER_LISTEN_PORT 1234
AsyncUDP udp;

void udpServerInit(){
  DEBUG_PORT.print("Starting UDP server on port " + WiFi.localIP());
  DEBUG_PORT.print(":"+ String(SERVER_LISTEN_PORT));
  DEBUG_PORT.print(" ");
  if(udp.listen(SERVER_LISTEN_PORT)){
    DEBUG_PORT.println("[OK]");
    udp.onPacket([](AsyncUDPPacket packet) {
            DEBUG_PORT.print("UDP Packet Type: ");
            DEBUG_PORT.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            DEBUG_PORT.print(", From: ");
            DEBUG_PORT.print(packet.remoteIP());
            DEBUG_PORT.print(":");
            DEBUG_PORT.print(packet.remotePort());
            DEBUG_PORT.print(", To: ");
            DEBUG_PORT.print(packet.localIP());
            DEBUG_PORT.print(":");
            DEBUG_PORT.print(packet.localPort());
            DEBUG_PORT.print(", Length: ");
            DEBUG_PORT.print(packet.length());
            DEBUG_PORT.print(", Data: ");
            DEBUG_PORT.write(packet.data(), packet.length());
            DEBUG_PORT.println();});
  }else{
    DEBUG_PORT.println("[ERR]");
  }
}

void otaSetup(){
  DEBUG_PORT.println("OTA hostname: "+ conf.hostname);
  ArduinoOTA.setHostname(conf.hostname.c_str());
  ArduinoOTA.begin();
}
