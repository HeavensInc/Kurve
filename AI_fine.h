#ifndef AI_fine_h
#define AI_fine_h

#include <stdio.h>
#include "AI_base.h"

class AI_fine : public AI_base
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
  int split_into;
public: // Intelligence
  AI_fine();
  ~AI_fine();
  
  void  calculate();
  int   steer(); // +1  0 -1
}
;
#endif //AI_fine_h
