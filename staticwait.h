#ifndef staticwait_h
#define staticwait_h

#include "including.h"

extern void staticwait(int msec)
{
  static int timing = 0 ;

  int ticks = SDL_GetTicks() ;

  if(timing < ticks - msec)
  {
    timing = ticks;
    return;
  }

  // Don't try to make delays < 3 ms. keep them for the next time
  if(timing < ticks-msec+3)
  {
    timing += msec;
    return;
  }
  
  timing += msec;
  SDL_Delay( timing - ticks );

  return;
}

#endif // staticwait_h
