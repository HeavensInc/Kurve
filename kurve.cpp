#include "including.h"

// undef main if it was defined by SDL
#undef main
#ifndef VERSION
#define VERSION "0.00E"
#endif // VERSION


#include "player.h"
#include "text.h"
#include "staticwait.h"

bool      game_running;
bool      menu_loop;
bool      prep_loop;
bool      game_loop;
bool	  post_loop;

player_c  player[6];

void	  check_collisions();
void	  initialize_game();

void      gl_init(int w, int h);
void      gl_setup(bool resize);

bool      commonevent(SDL_Event* event);
int       loop_mainmenu() ;
int       loop_initgame() ;
int       loop_run_game() ;
int       loop_postgame() ;

int main(int argc, char *argv[])
{
  //randomize
  srand((unsigned)time(NULL));

  //Variables...
  //
  //

  game_running = true ;
  menu_loop    = true ;
  prep_loop    = false;
  game_loop    = false;
  post_loop    = false;

  player[0].set_color( 1.0f , 0.0f , 0.0f );
  player[1].set_color( 1.0f , 0.5f , 0.0f );
  player[2].set_color( 1.0f , 1.0f , 0.0f );
  player[3].set_color( 0.0f , 1.0f , 0.0f );
  player[4].set_color( 0.0f , 0.5f , 1.0f );
  player[5].set_color( 1.0f , 0.0f , 1.0f );

  //SDL-Start
  //
  //

  SDL_Init(SDL_INIT_VIDEO);
  {
    char caption[32];
    sprintf( caption , "Kurve %s" , VERSION );
    SDL_WM_SetCaption( (char*)caption , NULL );
  }

  gl_init(800, 600);
  gl_setup(true);

  // Main-Loops
  //
  //
  int status;
  while ( game_running == true )
  {
    gl_setup(true);

    while ( game_running && menu_loop )
    {
      staticwait(20);
//      SDL_Delay(20);
      status = loop_mainmenu();
      if(status == 1)
      {
        prep_loop = true;
        menu_loop = false;
      }
    }

    while ( game_running && prep_loop )
    {
      staticwait(20);

      status = loop_initgame();
      if(status == 1)
      {
        prep_loop = false;
        game_loop = true;
      }
    }

    while ( game_running && game_loop )
    {
//      SDL_Delay(20);
      staticwait(20);
      status = loop_run_game();
      if(status == 1)
      {
        game_loop = false;
        post_loop = true;
      }
    }

    while ( game_running && post_loop )
    {
      SDL_Delay(20);
      status = loop_postgame();

      if(status == 1)
      {
        post_loop = false;
        menu_loop = true;
      }
      if(status == 2)
      {
        post_loop = false;
        prep_loop = true;
      }
    }

  }

  // CleanUP
  //
  //

  SDL_Quit();

  return 1;
}


void  gl_init( int w , int h )
{
  SDL_SetVideoMode( w , h ,32,SDL_OPENGL|SDL_RESIZABLE);

  global.sdl_width  = w ;
  global.sdl_height = h ;
  
  glClearColor(0, 0, 0, 0);
  glClearDepth(1.0f);
  glViewport(0, 0, global.sdl_width, global.sdl_height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA , GL_ONE);

  text.load();
}

