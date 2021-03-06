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
  dc->setup(tob_current, 100.0f * 2);
  
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
  
  float modifier = 1.0f;
  for(int i=0; i<9; i++)
  {
    int a;
    if(i < 3)
    {
      a = 0 ;
      modifier = 1.0f;
    }
    else
    if(i > 5)
    {
      a = 2 ;
      modifier = 1.0f;
    }    
    else
    {
      a = 1 ;
      modifier = 1.0f / 2;
    }
    decide[a] += dc->get_part(i) ;
  }

  decide[1] += 1 ;
  
//  std::cout << decide[0] << " "  << decide[1] << " " << decide[2] << std::endl;

//  printf("  %3f  %3f  %3f  \n" , decide[0] , decide[1] , decide[2] );
  float temp = ( 300 - decide[2] ) / 5.0f * (decide[0]/300);
  decide[2] += ( 300 - decide[0] ) / 5.0f * (decide[2]/300);
  decide[0] += temp;

  int randomchoice = 0 ;//get_random_choice();
//  if( randomchoice != 0 )
    decide[1-randomchoice] *= 1.001f;

  // close area decision
  bool this_was_close = false;
  if(dc->get_part(4) < 4 || dc->get_part(5) < 30 || dc->get_part(6) < 4 )// || decide[0] < 10 || decide[2] < 10)
  {
    this_was_close = true;
    decide[0] = dc->get_part(4) ;
    decide[1] = dc->get_part(5) ;
    decide[2] = dc->get_part(6) ;
  }


  if( decide[0] > decide[1] && decide[0] > decide[2])
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
  
  if( this_was_close )
  {
    if( (decide[0]+decide[2])*2.0f > decide[1])
    {
      decision = choose_something();
    }
    else if( decide[0] + 3 < decide[2] 
        || decide[0] > 3 + decide[2])
    {
      decision = ( decide[0] > decide[2] ? -1 : 1 ) ;
    }
  }
  
  return;
}

