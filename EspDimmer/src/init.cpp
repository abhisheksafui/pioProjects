#include <Arduino.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include "config.hpp"
#include <ESPAsyncUDP.h>
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
  Serial.println("Wifi Disconnected from accesspoint " + event.ssid +
  ". Reason [" + event.reason +"].");
  //connectWifi(ssid,passwd);
  disconnected = true;
}

void onConnected(const WiFiEventStationModeConnected& event){
  disconnected = false;
  Serial.println("Wifi connected to accespoint.");
  Serial.println("  SSID: " + event.ssid);
  char bssid[100];
  sprintf(bssid,"%02x:%02x:%02x:%02x:%02x:%02x",event.bssid[0],
  event.bssid[1],
  event.bssid[2],
  event.bssid[3],
  event.bssid[4],
  event.bssid[5]);
  Serial.print("  BSSID: " ); Serial.println(bssid);
  Serial.println("  Channel: " + event.channel);
}

void connectWifi(String &ssid,String &passwd)
{
  Serial.println("Connecting to " + ssid);
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
    Serial.println("Soft AP started.");
    Serial.print(" SSID: " + conf.softApSSID + " Passwd: " + conf.softApPasswd);

  }else if(conf.mode == "AP"){
    WiFi.mode(WIFI_AP);
    connectWifi(conf.apSSID,conf.apPasswd);
  }else if(conf.mode == "STA_AP"){
    WiFi.mode(WIFI_AP_STA);
    connectWifi(conf.apSSID,conf.apPasswd);
    WiFi.softAP(conf.softApSSID.c_str(),conf.softApPasswd.c_str());
    Serial.println("Soft AP started.");
    Serial.print(" SSID: " + conf.softApSSID + " Passwd: " + conf.softApPasswd);
  }
}

void spiffsInit(){
  Serial.print("Mounting SPIFFS..");
  if (!SPIFFS.begin()) {
    Serial.println("[ERR]");
    return;
  }
  Serial.println("[OK]");
}
#define SERVER_LISTEN_PORT 1234
AsyncUDP udp;

void udpServerInit(){
  Serial.print("Starting UDP server on port " + WiFi.localIP());
  Serial.print(":"+ String(SERVER_LISTEN_PORT));
  Serial.print(" ");
  if(udp.listen(SERVER_LISTEN_PORT)){
    Serial.println("[OK]");
    udp.onPacket([](AsyncUDPPacket packet) {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();});
  }else{
    Serial.println("[ERR]");
  }
}
