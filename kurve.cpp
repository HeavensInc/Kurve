#include "including.h"

// undef main if it was defined by SDL
#undef main

//  Create an empty version-string if none was supplied by the makefile
#ifndef VERSION
#define VERSION "0.00E"
#endif // VERSION

// !! Incomplete !!
// #define GM_SCORE

#include "player.h"
#include "text.h"
#include "staticwait.h"
#include "sound_controller.h"

bool      game_running;
bool      menu_loop;
bool      prep_loop;
bool      game_loop;
bool	    post_loop;

player_c  player[6];

void	  check_collisions();
void	  initialize_game();

//void      gl_init(int w, int h);
void      gl_init(bool fullscreen = false , int w = 0 , int h = 0 );
void      gl_setup(bool resize);

bool      commonevent(SDL_Event* event);
int       loop_mainmenu() ;
int       loop_initgame() ;
int       loop_run_game(bool render = true) ;
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
  
  global.dissolve=0;
  global.scale_id=0;
  global.scale = 1.0f ;
  global.speed_id=0;
  global.speed = 1.0f ;

  //  
  //  SDL-Start
  //

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  
  snd_ctl.init();
  
  global.sound_gap_skip = 0;
  global.sound_gap  = snd_ctl.LoadSound( "snd/gap.wav" );
  global.sound_kill = snd_ctl.LoadSound( "snd/kill.wav" );

  
  { // Init-time variables should not remain .. :)
    char caption[128];
    sprintf( caption , "Kurve %s" , VERSION );
    SDL_WM_SetCaption( (char*)caption , NULL );
    
    const SDL_VideoInfo* vinfo = SDL_GetVideoInfo();
    global.display_width  = vinfo->current_w ;
    global.display_height = vinfo->current_h ;

    global.window_width  = 800 ;
    global.window_height = 600 ;
  }

  gl_init(false);
  gl_setup(true);

  //
  // Main-Loops
  //
  int status;
  while ( game_running == true )
  {
    gl_setup(true);

    std::cout << "MAINLOOP" << std::endl ;
    
    SDL_ShowCursor( SDL_ENABLE ); 

    while ( game_running && menu_loop )
    {
      staticwait(20);
      status = loop_mainmenu();
      if(status == 1)
      {
        prep_loop = true;
        menu_loop = false;
      }
    }

    SDL_ShowCursor( SDL_DISABLE ); 

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
      bool zombies = true;
      for( int i=0;i<6;i++)
      {
        zombies = zombies && !( player[i].pt_get() == pt_human && player[i].isalive() ) ;
      }

      staticwait(20 / global.speed );

      status = loop_run_game();
      if(status == 1)
      {
        game_loop = false;
        post_loop = true;
      }
 
      if(zombies && game_loop)
      {
        for(int i=0;i<4;i++)
        {
          status = loop_run_game(false);
          if(status == 1)
          {
            game_loop = false;
            post_loop = true;
            break;
          }
        }
      }
    }

    while ( game_running && post_loop )
    {
      staticwait(20);
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

  //
  // CleanUP
  //

  SDL_Quit();

  return 1;
}

void  gl_init( bool fullscreen , int w , int h )
{
  if(fullscreen)
  {
    w = global.display_width;
    h = global.display_height;

    SDL_Surface* screen;
    screen = SDL_SetVideoMode( w , h , 0 , SDL_OPENGL | SDL_FULLSCREEN);
    
    if(screen)
    {
      w = screen->w;
      h = screen->h;
    }
    
  }else{
    if(w == 0 || h == 0)
    {
      w = global.window_width  ;
      h = global.window_height ;    
    }else{
      global.window_width  = w;
      global.window_height = h;
    }
    SDL_Surface* screen;
    screen = SDL_SetVideoMode( w , h , 0 , SDL_OPENGL | SDL_RESIZABLE );

    global.window_width  = screen->w;
    global.window_height = screen->h;
    w = screen->w;
    h = screen->h;
  }

  global.sdl_width  = w ;
  global.sdl_height = h ;
  
  text.load();
  particles.load();
  gl_setup(false);

}

