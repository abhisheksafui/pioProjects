#ifndef LED_H
#define LED_H
#include<Arduino.h>
enum ACTIVE_MODE{
  ACTIVE_LOW=0,
  ACTIVE_HIGH=1
};
class LED{
  uint8_t _pin;
  uint8_t _activeMode;
  uint8_t _state;
public:
  LED(uint8_t pin, uint8_t activeMode):
  _pin(pin), _activeMode(activeMode){
    pinMode(_pin,OUTPUT);
    OFF();

  }
  void ON(){
    digitalWrite(_pin,
      _activeMode == ACTIVE_HIGH? HIGH: LOW);
      _state=HIGH;
  }
  void OFF(){
    digitalWrite(_pin,
      _activeMode == ACTIVE_HIGH? LOW: HIGH);
      _state = LOW;
  }
  void toggle(){
    if(_state==HIGH){
      OFF();
    }else{
      ON();
    }
  }
};

#endif
