
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_PCD8544.h>

#include "triac.h"
#include "switch.h"
#include "setup.h"
#include "display.h"
#include "global.h"

#define SWITCH_UP_PIN 21
#define SWITCH_DOWN_PIN 12



HardwareTimer timer(1);
HardwareTimer timer_2(2);

Adafruit_PCD8544 display = Adafruit_PCD8544(6,4, 3, 7,27);

Display ds1(page1,2000);
Display ds2(page2,5000,100);
Display ds3(page3,5000,100);
Switch sw_up(SWITCH_UP_PIN,sw1PressCallback,ACTIVE_LOW,SW_POLL);
Switch sw_down(SWITCH_DOWN_PIN,sw2PressCallback,ACTIVE_LOW,SW_POLL);

Triac triac1(31,TRIAC_DIMMER,100);

volatile uint32_t freq_capture_ts = 0;

volatile float f=0,trigger_count=0;
uint8_t speed_set = 0;
uint8_t total_steps = MAX_STEPS; 
float step_size = 0;
uint32_t delay_count;
float T = 0;

volatile int a=0;

String command;





 


void trigger_triac()
{
  timer_2.pause();
  triac1.trigger();
  
  
}

void sw1PressCallback(){
  Serial.println("UP");
  triac1.setSpeed(triac1.getSpeed()+1);
  timer_2.setCompare(1,triac1.getCycleDelay());
  timer_2.setOverflow(triac1.getCycleDelay()+1);
  timer_2.refresh();
}


void sw2PressCallback(){
  Serial.println("UP");
  triac1.setSpeed(triac1.getSpeed()-1);
  timer_2.setCompare(1,triac1.getCycleDelay());
  timer_2.setOverflow(triac1.getCycleDelay()+1);
  timer_2.refresh();
  //Display::setIndex(2);
}






void setup() {
  // put your setup code here, to run once
  //Serial.end();
  Serial.begin(115200);
  Serial2.begin(115200);
  //Switch::init();
  
  //Switch *sw1 = new Switch(SWITCH_UP_PIN,sw1PressCallback,ACTIVE_LOW);

  
  setup_1sec_timer_interrupt();
 
  

#if 1
  /*Setup timer for triac trigger */
  /* initially trigger after 5us, which is almost = 0 */
  timer_2.pause();
  timer_2.setPrescaleFactor(72); /* increase every microsecond */
  timer_2.setMode(1,TIMER_OUTPUT_COMPARE);
  timer_2.setCompare(1,5000);
  timer_2.setOverflow(5001);
  timer_2.attachInterrupt(1,trigger_triac);
#endif
  
  display.begin();
  display.setContrast(27);
  display.display();
  //delay(1000);
  display.clearDisplay();

  //calculate_line_frequency(f);
  //T = 1/f;
  //step_size = T/total_steps; 
  
  trigger_count =0;
  freq_capture_ts = millis();
  pinMode(22,INPUT_PULLUP);
  attachInterrupt(22,extInt1,RISING);

}

void process_cmd(String& cmd){
  
  if(cmd.substring(0, cmd.indexOf(" ")) == "DIMMER")
  {
    Serial.println("Command for dimmer");
    cmd = cmd.substring(cmd.indexOf(" ")+1);
    if(cmd.substring(0, cmd.indexOf(" ")) == "SET" )
    {
      
      uint16_t val = cmd.substring(cmd.indexOf(" ")+1).toInt();
      Serial.println("Command SET Value = " + val);
      triac1.setSpeed(val);
      timer_2.setCompare(1,triac1.getCycleDelay());
      timer_2.setOverflow(triac1.getCycleDelay()+1);
      timer_2.refresh();
    }
  }
  
  
}
void loop() {


  //T=1/f;
  //step_size = T/100;
/* Frequency is given by f/2 = 50 MHZ  */
/* Timeperiod is given by 2/f = 20 ms  */
/* So timer can be set up to delay from 0 to 20ms in n steps*/   
  //T=(2/f)*1000000; //us
  //delay_count = floor((MAX_STEPS - speed_set)* (T/MAX_STEPS)); 
  //timer_2.setCompare(1,delay_count);
  //timer_2.setOverflow(1,
  
  

  Switch::loop();

  Display::update();
  

/*
  f=0;
  trigger_count=0;
  delay(1000);
  f = trigger_count/2.0;
*/
  
  #if 0
  Serial.print("f=");
  Serial.println(f);
  Serial.print("pages=");
  Serial.println(Display::count());
  Serial.println(trigger_count);
  Serial.print(Switch::switches_l.size());
  #endif
  if(Serial2.available()){
    while(Serial2.available()>0){
        command = Serial2.readStringUntil('\n');
    }

    process_cmd(command);
    Serial.print('D');
    Serial.print('o');
    Serial.print('\n');
  }
}


void extInt1()
{
  /* Delay based on value of fan speed */
  /* T = 1/F ; Divide it into 100 steps */ /* for 50 HZ
  /* S = T/100. U = user value = n */
  /* delay n*S */

    /* timer 2 has already been set up just fire it and stop it in timer interrupt*/
    if(triac1.getCycleDelay() !=0 )
    {
      timer_2.resume();
    }
    else
    {
      /* Speed set to maximum value */ 
      triac1.trigger();
      timer_2.resume(); 
    }

    trigger_count++;
  
    
 /* if(millis()-freq_capture_ts >= 1000)
  {
    
    f= trigger_count/(2*(millis()-freq_capture_ts))*1000;
    trigger_count = 0;
    freq_capture_ts = millis();
  }*/

}



