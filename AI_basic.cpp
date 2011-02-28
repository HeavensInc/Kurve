#include "AI_basic.h"
/*

class AI_basic : public AI_base
{
private:
  int   decision;

public: // Intelligence
  virtual void  calculate();
  virtual int   steer(); // +1  0 -1
}

*/
AI_basic::AI_basic()
{
  dc = new distcheck(9);
  decision = 0;
}

AI_basic::~AI_basic()
{
  delete dc;
}

int   AI_basic::steer()
{
//  std::cout << "AI going " << decision << std::endl ;
  return decision;
}

void  AI_basic::calculate()
{
  if(tob_current == NULL) return;

/*  
  static int toomuch = 0 ;
  toomuch++;
  if(toomuch < 2) return;
  toomuch = 0;
*/

//  std::cout << "AI DC set up" << std::endl ;
  dc->setup(tob_current);
  
  dc->calc_walls();

//  std::cout << "AI DC found walls" << std::endl ;


  for(int i=0;i<6;i++)
  {
    trailobj* tob = player_tstart[i] ;
    while( tob != NULL ) //&& tob != tob_current )
    {
//        std::cout << "AI DC calc tob" << std::endl ;
      dc->calc_tob(tob);
      tob = tob->next ;
    }
  }

  
  float decide[3] = { 0.0f , 0.0f , 0.0f};
  
  for(int i=0; i<9; i++)
  {
    int a;
    if(i < 4)
    {
      a = 0 ;
    }
    else
    if(i == 4)
    {
      a = 1 ;
    }    
    else
    {
      a = 2 ;
    }
    decide[a] += dc->get_part(i) ;
  }

  decide[1] *= 4 ;
  
  int randomchoice = get_random_choice();
  decide[1-randomchoice] *= 1.0005f;

//  std::cout << decide[0] << " "  << decide[1] << " " << decide[2] << std::endl;

//  printf("  %3f  %3f  %3f  \n" , decide[0] , decide[1] , decide[2] );
  decide[0] += ( 400 - decide[2] ) / 20.0f ;
  decide[2] += ( 400 - decide[0] ) / 20.0f ;

  
  if( decide[0] > decide[1] && decide[0] > decide[2] )
  {
    decision = -1 ;
    return ;
  }
  
  if(decide[2] > decide[1])
  {
    decision = +1 ;
  }else{
    decision =  0 ;
  }
    
  return;
}

