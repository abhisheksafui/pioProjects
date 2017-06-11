


//HardwareTimer timer_2(2); 

void capture_freq()
{
  f = trigger_count;
  trigger_count=0;
  a=millis();
}

void setup_1sec_timer_interrupt()
{
  timer.pause();
  timer.setPrescaleFactor(7200);
  timer.setMode(1,TIMER_OUTPUT_COMPARE);
  timer.setCompare(1,10000); 
  timer.setOverflow(10000);
  timer.attachInterrupt(1,capture_freq);
  timer.resume();
}

