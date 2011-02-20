#ifndef staticwait_h
#define staticwait_h

#include "including.h"

extern void staticwait(int msec)
{
  static Uint32 timing = 0 ;

  if(timing < SDL_GetTicks() - msec)
  {
    timing = SDL_GetTicks();
    return;
  }

  timing += msec;

  SDL_Delay( timing -  SDL_GetTicks() );

  return;
}

#endif // staticwait_h
