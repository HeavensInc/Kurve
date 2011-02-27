#include <math.h>
#include <iostream>

#include "collision.h"
int main()
{
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

}
