

void page3(void)
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.print("Cyc. Del:");
  display.println(triac1.getCycleDelay());
  display.setCursor(0,10);
  display.print("Cyc. Del:");
  display.println(triac1.getCycleDelay());
  display.display();
  
}
void page2(void)
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("FAN SPEED:");
  display.setCursor(34,25);
  display.setTextSize(2);
  display.println(triac1.getSpeed());
  display.display();
  
}
void page1(void)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(20,0);
  display.setTextColor(BLACK);
  display.println("AC Mains");
  display.setCursor(18,10);
  display.println("Frequency");
  display.setCursor(20,25);
  display.setTextSize(1);
  display.invertDisplay(true);
  display.println(f);

  display.setCursor(0,40);
  display.println(a);
  
  display.display();
  
}
