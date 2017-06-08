#ifndef MCUINERFACE_H
#define MCUINTERFACE_H


class ControlObject: public EspMcuInterface{
public:
  ControlObject(uint8_t code);
private:

}

enum class EspMcuCommand{
  DIMMER_SET,
  DIMMER_GET,
};

class EspMcuInterface{
public:
  send(const String s);
};

class Dimmer{
  uint8_t value;
  uint8_t max;
public:
  void setRangeMax(uint8_t m){
    Wire.write();
  }
  void set(uint8_t val);
  void get(uint8_t val);
};

#endif
