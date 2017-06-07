#include <FS.h>
#include <ESP8266WiFi.h>
#include "config.hpp"

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
