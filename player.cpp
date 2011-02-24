#include "player.h"

// Globals & temp

GLuint select_buf[DEF_SBUF];
GLuint hits;

// --------------

player_c::player_c()
{
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
}

void  player_c::initialize( float x, float y, int id) 
{
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

  hole_next = rand() % 50 + 25 ;
  hole_len  = rand() %  5 +  5 ;
  
  deathfade = DEF_DEADFADE;
  dead = false;
  alive= true;
  disappear = 50 ;

  t_current = t_start;
  this->id = id ;
}
void  player_c::set_color(  float r, float g, float b)
{
  color_trail[0] = r ;
  color_trail[1] = g ;
  color_trail[2] = b ;

  color_gap[0] = 0.20 ;
  color_gap[1] = 0.20 ;
  color_gap[2] = 0.20 ;
}

void  player_c::gl_color(float alpha)
{
  glColor4f( color_trail[0] , color_trail[1] , color_trail[2] , alpha ) ;
}

void  player_c::collide_prepare()
{

  glSelectBuffer(DEF_SBUF, select_buf);
  glRenderMode(GL_SELECT);

  glInitNames();
  glPushName(10);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float x = ( t_current->x1 + t_current->x2 ) / 2.0f ;
  float y = ( t_current->y1 + t_current->y2 ) / 2.0f ;
  
  glOrtho( x - DEF_TRWIDTH * 0.7f , x + DEF_TRWIDTH * 0.7f , y - DEF_TRWIDTH * 0.7f , y + DEF_TRWIDTH * 0.7f , -1.0f , 1.0f) ;


//  glColor4f( 1.0f,1.0f,1.0f , 0.25f);

}

int   player_c::collide_check()
{
  glPopName();
  glFlush();
  hits=0;
  hits = glRenderMode(GL_RENDER);
  glLoadIdentity();

  if(hits==0 || !alive || !playing ) return -1;

  GLuint  *bufp = select_buf ;
  GLuint  name, numnames, z1, z2;
  bool    killer = false ;

  for(unsigned int j = 0; j < hits; j++)
  {
    numnames = *bufp++;
    z1 = *bufp++;
    z2 = *bufp++;
    DEBUG( std::cout << " N " << numnames ; )
    while(numnames--)
    {
      name = *bufp++;

      if(name > 0 && name < 7 && name != (id+1))
      { 
        return name ; 
      }

      if(name == 10 || name == 0)
      {
        killer = true ;
      }
    }
  }

  float  x, y;
  x = t_current->x1 + t_current->x2 ;
  y = t_current->y1 + t_current->y2 ;
  if(x < 0 || y < 0 || x > 2* global.gl_width || y > 2*global.gl_height -50)
    {  killer = true;  }

  if (killer) return 10 ;
  return -1 ;
}

void  player_c::render_go_step()
{
  alive = !dead ;

  if( isalive() ) 
  {
    // Go a step...
  
    trailobj* step = new trailobj;
    step->status = 30 ;
    step->type   = ( hole_len > hole_next ? 1 : 0 ) ;

    float tmp_winkel = 2 * M_PI * winkel / DEF_WMAX ;


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

    disappear--;
    if(disappear == 0)
    {
      disappear = 10 ;
      t_start = t_start->next ;
      delete t_start->prev ;
      t_start->prev = NULL ;
    }

  }
}

void  player_c::render_trail_display()  
{
  if(dead)
  {
    render_trail_display_fade(0.5f , deathfade / 1.5f / DEF_DEADFADE );
    deathfade = ( deathfade > 0 ? deathfade - 1 : 0 ) ;
  }else{
    render_trail_display_fade(1.0f ,              0.0f);
  }

}

void  player_c::render_trail_collide()
{
  if(!playing) return;

  // render with no color and without holes (gray in display)
  // NOTE: Make Gaps
  bool drawing = true ;
  bool named = false ;
  glLoadName(10);
  glBegin( GL_TRIANGLE_STRIP );

  trailobj* t_draw;
  t_draw = t_start;
  while( t_draw != t_current )
  {
    t_draw = t_draw->next ;
    if(t_draw->status == 0 && named)
    {
      named = false ;
      glEnd();
      glLoadName(10);
      glBegin( GL_TRIANGLE_STRIP );
    }

    if(t_draw->status != 0 && !named)
    {
      named = true ;
      glEnd();
      glLoadName(id + 1);
      glBegin( GL_TRIANGLE_STRIP );
    }


    if(t_draw->type == 0 && !drawing )
    {
      drawing=true;
      glBegin( GL_TRIANGLE_STRIP );
    }
      
    if(t_draw->type != 0 && drawing )
    {
      drawing=false;
      glEnd();
    }
      
    if(drawing)
    {
      glVertex3f( t_draw->x1 , t_draw->y1 , 0.0f ) ;
      glVertex3f( t_draw->x2 , t_draw->y2 , 0.0f ) ;
    }
  }

  glEnd() ;

}

void  player_c::render_trail_display_fade( float alpha , float white )
{
  if(!playing) return;

  trailobj* t_draw;
  t_draw = t_start;

  glBegin( GL_TRIANGLE_STRIP );

  while( t_current != t_draw )
  {
    t_draw = t_draw->next ;

    if(t_draw->type == 0)
      glColor3f(
          ( ( t_draw->status % 10 ) / 10.0f + color_trail[0] )  * alpha  + white ,
          ( ( t_draw->status % 10 ) / 10.0f + color_trail[1] )  * alpha  + white ,
          ( ( t_draw->status % 10 ) / 10.0f + color_trail[2] )  * alpha  + white
             );
    else
      glColor3f(
          ( ( t_draw->status % 10 ) / 10.0f + color_gap[0] )  * alpha  + white ,
          ( ( t_draw->status % 10 ) / 10.0f + color_gap[1] )  * alpha  + white ,
          ( ( t_draw->status % 10 ) / 10.0f + color_gap[2] )  * alpha  + white
             );

    glVertex2f( t_draw->x1 , t_draw->y1 ) ;
    glVertex2f( t_draw->x2 , t_draw->y2 ) ;

    if( t_draw->status ) t_draw->status-- ;
  }
  glEnd() ;

}

