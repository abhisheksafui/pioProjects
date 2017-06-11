
#include <LinkedList.h>
enum SWITCH_MODE{
  SW_INTERRUPT,
  SW_POLL,
};
enum SWITCH_ACTIVE_MODE{
  ACTIVE_LOW = 0,
  ACTIVE_HIGH = 1
};

class Switch {
   
private: 
  /*
   *  Check Funtion to check current status of the switch. Collect read information and mark switch flags
   *  and return. 
   *  Called from interrupt routine (for interrupt mode) or from switch loop (for polling mode)
   */
  void check();
  /*
   *  This function is called from within switch loop. 
   *  If this function returns true, that means press event is detected. 
   *  This function is responsible for debounce and long press events.
   */
  bool pressed();

  
  int read_pin;
  int mode;
  int active_state;
  int long_press_interval;
  int press_ts;
  int debounce_time_ms;
  
  volatile int toggle_flag; /* only for SW_INTERRUPT */
  bool long_press_start_flag; /* flag to indicate start of long press interval */

  void (*swPressCallback)(void);

  //static int switches_init;
  

public: 
  /* library init routine */
  
  static LinkedList<Switch*> switches_l;
  static void interrupt_routine();
  static void loop();
  

  public:
  //static void init();
  Switch(int pin,void (*swPressCallback)(),
                int act_s = ACTIVE_LOW,
                SWITCH_MODE m=SW_INTERRUPT, 
                int lpi=500);
  
  
};

/*void Switch::init()
{
  switches_l = LinkedList<Switch*>();
  switches_init = true;
  
}*/
LinkedList<Switch*> Switch::switches_l = LinkedList<Switch *>();
//int Switch::switches_init = false;

Switch::Switch(int pin,
                void (*callback)(),
                int act_s,
                SWITCH_MODE m, 
                int lpi):
read_pin(pin),active_state(act_s),mode(m),long_press_interval(lpi)
{
  //if(switches_init==false)
  // init();
  switches_l.add(this);
  pinMode(read_pin,active_state==ACTIVE_LOW?INPUT_PULLUP:INPUT);
  if(m==SW_INTERRUPT)
    attachInterrupt(read_pin,Switch::interrupt_routine,active_state==ACTIVE_LOW?FALLING:RISING);
  press_ts = 0; 
  debounce_time_ms = 50;
  swPressCallback = callback;
  toggle_flag= 0;
  long_press_start_flag = false;
}

void Switch::check()
{
  if(digitalRead(read_pin) == active_state)
  {
      toggle_flag = true;
      /* 
       *  The moment we see the switch is pressed (or atleast likely to be considered as start of debounce time),
       *  we mark the timestamp and set the flag. This initiates the debounce time checking in the next 
       *  switch loop call. 
       */
      press_ts = millis();
  }
}
/*
 *  All interrupt switches attach this function as ISR
 */
void Switch::interrupt_routine(){
  Switch *sw;
  for(int i=0; i<switches_l.size(); i++)
  {
     sw = switches_l.get(i);
     sw->check();
  }
}
/* To be called in loop */
bool Switch::pressed()
{
  if(mode == SW_INTERRUPT)
  {
    if(toggle_flag)
    {
      /* First time this will not be satisfied as long_press_start_flag not yet marked true */
      if( long_press_start_flag == true && millis()-press_ts > long_press_interval)
      {
       /* check if still pressed */
        if(digitalRead(read_pin)==active_state)
        {
          /*update timestamp and say yes*/
          press_ts = millis();
          long_press_start_flag = true;
          return true;
        }
        else
        {
          /* Not pressed so end this long press interval */
          press_ts = 0;
          toggle_flag=0;
          long_press_start_flag = false;
          return false; 
        }
      
      }
      else if( long_press_start_flag == false  && (millis()-press_ts) >= debounce_time_ms)
      {  
         if(digitalRead(read_pin)==active_state)
        {
          /*update timestamp and say yes*/
          press_ts = millis();
          long_press_start_flag = true;
          return true;
        }
        else
        {
          press_ts = 0;
          toggle_flag = 0;
          long_press_start_flag = false; 
          return false;
        }
      }  
    }
  }
  else if(mode == SW_POLL)
  {
    if(!press_ts && digitalRead(read_pin)==active_state)
    {
      press_ts = millis();
    }
    
    if(press_ts && (long_press_start_flag == true) && ((millis()-press_ts) >= long_press_interval))
    {
      if(digitalRead(read_pin)==active_state)
      {
        press_ts = millis();
        return true;  
      }
      else
      {
        long_press_start_flag = false;
        press_ts = 0;
        return false;
      }
    }
    else if (press_ts && (long_press_start_flag == false) && (millis()-press_ts >= debounce_time_ms))
    {
      if(digitalRead(read_pin)==active_state)
      {
        long_press_start_flag = true;
        press_ts = millis();
        return true;
      }
      else
      {
        press_ts = 0;
        long_press_start_flag = false;
        return false;
      }
    }
      
  }
  return false;
}

void Switch::loop()
{
  /*
   * To be called in a loop as frequently as possible, to reduce delay between press event and action
   */
  //Serial.println("loop");
  for(int i=0; i<switches_l.size(); i++)
  {
    Switch *sw = switches_l.get(i); 
    //Serial.println("SW1 in loop");
    if(sw->pressed())
    {
      Serial.println("Pressed");
      sw->swPressCallback();
    }
  }
}


