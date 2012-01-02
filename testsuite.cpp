#include <math.h>
#include <iostream>

#include <SDL/SDL.h>

#include "collision.h"


int main()
{
  
  std::cout<< " SDL_Delay (  0 ) " << std::endl;
  SDL_Delay ( 0 ) ;
  std::cout<< " SDL_Delays done " << std::endl;
        
  for(int s=0;s<20;s++)
  {
    int start = SDL_GetTicks();
    SDL_Delay(s);
    int stop = SDL_GetTicks();
    std::cout << s << " --> " << stop - start << std::endl;
  }

/*
  distcheck dc(30);
  trailobj tob[6];
  
  for(int i=0; i<5; i++)
  {
  tob[i].next = &tob[i+1];
  
  tob[i].x1 = 0 ;
  tob[i].x2 = 1 ;
  tob[i].y1 = i ;
  tob[i].y1 = i ;
  }

  tob[5].next = NULL;  
  tob[5].x1 = 0 ;
  tob[5].x2 = 1 ;
  tob[5].y1 = 5 ;
  tob[5].y1 = 5 ;

  dc.setup( &tob[5]);
  
  for(int i=0; i<5; i++)
  {
    dc.calc_tob( &tob[i]) ;
  }

  for(int i=0; i<30; i++)
  {
    std::cout << dc.get_part(i) << std::endl;
  }
*/
}
