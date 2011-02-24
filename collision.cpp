#include "collision.h"

  /*
   
      class quad
      {
      private:
        float points[4][2];
        float vector[4][2];
      public:
        quad( float[4][2] ) ;
        quad(float x1, float x2, float x3, float x4,
             float y1, float y2, float y3, float y4);
             
        bool check_point(float x, float y);
      }

   */

quadcheck::quadcheck( float points_in[4][2] )
{
  for(int i = 0 ; i < 4 ; i++)
    for(int j = 0 ; j < 2 ; j++)
      points[i][j] =  points_in[i][j] ;
  
  for(int i = 0; i < 2 ; i++)
  {
    vector[0][i] = points[1][i] - points[0][i] ;
    vector[1][i] = points[2][i] - points[1][i] ;
    vector[2][i] = points[3][i] - points[2][i] ;
    vector[3][i] = points[0][i] - points[3][i] ; 
  }
}

quadcheck::quadcheck( trailobj* tob )
{
  points[0][0] = tob->x1;
  points[1][0] = tob->prev->x1;
  points[2][0] = tob->prev->x2;
  points[3][0] = tob->x2;
  
  points[0][1] = tob->y1;
  points[1][1] = tob->prev->y1;
  points[2][1] = tob->prev->y2;
  points[3][1] = tob->y2;
  
  for(int i = 0; i < 2 ; i++)
  {
    vector[0][i] = points[1][i] - points[0][i] ;
    vector[1][i] = points[2][i] - points[1][i] ;
    vector[2][i] = points[3][i] - points[2][i] ;
    vector[3][i] = points[0][i] - points[3][i] ; 
  }
}



bool quadcheck::check_point(float x, float y)
{
  float det;
  for(int i = 0 ; i < 4 ; i++)
  {
    det = vector[i][1] * ( x - points[i][0]) - vector[i][0] * ( y - points[i][1]);
        
    if(det < 0) return false;
  }
  std::cout << "HIT" ;
  return true;
}

/*
    class trailcheck
    {
    private:
      bool setup;
      trailobj* last;
      float x,y;
    public:
      void  point(float new_x, float new_y);
      void  reset(trailobj tob);
      bool  check(trailobj tob);
    }
*/


void  trailcheck::point(float new_x, float new_y)
{
  x = new_x ;
  y = new_y ;
  setup = true;
}

bool  trailcheck::check(trailobj* tob)
{
  if(!setup) return false;

  float det;

  det = (tob->prev->x1 - tob->x1) * ( y - tob->y1) - (tob->prev->y1 - tob->y1) * ( x - tob->x1);
  if(det > 0) return false;

  det = (tob->x2 - tob->prev->x2) * ( y - tob->prev->y2) - (tob->y2 - tob->prev->y2) * ( x - tob->prev->x2);
  if(det > 0) return false;
  
  det = (tob->prev->x2 - tob->prev->x1) * ( y - tob->prev->y1) - (tob->prev->y2 - tob->prev->y1) * ( x - tob->prev->x1);
  if(det > 0) return false;

  det = (tob->x1 - tob->x2) * ( y - tob->y2) - (tob->y1 - tob->y2) * ( x - tob->x2);
  if(det > 0) return false;

  return true;
}


