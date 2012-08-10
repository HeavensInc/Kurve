#include "player.h"

// Names ...

const char AI_name_nothing[] = " --- " ;
const char AI_name_human[]   = "Playing" ;
const char AI_name_basic[]   = "AI-Medium (Dist 9)" ;
const char AI_name_fine[]    = "AI-Easy   (Dist 3)" ;

const char AI_name_areas[]    = "AI-Hopefully Hard (Areas)" ;

const char AI_name_unknown[] = "AI-Whatever" ;
const char AI_name_error[]   = "[ E R R ]" ;

char* get_AI_name(int pt)
{
  if(pt < 0 || pt >= pt_enum_end )
    return (char*)AI_name_error ;
  
  switch(pt)
  {
  case pt_nothing:
    return (char*)AI_name_nothing;
  case pt_human:
    return (char*)AI_name_human ;
  case pt_ai_distances:
    return (char*)AI_name_basic ;
  case pt_ai_fine:
    return (char*)AI_name_fine ;
    
  default:
    return (char*)AI_name_unknown ;
  }
}
// Globals & temp

GLuint select_buf[DEF_SBUF];
GLuint hits;

// --------------

player_c::player_c()
{
  intelligence = NULL ;
  
  score   =     0 ;
  playing = false ;
}

player_c::~player_c()
{
  trailobj* t_temp ;

  while( t_start != t_current )
  {
    t_temp    = t_current ;
    t_current = t_current->prev;
    delete    t_temp ;
  }
  
  if(intelligence != NULL)
    delete intelligence;
}

char* player_c::get_name()
{
  return get_AI_name(playertype);
}

void  player_c::pt_next()
{
  playertype = (playertype+1) % pt_enum_end;
  playing = (playertype != 0);
}

void  player_c::pt_prev()
{
  playertype = (playertype-1) % pt_enum_end;
  playing = (playertype != 0);
}

void  player_c::go_left( bool force)
{
  if( playertype != pt_human && !force )
    return;
  
  winkel = (winkel + 1) % DEF_WMAX ;
  winkel_changed = +1 ; 
}

void  player_c::go_right(bool force)
{
  if( playertype != pt_human && !force )
    return;

  winkel = (winkel - 1) % DEF_WMAX ;
  winkel_changed = -1 ;
}

void  player_c::initialize( float x, float y, int id) 
{
  //
  //  Initialize AI if used?
  //
  if(intelligence != NULL)
  {
    std::cout << "Killing AI at "<< id << std::endl ;
    delete intelligence;
    intelligence = NULL;
  }
  
  if(playertype == pt_ai_distances)
  {
    std::cout << "Loading AI-basic at "<< id << std::endl ;
    intelligence = new AI_basic ; // AI_basic
  }
  if(playertype == pt_ai_fine)
  {
    std::cout << "Loading AI-fine at "<< id << std::endl ;
    intelligence = new AI_fine ; // AI_fine
  }
  
  //
  //  Initialize player 
  //
  if(t_start)
  {
    trailobj* t_temp ;
    while( t_start != t_current )
    {
      t_temp    = t_current ;
      t_current = t_current->prev;
      delete    t_temp ;
    }
  }else{
    t_start   = new trailobj ;
  }

  winkel = rand() % DEF_WMAX ;
  winkel_changed = 0 ;

  float tmp_winkel_go  = 2 * M_PI * (winkel - winkel_changed / 2.0f) / DEF_WMAX ;
  float tmp_winkel_dir = 2 * M_PI *  winkel                          / DEF_WMAX ;

  t_start->x1      = x + cos( tmp_winkel_go ) * DEF_STEPLEN + sin( tmp_winkel_dir ) * DEF_TRWIDTH ;
  t_start->y1      = y + sin( tmp_winkel_go ) * DEF_STEPLEN - cos( tmp_winkel_dir ) * DEF_TRWIDTH ;
  t_start->x2      = x + cos( tmp_winkel_go ) * DEF_STEPLEN - sin( tmp_winkel_dir ) * DEF_TRWIDTH ;
  t_start->y2      = y + sin( tmp_winkel_go ) * DEF_STEPLEN + cos( tmp_winkel_dir ) * DEF_TRWIDTH ;

  t_start->status = 10 ;
  t_start->keep_this = false;

  hole_next = rand() % 50 + 25 ;
  hole_len  = rand() %  5 +  5 ;
  
  deathfade = DEF_DEADFADE;
  dead = false;
  alive= true;
  disappear = 20 ;

  t_current = t_start;
  t_start->next = NULL;
  this->id = id ;
  
  if(!playing) return;
  
  particle* pt =   pt_create_start(color_trail[0],color_trail[1],color_trail[2], x,y, tmp_winkel_go);  
  particles.add(pt);

}

