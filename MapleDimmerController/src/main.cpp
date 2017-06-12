#include <Arduino.h>
#include <Wire.h>
#include "mcuInterface.hpp"

class Dimmer: public ESPMcuInterfaceObject{
  int value;
  int maxRange;
public:
  Dimmer(int i, int v, int m)
  : ESPMcuInterfaceObject(i), value(v), maxRange(m){}

  void setValue(int value){ this->value = value; }
  void setMax(int max) {this->maxRange = max;}
  int getValue(){    return value;  }
  int getMax(){ return maxRange; }

  void onReceive(String &command){

  }
};

Dimmer d(1,70,100);

void setup(){

}

void loop(){

}
