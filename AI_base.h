#ifndef AI_base_h
#define AI_base_h

#include <time.h>
#include <cstdlib>

#include "collision.h"


class AI_base
{
protected: // Stuff all AI need
  trailobj* tob_current       ;
  trailobj* player_tstart[6]  ;
  bool      player_playing[6] ;
  
public: // Basic Funtions
  AI_base();
  void    patch_pointers(trailobj* pointers[6],trailobj* current);
  int     get_random_choice();

public: // Intelligence
  virtual void  calculate(){};
  virtual int   steer(){return 0;} // +1  0 -1
}
;

#endif //AI_base_h
