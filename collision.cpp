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

/*
      class distcheck
      {
      private:
        float  x,y;
        float  vector_x, vector_y;
        float  direction;
        int    parts;
        float* min_dist;
        
        float get_direction(float dx, float dy);
      public:
        distcheck(int parts, trailobj*);
        ~distcheck();
        
        void   calc_walls();
        void   calc_tob(trailobj* tob);
        
        int   get_parts(){ return subparts; }
        int   get_part(int dir);
      };
*/

distcheck::distcheck(int new_parts)
{
  parts = new_parts;
  
  min_dist = new float [ new_parts ] ;
  if(min_dist == NULL)
  {
    std::cout << "could not take\n" ;

    min_dist = new float [ new_parts ] ;
    if(min_dist == NULL)
    {
      std::cout << "failed twice" << std::endl ;
    }
  }
}

void  distcheck::setup(trailobj* tob)
{
  for(int i = 0;i<parts;i++)
    min_dist[i] = 100;

  x = (tob->x1 + tob->x2) / 2.0f ;
  y = (tob->y1 + tob->y2) / 2.0f ;
  
  vector_x =  (tob->y2 - tob->y1); //  x =  y
  vector_y = -(tob->x2 - tob->x1); //  y = -x
  
  x += vector_x;
  y += vector_y;
  
  direction = atan2(vector_y, vector_x) ;
  
//  std::cout << direction << std::endl;
}

distcheck::~distcheck()
{
  delete [] min_dist ;
}

float distcheck::get_direction(float dx, float dy)
{
  float dir1 = atan2(dy,dx);
  dir1 = dir1 - direction ;
  
  dir1  = (dir1 >  M_PI ? dir1 - 2*M_PI : dir1 ) ;
  dir1  = (dir1 < -M_PI ? dir1 + 2*M_PI : dir1 ) ;
  return dir1;
}

void distcheck::calc_walls()
{
  float dist[ parts+1 ];
  for(int i=0;i<parts+1;i++)
  {
    dist[i] = 100.0f; //maxdist
    
    float vecx = cos( direction + M_PI * ( +0.5f +1.0f * i / parts ) );
    float vecy = sin( direction + M_PI * ( +0.5f +1.0f * i / parts ) );

    if(vecy != 0.0f)
    {
       float t_dist ;

       t_dist = y / vecy ;
       if(t_dist < dist[i] && t_dist > 0.0f)
          dist[i] = t_dist;

       t_dist = ( y - global.gl_height + DEF_SCORES ) / vecy ;
       if(t_dist < dist[i] && t_dist > 0.0f)
          dist[i] = t_dist;
          
    }
    
    if(vecx != 0.0f)
    {
       float t_dist = x / vecx ;
       if(t_dist < dist[i] && t_dist > 0.0f)
          dist[i] = t_dist;
          
       t_dist = ( x - global.gl_width ) / vecx ;
       if(t_dist < dist[i] && t_dist > 0.0f)
          dist[i] = t_dist;

    }
  }

  for(int i=0;i<parts;i++)
  {
    min_dist[i] = (min_dist[i] < dist[ i ] ? min_dist[i] : dist[ i ]) ;
    min_dist[i] = (min_dist[i] < dist[i+1] ? min_dist[i] : dist[i+1]) ;
  }
}

void distcheck::calc_tob(trailobj* tob)
{
  if(tob->type > 0) return;

  float dir;
  float t_dist;

  
  float vecx = tob->x1 - x ;
  float vecy = tob->y1 - y ;

  dir = get_direction( vecx , vecy );
  t_dist = sqrt( vecx*vecx + vecy*vecy ) * (tob->status > 25? 0.25f : 1.0f);

  if(dir > -0.5f*M_PI && dir < 0.5f*M_PI)
  {
    int t_part = parts / 2.0f + dir * parts / M_PI ;
    if(t_part >= parts) t_part = parts-1;
       
    min_dist[t_part] = (min_dist[t_part] < t_dist ? min_dist[t_part] : t_dist) ;
  }


  vecx = tob->x2 - x ;
  vecy = tob->y2 - y ;

  dir = get_direction( vecx , vecy );
  t_dist = sqrt( vecx*vecx + vecy*vecy ) * (tob->status > 25? 0.25f : 1.0f);

  if(dir > -0.5f*M_PI && dir < 0.5f*M_PI)
  {
    int t_part = parts / 2.0f + dir * parts / M_PI ;
    if(t_part >= parts) t_part = parts-1;
    
    min_dist[t_part] = (min_dist[t_part] < t_dist ? min_dist[t_part] : t_dist) ;
  }
  
}

float  distcheck::get_part(int dir)
{
  if(dir < 0 || dir >= parts) return 0.0f;
  
  return min_dist[dir]; 
}
