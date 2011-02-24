#ifndef collision_h
#define collision_h

#include <iostream>
#include <math.h>

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

#endif //collision_h
