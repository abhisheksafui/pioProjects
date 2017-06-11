#ifndef MCUINERFACE_H
#define MCUINTERFACE_H
#include <Wire.h>
#if 0
class EspMcuInterface{

  public:
     EspMcuInterface();
     static init(){
       Wire.begin();
     }
     static sendCommand(){

     }
}
class ControlObject{

public:
  ControlObject(uint8_t addr, uint8_t code);
  sendCommand(uint8_t command,char *buff,int16_t len){

  }
private:
  uint8_t _code;
}

#endif

#include "global.h"

#define INTERFACE swSer

class Dimmer{
  const char * name;
  uint16_t value;
  uint16_t max;
public:
  Dimmer(const char *name,uint16_t max=20){
    this->name = name;
    this->max = max;
    this->value= max/2;
  }
  void setRangeMax(uint16_t m){
    INTERFACE.print(name);
    INTERFACE.println(" RANGE " + m);
  }
  void set(uint16_t val){
    INTERFACE.println(name + String(" SET ") + val);
    value = val;
  }
  uint16_t get(){
    return value;
  }
  void increment(){
    value++;
    if(value>max)
      value=0;
    set(value);
  }
};
#endif
