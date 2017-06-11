#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <Arduino.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <global.h>

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
    DEBUG_PORT.println("Failed to open config file");
    return false;
  }else{
    DEBUG_PORT.println("Config file loaded.");
  }

  size_t size = configFile.size();
  DEBUG_PORT.print("Size of config file: "); DEBUG_PORT.println((int)size);
  if(size > 1024)
  {
    DEBUG_PORT.println("ERROR: Config file too large.");
    return false;
  }

  std::unique_ptr<char[]> buf(new char[size]{});
  configFile.readBytes(buf.get(),size);
  DynamicJsonBuffer j_buff;
  JsonObject &root= j_buff.parseObject(buf.get());

  if(!root.success()){
    DEBUG_PORT.println("ERROR parsing configuration.");
  }
  DEBUG_PORT.println("Configuration parsed sucessfully..");



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
    hostname = root["hostname"].asString();
  }

  dumpConfig();
  return true;

  }

  void dumpConfig(){
    DEBUG_PORT.println("======= CONFIGURATION =======");
    DEBUG_PORT.println("  mode : " + mode);
    DEBUG_PORT.println("  softApSSID: " + softApSSID);
    DEBUG_PORT.println("  softApPasswd: " + softApPasswd);
    DEBUG_PORT.println("  apSSID: " + apSSID);
    DEBUG_PORT.println("  apPasswd: " + apPasswd);
  }
};


#endif
