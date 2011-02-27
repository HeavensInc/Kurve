#ifndef player_h
#define player_h

#include <iostream>
#include <cstdlib>
#include "including.h"
#include "collision.h"

#include "AI_base.h"
#include "AI_basic.h"

enum
{
  pt_nothing,
  pt_human,
  pt_ai_distances,
  
  pt_enum_end
}
;

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

  int    playertype;
  AI_base* intelligence;

  int    deathfade ; // 0 --> 10

  float  color_trail[3] ;
  float  color_gap[3] ;

  int    winkel;         // direction .. what short english word would fit?
  int    winkel_changed;
public:
  int    score;
  bool   playing;
public:
  player_c()  ;
  ~player_c() ;

  void  pt_next();
  void  pt_prev();
  
  void  initialize( float x, float y, int  id );
  void  set_color(  float r, float g, float b );
  void  gl_color(float alpha=1.0f);

  void  render_go_step(player_c* players) ;
  void  render_trail_display() ;
  void  render_trail_display_fade(float alpha, float white) ;

  bool  collide_contains_point_head(int x, int y);  
  trailobj*  collide_contains_point_trail(int x, int y, bool self=false);

  void  go_left( bool force = false) ; 
  void  go_right(bool force = false) ;
  void  kill()       {  dead = true; }

  bool  isalive()    {  return alive && playing ;  } 
  char* get_name();
  
  trailobj* get_t_start(   ) { return t_start   ; } ;
  trailobj* get_t_current( ) { return t_current ; } ;

}
;

#endif