void  gl_setup(bool resize)
{
  glViewport(0, 0, global.sdl_width, global.sdl_height);

  if(resize)
  {
    global.gl_width  = global.sdl_width  / global.scale;
    global.gl_height = global.sdl_height / global.scale;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, global.gl_width , 0.0f, global.gl_height , -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

  }else{

    glClearColor(0, 0, 0, 0);
    glClearDepth(1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0f, global.gl_width , 0.0f, global.gl_height , -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }
}

bool  commonevent(SDL_Event* event)
{
  static Uint8 *keystate = SDL_GetKeyState(NULL);
  static bool  multiblock = false ;
  if( (keystate[SDLK_LALT] || keystate[SDLK_RALT]) && keystate[SDLK_RETURN] && !multiblock)
  {
    global.fullscreen = !global.fullscreen ;
    gl_init(global.fullscreen);
    gl_setup(menu_loop) ;
    
    multiblock = true;
  }else{
    multiblock = (keystate[SDLK_LALT] || keystate[SDLK_RALT]) && keystate[SDLK_RETURN] ;
  }

  switch(event->type)
  {
    case SDL_VIDEORESIZE:
      gl_init( global.fullscreen , event->resize.w , event->resize.h ) ;
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
  player[0].set_color( 1.0f , 0.25f , 0.25f );
  player[1].set_color( 1.0f , 0.5f , 0.0f );
  player[2].set_color( 0.9f , 0.9f , 0.0f );
  player[3].set_color( 0.25f , 1.0f , 0.25f );
  player[4].set_color( 0.3f , 0.7f , 1.0f );
  player[5].set_color( 0.75f , 0.0f , 0.75f );
  
  static int status = 0;
  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    commonevent(&event);
    if(event.type == SDL_MOUSEBUTTONDOWN)
      if(event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT)
         player[4].pt_next();

    if(event.type == SDL_KEYDOWN && status < 3)
    {
    switch(event.key.keysym.sym)
      {
      case SDLK_ESCAPE:
        game_running = false;
        break;

      case SDLK_SPACE:
        if(global.playercount > 1)
          status++;
        break;

      case SDLK_F7:
        global.dissolve++;
        if(global.dissolve > 1) global.dissolve=0;
        break;

      case SDLK_F8:
        global.scale_id++;
        if(global.scale_id > 3) global.scale_id=0;
        
        if(global.scale_id == 0) global.scale=1.0f ;
        if(global.scale_id == 1) global.scale=1.5f ;
        if(global.scale_id == 2) global.scale=2.0f ;
        if(global.scale_id == 3) global.scale=3.0f ;
        
        if(global.sdl_height / global.scale < DEF_BDIST * 3 && global.sdl_width / global.speed < DEF_BDIST * 3)
        {
          global.scale_id=0;
          global.scale=1.0f ;
        }
        gl_setup(true);
        
        break;


      case SDLK_F9:
        std::cout << "F9!!\n" ;
        global.speed_id++;
        if(global.speed_id > 4) global.speed_id=0;
        
        if(global.speed_id == 0) global.speed=1.0f ;
        if(global.speed_id == 1) global.speed=0.7f ;
        if(global.speed_id == 2) global.speed=0.5f ;
        if(global.speed_id == 3) global.speed=2.0f ;
        if(global.speed_id == 4) global.speed=1.4f ;
                
        break;


      case SDLK_1:
      case SDLK_q:
        player[0].pt_next();
        break;

      case SDLK_x:
      case SDLK_c:
        player[1].pt_next();
        break;

      case SDLK_n:
      case SDLK_m:
        player[2].pt_next();
        break;

      case SDLK_LEFT:
      case SDLK_RIGHT:
        player[3].pt_next();
        break;

      case SDLK_KP_PLUS:
      case SDLK_KP_MINUS:
        player[5].pt_next();
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


  //
  // RENDER
  //

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
      {
        player[i].score = 0 ;      

        #ifdef GM_SCORE
          player[i].score = 10 ;
        #endif
      }
      
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
  text.settings(alpha);
  
  SDL_GL_SwapBuffers();

  return 0;
}


int         loop_initgame()
{ 
  //
  // RESET Player-Colors
  //
  player[0].set_color( 1.0f , 0.25f , 0.25f );
  player[1].set_color( 1.0f , 0.5f , 0.0f );
  player[2].set_color( 0.85f , 0.85f , 0.0f );
  player[3].set_color( 0.25f , 1.0f , 0.25f );
  player[4].set_color( 0.0f , 0.5f , 1.0f );
  player[5].set_color( 0.75f , 0.0f , 0.75f );
  
  
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
    gl_setup(true);
    
    global.livecount = global.playercount ;

    float a[6];
    float b[6];
    int tries=0;    
    int i=0;

    bool skip= false;

    while(i < 6)
    {
      skip=false;
      
      a[i] = rand() % (global.gl_height - 2 * DEF_BDIST) + DEF_BDIST ;
      b[i] = rand() % (global.gl_width  - 2 * DEF_BDIST) + DEF_BDIST ;
      
      for(int k=0;k<i;k++)
      {
        if(player[k].playing)
        {
          float dist= sqrt( (b[k] - b[i]) * (b[k] - b[i])  +  (a[k] - a[i]) * (a[k] - a[i])) ;
          if(dist < DEF_BDIST && tries < 10)
          {
            skip=true;
            tries++;
            break; //inner 'for'-loop
          }
        }
      }

      #ifdef GM_SCORE
        player[i].score = 10 ;
      #endif
      
            
      if(!skip)
      {
        player[i].initialize(b[i],a[i],i);
        i++;
        tries = 0 ;
      }

    }
    status++;
  }

  static int steps = 0 ;
  status++;
  if( status < 150 )
  {  
    for(int i=0;i<6;i++)
    {
      if(status % 20 == 0)
        player[i].render_go_step(player) ;
        
      player[i].render_trail_display() ;
    }
    particles.render();
    text.countdown( 4 - status / 50.0f );
  }

  SDL_GL_SwapBuffers();

  if(status >= 150)
  {
    status = 0;
    return 1;
  }

  return 0 ;
}


int         loop_run_game(bool render) //render=true
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
    player[i].render_go_step(player);
    if(render) 
      player[i].render_trail_display();
  }

  if(render) 
  {    
    text.scores(player);
    particles.render();
    SDL_GL_SwapBuffers();
    
    global.sound_gap_skip--;
  }
  
  for(int i=0;i<6;i++)
  {
    if(player[i].isalive())
    {
      bool  rand  = false; // anders spielen
      bool  check = false;
      int   killer = -1;
      trailobj* t_curr = player[i].get_t_current() ;
      float x = (t_curr->x1 + t_curr->x2) /2.0f;
      float y = (t_curr->y1 + t_curr->y2) /2.0f;
      if( x < DEF_WALL || y < DEF_WALL || x > global.gl_width - DEF_WALL || y > global.gl_height - DEF_SCORES - DEF_WALL )
      {
        check = true;
        rand = true ;
      }

      for(int k=0;k<6;k++)
      {      
        if(check) break;
        
        if( !player[k].playing ) continue ;
        
        trailobj* t_draw = player[k].get_t_start() ;
        trailobj* t_end  = player[k].get_t_current() ;
        
        
        //Prevent killing from behind. 
        t_end = t_end->prev;
        // And random suicide.. 
        if(i == k) t_end = t_end->prev;
        
        while( t_draw != t_end && !check)
        {
          t_draw = t_draw->next ;

          if(t_draw->type == 0)
          {
            check = check || player[i].collide_contains_point_head(t_draw->x1 , t_draw->y1);
            check = check || player[i].collide_contains_point_head(t_draw->x2 , t_draw->y2);
          }
        }
        

        if(!check)
        {
          t_draw = player[k].collide_contains_point_trail( t_curr->x1, t_curr->y1 , (i==k) );
          if(NULL == t_draw || t_draw->type != 0 || (t_draw->prev != NULL && t_draw->prev->type != 0))
          {
            t_draw = player[k].collide_contains_point_trail( t_curr->x2, t_curr->y2 , (i==k) );
          }
        }

        if( !check &&
            ((t_draw != NULL && t_draw->type != 0 )
            || (t_draw != NULL && t_draw->prev != NULL && t_draw->prev->type != 0 )))
        {
          float x = t_draw->x1/2 +t_draw->x2/2;
          float y = t_draw->y1/2 +t_draw->y2/2;
          for(int f=0; f<31; f++)
          {
            particle* pt =   pt_create_breakthrough(x,y, false) ;  
            particles.add(pt);
          }
          
          if(global.sound_gap_skip < 0)
          {
            snd_ctl.PlaySound( global.sound_gap );
            global.sound_gap_skip=3;
          }
        }
                  
        
        if(check)
          if(t_draw->status > 0)
            killer = k ;
        
        if(NULL != t_draw)
        {
          t_draw->status = 100;

          if(NULL != t_draw->next)
             t_draw->next->status = 100;
          if(NULL != t_draw->prev)
             t_draw->prev->status = 100;
          
             
          t_draw->keep_this = true;
          
          if(NULL != t_draw->next)
              t_draw->next->keep_this = true;
          if(NULL != t_draw->prev)
              t_draw->prev->keep_this = true;
          
        }


      }

      #ifdef GM_SCORE      
        static int suicide_prevention[6] = { 0 , 0 , 0 , 0, 0 , 0 };

        suicide_prevention[i]--;
      #endif
      
      if( check )
      {
        #ifdef GM_SCORE
        if( rand || suicide_prevention[i] < 0 )
        {
          player[i].score--;
          suicide_prevention[i] = 3 ;
        
          if( player[i].score < global.playercount - global.livecount + 1 )
          {
            global.livecount--;
            player[i].kill();
          }
        }
        #else
          global.livecount--;
          player[i].kill();
        
        for(int k=0;k<6;k++)
        {
          if( i != k && player[k].isalive())
          {
            player[k].score++;
            
            trailobj* tob=player[i].get_t_current() ;
            float x = (tob->x1 + tob->x2) /2;
            float y = (tob->y1 + tob->y2) /2;
            
            particle* pt =   pt_create_punkt(player[k].get_r(), player[k].get_g(), player[k].get_b(), x,y);  
            particles.add(pt);

          }
        }
        if(killer > -1 && !player[killer].isalive() ) player[killer].score++;
    
        #endif
        
        snd_ctl.PlaySound( global.sound_kill );

        float x = t_curr->x1/2 +t_curr->x2/2;
        float y = t_curr->y1/2 +t_curr->y2/2;
        for(int f=0; f<62; f++)
        {
          particle* pt =   pt_create_breakthrough(x,y, true) ;  
          particles.add(pt);
        }       

      }
    }
  }

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
  static int targetstate = 0 ;
  static int gfx_helper  = 0 ;

  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    commonevent(&event);

    int maxpkt = 0;
    for(int i=0;i<6;i++)
    {
      maxpkt = ( maxpkt > player[i].score ? maxpkt : player[i].score) ;
    }

    if(event.type == SDL_KEYDOWN)
    {

    switch(event.key.keysym.sym)
      {
      case SDLK_SPACE:
        if(maxpkt < 5 * global.playercount)
        {
          targetstate = 2;
        }else{
          targetstate = 1;
          #ifdef GM_SCORE
            targetstate = 2;
          #endif
        }
        break;
      case SDLK_ESCAPE:
        targetstate = 1;
        break;
      default:
        break;
      }
    }
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gl_setup(false);
  for(int i=0;i<6;i++)
  {
    player[i].render_trail_display();
  }
  particles.render();
  text.helper( 3 );
  text.scores(player);

  text.scoreboard(player, gfx_helper);

  SDL_GL_SwapBuffers();

  gl_setup(false);

  if(targetstate == 0)
  {
    gfx_helper++;
  }else{
    gfx_helper--;
    if(gfx_helper == 0)
    leavestate = targetstate ;
    if(gfx_helper > 50) gfx_helper = 50;
  }

  if(leavestate != 0)
  {
    targetstate = 0;
    gfx_helper  = 0;
  }

  return leavestate;
}
