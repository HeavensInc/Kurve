#ifndef player_h
#define player_h

#include <iostream>
#include <cstdlib>
#include "including.h"

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


class player_c
{
private:
  int  disappear;
  int  id ;

  int hole_next       ; // Hole information are to be reset
  int hole_len        ; // when a hole is finished

  trailobj* t_start   ;
  trailobj* t_current ;

  bool   alive ;
  bool   dead  ;

  int    deathfade ; // 0 --> 10

  float  color_trail[3] ;
  float  color_gap[3] ;

  int    winkel;

public:
  int    score;
  bool   playing;
public:
  player_c()          ;
  ~player_c()         ;

  void  initialize( float x, float y, int  id );
  void  set_color(  float r, float g, float b );
  void  gl_color(float alpha=1.0f);

  void  render_trail_collide() ;
  void  render_go_step() ;
  void  render_trail_display() ;
  void  render_trail_display_fade(float alpha, float white) ;

  void  collide_prepare() ; // set view to a quad containing the 2 points from t_current

  int   collide_check() ;

  void  go_left()    {  winkel = (winkel + 1) % DEF_WMAX ; }
  void  go_right()   {  winkel = (winkel - 1) % DEF_WMAX ; }
  bool  isalive()    {  return alive && playing ;  } 
  void  kill()       {  dead = true; }

  trailobj* get_t_start(   ) { return t_start   ; } ;
  trailobj* get_t_current( ) { return t_current ; } ;

};


#endif
