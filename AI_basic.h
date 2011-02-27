#ifndef AI_basic_h
#define AI_basic_h

#include <stdio.h>
#include "AI_base.h"

#define AI_BASIC_RADIUS 200

class AI_basic : public AI_base
{
/*
private: // Stuff all AI need
  trailobj* player_tstart[6]  ;
  bool      player_playing[6] ;
  
public: // Basic Funtions
  AI_base();
  void    patch_pointers(trailobj* pointers[6]);
  int     get_random_choice();
*/

private:
  int   decision;
  distcheck* dc;
  
public: // Intelligence
  AI_basic();
  ~AI_basic();
  
  void  calculate();
  int   steer(); // +1  0 -1
  
}
;
#endif //AI_basic_h