void  player_c::set_color(  float r, float g, float b)
{
  color_trail[0] = r ;
  color_trail[1] = g ;
  color_trail[2] = b ;

  color_gap[0] = r * 0.3f ;
  color_gap[1] = g * 0.3f ;
  color_gap[2] = b * 0.3f ;
}

void  player_c::gl_color(float alpha , float white)
{
  glColor4f( color_trail[0] + white, color_trail[1] + white , color_trail[2] + white , alpha ) ;
}


void  player_c::render_go_step(player_c* players)
{
  alive = !dead ;

  if( isalive() ) 
  {
    // AI if present
    if(intelligence != NULL)
    {
//      std::cout << "AI Processing..." << std::endl ;

      trailobj* tobs[6];
      
      for(int i=0;i<6;i++)
        tobs[i] = players[i].get_t_start();

//      std::cout << "AI Patching" << std::endl ;

      intelligence->patch_pointers(tobs, t_current );
      
//      std::cout << "AI Calculating" << std::endl ;

      intelligence->calculate();
      
//      std::cout << "AI Steering" << std::endl ;

//      intelligence->steer();

      winkel          = ( winkel + intelligence->steer() ) % DEF_WMAX ;
      winkel_changed  =            intelligence->steer() ;
      
//      std::cout << "AI done" << std::endl ;

    }
    // Go a step...
  
    trailobj* step = new trailobj;
    step->status = 30 ;
    step->type   = ( hole_len > hole_next ? 1 : 0 ) ;
    step->keep_this = false;
//    float tmp_winkel = 2 * M_PI * winkel / DEF_WMAX ;

    float  x, y;
    x = (t_current->x1 + t_current->x2) / 2.0f ;
    y = (t_current->y1 + t_current->y2) / 2.0f ;

    float tmp_winkel_go  = 2 * M_PI * (winkel + winkel_changed / 2.0f) / DEF_WMAX ;
    float tmp_winkel_dir = 2 * M_PI *  winkel                          / DEF_WMAX ;

    step->x1      = x + cos( tmp_winkel_go ) * DEF_STEPLEN + sin( tmp_winkel_dir ) * DEF_TRWIDTH ;
    step->y1      = y + sin( tmp_winkel_go ) * DEF_STEPLEN - cos( tmp_winkel_dir ) * DEF_TRWIDTH ;
    step->x2      = x + cos( tmp_winkel_go ) * DEF_STEPLEN - sin( tmp_winkel_dir ) * DEF_TRWIDTH ;
    step->y2      = y + sin( tmp_winkel_go ) * DEF_STEPLEN + cos( tmp_winkel_dir ) * DEF_TRWIDTH ;

    winkel_changed = 0 ;

    
    hole_next-- ;
    if( hole_next == 0 )
    {
      hole_next = rand() % 50 + 25 ;
      hole_len  = rand() %  5 +  5 ;
    }

    t_current->next = step;
    step->prev = t_current;
    t_current = step;
    
    step->next = NULL;

    disappear--;
    if(disappear == 0  && global.dissolve == 0) // && !t_start->keep_this
    {
      disappear = 2 ;
      t_start = t_start->next ;
      delete t_start->prev ;
      t_start->prev = NULL ;
      
    }
    
    if(intelligence == NULL || true)
    {
      particle* pt =   pt_create_dot(color_trail[0],color_trail[1],color_trail[2], x,y, tmp_winkel_go) ;  
      particles.add(pt);
    }
    
  }
}

