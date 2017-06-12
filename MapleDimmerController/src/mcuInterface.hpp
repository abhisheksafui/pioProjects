#ifndef MCUINTERFACE_HPP
#define MCUINTERFACE_HPP

#include <LinkedList.h>

class ESPMcuInterfaceObject;


class ESPMcuInterface{
  LinkedList<ESPMcuInterfaceObject *> objectList;
public:
  ESPMcuInterface(ESPMcuInterfaceObject *obj){
    objectList.add(obj);
  }

  static void init(){
      Serial.begin(115200);
  }

  void sendCommand(String s){
    Serial.println(s);
  }
};

class ESPMcuInterfaceObject: public ESPMcuInterface{
  uint8_t id;

public:
    /*Constructor*/
  ESPMcuInterfaceObject(int i):
   ESPMcuInterface(this),id(i){}


  void send(uint8_t command, String &arg)
  {
      char hdr[3]{0};
      hdr[0] = id;
      hdr[1] = command;
      String cmd(hdr);
      cmd += arg;
      sendCommand(cmd);
  }

  virtual void onReceive(String &command) = 0;

};


#endif
