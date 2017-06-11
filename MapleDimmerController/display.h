

typedef void (*DISPLAY_FUNC)(void) ;

class Display{
  
  
  
  uint32_t _display_time;
  uint32_t _refresh_interval;
  uint32_t _refresh_ts;
  uint32_t _display_ts; 
  DISPLAY_FUNC df;

  public:
  static LinkedList<Display*> pages_list;
  static int current_page_index; 
  
  Display(void (*f)(void), uint32_t dt_ms=5000, uint32_t rfi_ms = 1000);
  static void update();
  static int count();
  static int index();
  
};



LinkedList<Display *> Display::pages_list;
int Display::current_page_index  = 0;

Display::Display(void (*f)(void), uint32_t dt, uint32_t rfi)
: df(f), _display_time(dt), _refresh_interval(rfi){
  
  _display_ts = 0;
  _refresh_ts = 0;
  pages_list.add(this);
}

void Display::update(void)
{
  if(pages_list.size() > 0)
  {
    Display *disp = pages_list.get(current_page_index);
    if(disp->_display_ts == 0)
    {
      disp->_display_ts = millis();
      disp->_refresh_ts = millis(); 
      disp->df();
    }
    else if((millis() - disp->_display_ts) >= disp->_display_time)
    {
      current_page_index = (current_page_index+1)%pages_list.size();
      disp->_display_ts = 0;
      disp->_refresh_ts = 0; 
    }else if((millis() - disp->_refresh_ts >= disp->_refresh_interval))
    {
      disp->_refresh_ts = millis(); 
      disp->df();
    }
  }
      
}

int Display::count(){
  return pages_list.size();
}
int Display::index(){
  return current_page_index;
}







