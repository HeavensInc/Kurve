#include "AI_base.h"


AI_base::AI_base()
{
  for(int i=0;i<6;i++)
  {
    player_tstart[i]  = NULL  ;
    player_playing[i] = false ;
  }
  
  random_choice = rand() % 3 - 1 ;
  random_keep   = 5 ;
  
  random_do_choose = 0 ;        
  random_do_keep   = 0 ;

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
  if(random_keep < 1)
  {
    random_choice = rand() % 10 ;
    if(random_choice==0) random_choice = -1 ;
    else
    if(random_choice==1) random_choice = +1 ;
    else
        random_choice = 0 ;
        
    random_keep   = rand() % 10 + 10 ; // 4..6
  }
  
  random_do_keep--;
  random_keep--;
  return random_choice;
}

int   AI_base::choose_something()
{
  if(random_do_keep < 1)
  {
    random_do_choose = rand() % 2 ;
    if(random_do_choose==0) random_do_choose = -1 ;
    else
    if(random_do_choose==1) random_do_choose = +1 ;
    else
        random_do_choose = 0 ;
        
    random_do_keep   = 40 ;
  }
  
  random_do_keep--;
  return random_do_choose;
}
