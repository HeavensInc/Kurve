#include "AI_base.h"


AI_base::AI_base()
{
  for(int i=0;i<6;i++)
  {
    player_tstart[i]  = NULL  ;
    player_playing[i] = false ;
  }
}

void  AI_base::patch_pointers(trailobj* pointers[6] , trailobj* current)
{
  for(int i=0;i<6;i++)
  {
    player_tstart[i] = pointers[i];
    if(pointers[i] == NULL)
    {
      player_playing[i] = false;
    }else{
      player_playing[i] = true;
    }
  }
  
  tob_current = current;
}

int   AI_base::get_random_choice()
{
  static int choice = rand() % 3 - 1 ;
  static int keep   = 5 ;
  
  if(keep < 1)
  {
    choice = rand() % 3 - 1 ;
    keep   = rand() % 3 + 4 ; // 4..6
  }
  
  keep--;
  return choice;
}
