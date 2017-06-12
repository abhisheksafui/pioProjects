
#define MAX_STEPS 100

enum TRIAC_MODE{
    TRIAC_ON_OFF,
    TRIAC_DIMMER
};

class Triac {
  private:
   int trigger_pin;
   TRIAC_MODE mode;
   /* IF DIMMER */
   uint32_t max_steps; 
   uint8_t speed_set;
   uint32_t cycle_delay_us;
   float freq;
   float time_period_us;
    
  public:
  Triac(int,TRIAC_MODE m=TRIAC_DIMMER, uint32_t max_st=10);
  void trigger();
  
  void setSpeed(uint8_t s);
  uint8_t getSpeed();

  void setAcFrequency(float f);
  uint32_t getCycleDelay(void);

};

Triac::Triac(int pin, TRIAC_MODE m, uint32_t max_st): mode(m), trigger_pin(pin), max_steps(max_st)
{
  trigger_pin= pin;
  pinMode(pin,OUTPUT);
  digitalWrite(pin,HIGH);
  speed_set = max_steps; 
  freq = 50;
  time_period_us = 1/freq * 1000000/2; // 20 * 1000
  cycle_delay_us = time_period_us - speed_set*(time_period_us/max_steps);
}
void Triac::trigger()
{
  if(speed_set == 0)
    return; 
    
  digitalWrite(trigger_pin,HIGH);
  delayMicroseconds(100);
  digitalWrite(trigger_pin,LOW);
}

void Triac::setSpeed(uint8_t speed)
{
  if(speed < 0 || speed > max_steps)
    return;
     
  speed_set = speed;
  cycle_delay_us = time_period_us - speed_set*(time_period_us/max_steps);

  /*Minimum 400 us delay needed to turn on the triac*/
  /* This occurs only at extreme high end of the value (approx 97%)*/
  if(cycle_delay_us <= 400){cycle_delay_us = 400;}
  
}

uint8_t Triac::getSpeed(void)
{
  return speed_set;
}

void Triac::setAcFrequency(float f)
{
  freq = f;
  time_period_us = 1/f;
  cycle_delay_us = time_period_us - speed_set*(time_period_us/max_steps);
}

uint32_t Triac::getCycleDelay(void)
{
  return (cycle_delay_us);
}

