#ifndef collision_h
#define collision_h

#include <iostream>
#include <math.h>
#include "constants.h"

struct trailobj;
struct trailobj
{
  trailobj* next ;
  trailobj* prev ;
  int status ;     // 0       : Normal
  		   // ? --> 1 : Fadein

  int type ;       // 0    : Normal
                   // 1    : Gap / Loch

  float x1, x2,	   // Coordinates the trail goes to in that step
        y1, y2 ;   //
        
  bool keep_this ;
};

class quadcheck
{
private:
  float points[4][2];
  float vector[4][2];
public:
  quadcheck( float[4][2] ) ;
  quadcheck( trailobj* tob);
       
  bool check_point(float x, float y);
};

class trailcheck
{
private:
  bool setup;
  trailobj* last;
  float x,y;
public:
  void  point(float new_x, float new_y);
  bool  check(trailobj* tob);
};


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
  distcheck(int parts);
  ~distcheck();
  
  void   setup(trailobj* tob);
  
  void   calc_walls();
  void   calc_tob(trailobj* tob);
  
  int   get_parts(){ return parts; }
  float get_part(int dir);
};

#endif //collision_h