void  gl_setup(bool resize)
{
  if(resize)
  {
    global.gl_width  = global.sdl_width;
    global.gl_height = global.sdl_height;
  }

  glOrtho(0.0f, global.gl_width , 0.0f, global.gl_height , -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

bool  commonevent(SDL_Event* event)
{
    switch(event->type)
    {
      case SDL_VIDEORESIZE:
        gl_init( event->resize.w , event->resize.h ) ;
        gl_setup(menu_loop) ;
        break;

      case SDL_QUIT:
        game_running = false;
        break;

      default:
        return false;
        break;
    }
    return true;
}

int   loop_mainmenu()
{
  static int status = 0;
  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    commonevent(&event);
    if(event.type == SDL_MOUSEBUTTONDOWN)
      if(event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT)
         player[4].playing = !player[4].playing ;

    if(event.type == SDL_KEYDOWN && status < 3)
    {
    switch(event.key.keysym.sym)
      {

      case SDLK_SPACE:
        if(global.playercount > 1)
          status++;
        break;


      case SDLK_1:
      case SDLK_q:
        player[0].playing = !player[0].playing ;
        break;

      case SDLK_x:
      case SDLK_c:
        player[1].playing = !player[1].playing ;
        break;

      case SDLK_n:
      case SDLK_m:
        player[2].playing = !player[2].playing ;
        break;

      case SDLK_LEFT:
      case SDLK_RIGHT:
        player[3].playing = !player[3].playing ;
        break;

      case SDLK_KP_PLUS:
      case SDLK_KP_MINUS:
        player[5].playing = !player[5].playing ;
        break;

      default:
        break;
      }
    }
  }

	global.playercount = 0;
	for(int i=0;i<6;i++)
	{
  	if(player[i].playing) global.playercount++;
	}

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // RENDER

  static float alpha = 0.0f;

  if(alpha == 0.0f)
    text.helper( 1 , true , false );

  if(global.playercount > 1)
    text.helper( 2 );
  else
    text.helper( 1 );

  if(status == 2)
  {
    text.gamemenu(player, alpha);
  }

  if(status == 0)
  {
    alpha += DEF_MENUFADE ;
    if(alpha > 1.0f)
    {
      alpha = 1.0f ;
      status = 2 ;

      for(int i=0;i<6;i++)
        player[i].score = 0 ;
    }
    text.gamemenu(player, alpha);
  }

  if(status == 1)
  {
    alpha += DEF_MENUFADE ;
    if(alpha > 1.0f)
    {
      alpha = 1.0f ;
      status = 2 ;
    }
    text.gamemenu(player, alpha);
    text.scores(player, 1.0f-alpha);
  }

  if(status >= 3)
  {
    text.helper( 0 , false , true );
    alpha -= DEF_MENUFADE ;
    if(alpha < 0.1f)
    {
      alpha = 0.0f ;
      status = 1 ;

      for(int i=0;i<6;i++)
        player[i].score = 0 ;

      return 1;
    }
    text.gamemenu(player, alpha);
    text.scores(player, 1.0f-alpha);
  }

  SDL_GL_SwapBuffers();

  return 0;
}


int         loop_initgame()
{
  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    commonevent(&event);
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  text.scores(player);
  text.helper(0, false, true);

  static int status = 0;
  if(status == 0)
  {
    global.livecount = global.playercount ;

    float a[6];
    float b[6];
    int tries=0;    
    int i=0;
    while(i < 6 && tries < 10)
    {
      a[i] = rand() % (global.gl_height - 2 * DEF_BDIST) + DEF_BDIST ;
      b[i] = rand() % (global.gl_width  - 2 * DEF_BDIST) + DEF_BDIST ;
      
      for(int k=0;k<i;k++)
      {
        if(player[k].playing)
        {
          float dist= sqrt( (b[k] - b[i]) * (b[k] - b[i])  +  (a[k] - a[i]) * (a[k] - a[i])) ;
          if(dist < DEF_BDIST)
          {
            tries++;
            continue;
          }
        }
      }      
      player[i].initialize(b[i],a[i],i);
      i++;
    }
    status++;
  }

  static int steps = 0 ;
  if(status == 1)
  {
    steps++;
//    SDL_Delay(50);
    for(int i=0;i<6;i++)
    {
      player[i].render_go_step() ;
      player[i].render_trail_display() ;
    }
    if(steps > 5)
    {
      status = 2;
      steps  = 0;
    }
  }

  if(status > 1)
  {
//    SDL_Delay(1000.0f / DEF_DEADFADE );
//      staticwait(20); //   SDL_Delay(20);

    float fade = ((DEF_NUMFADE - steps) * 1.0f /DEF_NUMFADE < 0 ? 0 : (DEF_NUMFADE - steps) * 1.0f /DEF_NUMFADE) ;

    text.countdown( 5 - status + fade );

    fade -= 0.5f ;

    for(int i=0;i<6;i++)
    {
      player[i].render_trail_display_fade( DEF_ALPHA , fade ) ;
    }
    steps++;
    if(steps > 50)
    {
      steps=0;
      status++;
    }
  }

  SDL_GL_SwapBuffers();

  if(status==5)
  {
    steps  = 0;
    status = 0;
    for(int i=0;i<6;i++)
    {
      trailobj* t_draw;
      t_draw = player[i].get_t_start();
      while( player[i].get_t_current() != t_draw )
      {
        t_draw->status = 8;
        t_draw = t_draw->next ;
      }
    }

    return 1;
  }

  return 0 ;
}


int         loop_run_game()
{
  gl_setup(false);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    commonevent(&event);
  }


  Uint8 mousestate = SDL_GetMouseState(NULL, NULL);
  static Uint8 *keystate = SDL_GetKeyState(NULL);

  if(keystate[SDLK_1]) player[0].go_left();
  if(keystate[SDLK_q]) player[0].go_right();

  if(keystate[SDLK_x]) player[1].go_left();
  if(keystate[SDLK_c]) player[1].go_right();

  if(keystate[SDLK_n]) player[2].go_left();
  if(keystate[SDLK_m]) player[2].go_right();

  if(keystate[SDLK_LEFT])  player[3].go_left();
  if(keystate[SDLK_RIGHT]) player[3].go_right();

  if(mousestate & SDL_BUTTON(1)) player[4].go_left();
  if(mousestate & SDL_BUTTON(3)) player[4].go_right();

  if(keystate[SDLK_KP_PLUS])  player[5].go_left();
  if(keystate[SDLK_KP_MINUS]) player[5].go_right();


  for(int i=0;i<6;i++)
  {
    player[i].render_go_step();
    player[i].render_trail_display();
  }

  text.scores(player);

  SDL_GL_SwapBuffers();

  for(int i=0;i<6;i++)
  {
    if(player[i].isalive())
    {
      bool  check = false;
      int   killer = -1;
      trailobj* t_curr = player[i].get_t_current() ;
      float x = (t_curr->x1 + t_curr->x2) /2.0f;
      float y = (t_curr->y1 + t_curr->y2) /2.0f;
      if( x < DEF_WALL || y < DEF_WALL || x > global.gl_width - DEF_WALL || y > global.gl_height - DEF_SCORES - DEF_WALL )
        check = true;

      for(int k=0;k<6;k++)
      {      
        if( !player[k].playing ) continue ;
        
        trailobj* t_draw = player[k].get_t_start() ;
        trailobj* t_end  = player[k].get_t_current() ;
        
        
        if(i == k) t_end = t_end->prev->prev;
        
        while( t_draw != t_end && !check)
        {
          t_draw = t_draw->next ;

          if(t_draw->type == 0 )
          {
            check = check || player[i].collide_contains_point_head(t_draw->x1 , t_draw->y1);
            check = check || player[i].collide_contains_point_head(t_draw->x2 , t_draw->y2);
          }
        }
        

        if(!check)
        {
          t_draw = player[k].collide_contains_point_trail( t_curr->x1, t_curr->y1 , (i==k) );
          if(NULL == t_draw)
          {
            t_draw = player[k].collide_contains_point_trail( t_curr->x2, t_curr->y2 , (i==k) );
          }
        }

        if(NULL != t_draw)
        {
          check = true;
        }

        
        if(check && t_draw->status > 0) killer = k ;
      }
  
      if( check )
      {
        global.livecount--;
        player[i].kill();
        for(int k=0;k<6;k++)
        {
          if( i != k && player[k].isalive()) player[k].score++;
        }
        if(killer > -1) player[killer].score++;
      }
    }
  }


//  DEBUG( std::cout << " L " << global.livecount << " P " << global.playercount << "\n" ; )

  if(global.livecount < 2)
  {
    text.helper( 3 , true , false );
    return 1 ;
  }

  return 0;
}




int         loop_postgame()
{
  int leavestate = 0 ;
  gl_setup(false);

  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    commonevent(&event);

    if(event.type == SDL_KEYDOWN)
    {
    int maxpkt = 0;
    switch(event.key.keysym.sym)
      {
      case SDLK_SPACE:
        for(int i=0;i<6;i++)
        {
          maxpkt = ( maxpkt > player[i].score ? maxpkt : player[i].score) ;
        }
        if(maxpkt < 5 * global.playercount)
        {
          leavestate = 2;
        }else{
          leavestate = 1;
        }
        break;
      case SDLK_ESCAPE:
        leavestate = 1;
        break;
      default:
        break;
      }
    }
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for(int i=0;i<6;i++)
  {
    player[i].render_trail_display();
  }
  text.helper( 3 );
  text.scores(player);

  SDL_GL_SwapBuffers();

//  DEBUG( std::cout << " L " << global.livecount << " P " << global.playercount << "\n" ; )

  return leavestate;
}
