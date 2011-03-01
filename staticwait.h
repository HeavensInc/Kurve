#ifndef staticwait_h
#define staticwait_h

#include "including.h"

extern void staticwait(int msec)
{
  static Uint32 timing = 0 ;

  int ticks = SDL_GetTicks() ;

  if(timing < ticks - msec)
  {
    timing = ticks;
    return;
  }

  timing += msec;

  SDL_Delay( timing - ticks );

  return;
}

#endif // staticwait_h
