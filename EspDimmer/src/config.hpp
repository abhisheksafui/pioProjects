#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <Arduino.h>
#include <FS.h>
#include <ArduinoJson.h>

enum WifiMode{
  STA,
  AP,
  STA_AP
};

struct Config{
  String softApSSID;
  String softApPasswd;
  String hostname;
  String apSSID;
  String apPasswd;
  String mode;

  bool parse(const char *file){
    File configFile = SPIFFS.open(file, "r");

  if(!configFile){
    Serial.println("Failed to open config file");
    return false;
  }else{
    Serial.println("Config file loaded.");
  }

  size_t size = configFile.size();
  Serial.print("Size of config file: "); Serial.println((int)size);
  if(size > 1024)
  {
    Serial.println("ERROR: Config file too large.");
    return false;
  }

  std::unique_ptr<char[]> buf(new char[size]{});
  configFile.readBytes(buf.get(),size);
  DynamicJsonBuffer j_buff;
  JsonObject &root= j_buff.parseObject(buf.get());

  if(!root.success()){
    Serial.println("ERROR parsing configuration.");
  }
  Serial.println("Configuration parsed sucessfully..");



  mode = root["mode"].asString();
  if(mode == "STA"){
    softApSSID = root["softApSSID"].asString();
    softApPasswd = root["softApPasswd"].asString();
  }else if(mode == "AP"){
    apSSID = root["apSSID"].asString();
    apPasswd = root["apPasswd"].asString();
  }else if(mode == "STA_AP"){
    softApSSID = root["softApSSID"].asString();
    softApPasswd = root["softApPasswd"].asString();
    apSSID = root["apSSID"].asString();
    apPasswd = root["apPasswd"].asString();
  }

  dumpConfig();
  return true;

  }

  void dumpConfig(){
    Serial.println("======= CONFIGURATION =======");
    Serial.println("  mode : " + mode);
    Serial.println("  softApSSID: " + softApSSID);
    Serial.println("  softApPasswd: " + softApPasswd);
    Serial.println("  apSSID: " + apSSID);
    Serial.println("  apPasswd: " + apPasswd);
  }
};


#endif