void  player_c::render_trail_display()  
{
  if(dead)
  {
    render_trail_display_fade(0.8f , deathfade / 1.5f / DEF_DEADFADE );
    deathfade = ( deathfade > 0 ? deathfade - 1 : 0 ) ;
  }else{
    render_trail_display_fade(1.0f ,              0.0f);
  }

}


void  player_c::render_trail_display_fade( float alpha , float white )
{
  if(!playing) return;

  trailobj* t_draw;
  t_draw = t_start;

  glBegin( GL_TRIANGLE_STRIP );

  do
  {
    float flash = ( t_draw->status ) / 40.0f ;
    flash = (flash < 0 || flash > 1 ? 1 : flash ) ;
    flash = flash * flash * (t_draw->type == 0 ? 1 : 0.5) ;   
    
    float alpha2 = alpha ;//* (t_draw->status == 0 ? 0.66f : 1 ) ;   

    
    if(t_draw->type == 0)
      glColor3f(
          ( flash + color_trail[0] + 0.5f ) /2 * alpha2  + white ,
          ( flash + color_trail[1] + 0.5f ) /2 * alpha2  + white ,
          ( flash + color_trail[2] + 0.5f ) /2 * alpha2  + white
             );
    else
      glColor3f(
          ( flash +  color_gap[0]  + 0.2f ) /2 * alpha2  + white ,
          ( flash +  color_gap[1]  + 0.2f ) /2 * alpha2  + white ,
          ( flash +  color_gap[2]  + 0.2f ) /2 * alpha2  + white
             );
/*
    if(t_draw->type == 0)
      glColor3f(
          ( 0.5f )  * alpha2  + white ,
          ( 0.5f )  * alpha2  + white ,
          ( 0.5f )  * alpha2  + white
             );
    else
      glColor3f(
          ( 0.2f )  * alpha2  + white ,
          ( 0.2f )  * alpha2  + white ,
          ( 0.2f )  * alpha2  + white
             );
*/           
    glVertex2f( t_draw->x1 , t_draw->y1 ) ;
    glVertex2f( t_draw->x2 , t_draw->y2 ) ;

    if( t_draw->status > 0 ) t_draw->status-- ;
  }
  while( NULL != (t_draw = t_draw->next) );

  glEnd() ;

}


bool  player_c::collide_contains_point_head(int x, int y)
{
  quadcheck qc(t_current);

  if(qc.check_point(x,y)) std::cout << "killed by " << x << " " << y << std::endl ;
  
  return  qc.check_point(x,y);
}


trailobj*  player_c::collide_contains_point_trail(int x, int y, bool self)
{
  trailcheck tc;
  tc.point(x,y);

  trailobj* t_juice = NULL;
  
  trailobj* t_draw;
  t_draw = t_start;
  
  trailobj* t_end;
  t_end = t_current;
  
  //prevent self collision
//  if(self) t_end = t_end->prev->prev->prev;
  if(self) t_end = t_end->prev;
  
  while( t_draw != t_end )
  {
    t_draw = t_draw->next ;

    if(t_draw->type == 0 && !(t_draw->prev == NULL || t_draw->prev->type > 0))
    {
      if(tc.check(t_draw))
      {
//        t_draw->keep_this = true ;
        return t_draw;
      }
    }else{
      if(tc.check(t_draw))
      {
        t_juice = t_draw;
      }
    }
  }
  return t_juice; 
}
