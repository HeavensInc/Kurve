#include "text.h"

//Global TEXT object
__text text ;


///  1 / 12 (10x)
double coord_x [] = { 0 , 1/12.8 , 2/12.8 , 3/12.8 ,  4/12.8 ,  5/12.8 ,  6/12.8 ,
              7/12.8 , 8/12.8 , 9/12.8 , 10/12.8 , 11/12.8 , 12/12.8 } ;
///  7/32 (4x)
double coord_y [] = { 0 , 7/64.0f , 14/64.0f , 21/64.0f , 28/64.0f , 35/64.0f , 42/64.0f , 49/64.0f , 56/64.0f , 1 } ;

coordinaten getcoord(char letter)
{
  coordinaten chxy ;
  if (letter > 47 && letter < 58)
  {
    chxy.x = letter - 47 ;
    chxy.y = 1 ;
    return chxy;
  }

  if (letter == 1) /// M1
  {
    chxy.x = 3 ;
    chxy.y = 5 ;
    return chxy;
  }
  if (letter == 2) /// M2
  {
    chxy.x = 4 ;
    chxy.y = 5 ;
    return chxy;
  }


  if (letter == 45) /// Minus
  {
    chxy.x = 11 ;
    chxy.y = 1 ;
    return chxy;
  }
  if (letter == '+')
  {
    chxy.x = 12 ;
    chxy.y = 1 ;
    return chxy;
  }

  if (letter == '(')
  {
    chxy.x = 11 ;
    chxy.y = 3 ;
    return chxy;
  }
  if (letter == ')')
  {
    chxy.x = 12 ;
    chxy.y = 3 ;
    return chxy;
  }

  if (letter == '<')
  {
    chxy.x = 11 ;
    chxy.y = 4 ;
    return chxy;
  }
  if (letter == '>')
  {
    chxy.x = 12 ;
    chxy.y = 4 ;
    return chxy;
  }
  if (letter == 47 || letter == 92) /// Slash & Backslash
  {
    chxy.x = 11 ;
    chxy.y = 2 ;
    return chxy;
  }
  if (letter == 32) /// Space
  {
    chxy.x = 11 ;
    chxy.y = 5 ;
    return chxy;
  }
  if (letter == 46) /// Punkt
  {
    chxy.x = 10 ;
    chxy.y = 4 ;
    return chxy;
  }
  if (letter ==95) /// Underline
  {
    chxy.x = 7 ;
    chxy.y = 4 ;
    return chxy;
  }
  if (letter > 96) letter -= 32 ;

  if (letter > 64 && letter < 91)
  {
    chxy.x = (letter - 65)%10 +1 ;
    chxy.y = (letter - 65)/10 +2;
    return chxy;
  }

  /// Sonst:  [?]

  chxy.x = 8 ;
  chxy.y = 4 ;
  return chxy;
}  /// getchoord

void __text::load()
{
  SDL_Surface *surface;
  GLenum texture_format = 0;
  GLint  nOfColors      = 0;
  bool image_loaded ;

  image_loaded = (surface = SDL_LoadBMP( texture )) ;

  if ( image_loaded ) {
    nOfColors = surface->format->BytesPerPixel;
    if (nOfColors == 4)
    {
    if (surface->format->Rmask == 0x000000ff)
        texture_format = GL_RGBA;
      else
        texture_format = GL_BGRA;
    } else
     if (nOfColors == 3)
    {
      if (surface->format->Rmask == 0x000000ff)
        texture_format = GL_RGB;
      else
        texture_format = GL_BGR;
    } else {
      printf("warning: the image is not truecolor..  this will probably break\n");
    }
    glGenTextures( 1, &texID );
    glBindTexture( GL_TEXTURE_2D, texID );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
//      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
//      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
        texture_format, GL_UNSIGNED_BYTE, surface->pixels );
  }
  else {
    printf("Some Image could not be loaded : %s\n", SDL_GetError());
  }
  if(surface) SDL_FreeSurface( surface ) ;
}//load

void __text::draw(char* text , int x , int y)
{
  char* pos = text + strlen(text) -1;
  coordinaten xy;

  /// INIT
  glEnable( GL_TEXTURE_2D );
  glLoadIdentity();
  glBindTexture( GL_TEXTURE_2D, texID );
//  glColor4f (1.0f , 1.0f, 1.0f, 1.0f );

  glBegin(GL_QUADS);

  while( pos >= text )
  {
//    int i = strlen(text) + (int)(text - pos) ;
    int i = (int)(pos - text) ;

    xy = getcoord( *pos ) ;

//    DEBUG( std::cout << "Draw at " << i*10 + x << " , " << global.gl_height - y << "\n" ;)

    glTexCoord2f(  coord_x[xy.x-1]  , coord_y[xy.y-1] );
    glVertex3f( i*10 + x ,          global.gl_height - y ,         0.0f ) ;
    glTexCoord2f(  coord_x[xy.x]  , coord_y[xy.y-1] );
    glVertex3f( (i+1)*10 + x ,      global.gl_height - y ,         0.0f ) ;
    glTexCoord2f(  coord_x[xy.x]  , coord_y[xy.y] );
    glVertex3f( (i+1)*10 + x ,      global.gl_height - y - 14 ,    0.0f ) ;
    glTexCoord2f(  coord_x[xy.x-1]  , coord_y[xy.y] );
    glVertex3f( i*10   + x,         global.gl_height - y - 14 ,    0.0f ) ;
    pos--;
  }
  glEnd();

  glDisable( GL_TEXTURE_2D );
  glLoadIdentity();
}

char* get_steer_string(int id)
{
  const char* strings[] = { (char*)"1 Q" , (char*)"X C"       , (char*)"N M",
                            (char*)"< >" , (char*)"\x01 \x02" , (char*)"+ -" } ;
                   
  if(id > -1 && id < 6)
     return (char*)strings[id] ;
     
  return (char*)"ERR" ;
}

void __text::gamemenu(player_c* players , float alpha)
{
  for(int i=0;i<6;i++)
  {
    if(players[i].playing)
      { players[i].gl_color(alpha); }
    else
      { players[i].gl_color( DEF_ALPHAM * alpha); }

    char displaystring[256];    

    sprintf(displaystring, "Player %d  ( %s )  %s" , i+1, get_steer_string(i), players[i].get_name() );
    
    draw( displaystring , global.gl_width/2 - 110 , global.gl_height/2 - 20 * (5-i) + 40) ;
  }
}


void __text::helper(int select, bool reset, bool fade)
{
  static  int  last_help = 1 ;

  static  int  fading = 0 ;
  static  int  runner = 0 ;
  static  int  status = 0 ;  //  0..4

  if( select == 0 )
  {
    select = last_help ;
  }else{
    last_help = select ;
  }

  if(reset)
  {
    fading = 0 ;
    runner = 0 ;
    status = 0 ;
    return ;
  }

  if(fade)
  {
    status = 5 ;
  }

  switch(status)
  {
    case 0 :
      fading++;
      if(fading == 30) status++;
      break;
    case 1 :
      runner++;
      if(runner == 10){ status++; runner=0; }
      break;
    case 2 :
      fading--;
      if(fading == 15) status++;
      break;
    case 3 :
      runner++;
      if(runner == 10){ status=0; runner=0; }
      break;
    default :
      fading--;
      break;
  }

  if(fading > 0)
  {
    glColor4f(1.0f , 1.0f , 1.0f , fading / 40.0f);

    if(select == 1)
    {
      draw( (char*)"Select at least 2 Players to play" , global.gl_width/2 - 165 , global.gl_height/2 + 80 ) ;
    }
    else
    if(select == 2)
    {
      draw( (char*)"Press Space to start the game" , global.gl_width/2 - 145 , global.gl_height/2 + 80 ) ;
    }
    else
    {
      draw( (char*)"Press Space to continue" , global.gl_width/2 - 115 , global.gl_height - 30 ) ;
    }
  }

}


void __text::scores(player_c* players, float alpha)
{
  char string[32] ;

  glBegin( GL_QUADS );
  glColor4f(1.0f,1.0f,1.0f,0.1f * alpha);
  glVertex3f(	  0.0f         , global.gl_height     , 0.0f);
  glVertex3f(	  0.0f         , global.gl_height -22 , 0.0f);
  glVertex3f(global.gl_width , global.gl_height -23 , 0.0f);
  glVertex3f(global.gl_width , global.gl_height     , 0.0f);


  glColor4f(1.0f, 1.0f, 1.0f,0.20f * alpha);
  glVertex3f(global.gl_width      , global.gl_height -23   , 0.0f);
  glVertex3f(	        	0.0f      , global.gl_height -23   , 0.0f);
  glColor4f(1.0f, 1.0f, 1.0f,0.50f * alpha);
  glVertex3f(       		0.0f      , global.gl_height -25   , 0.0f);
  glVertex3f(global.gl_width      , global.gl_height -25   , 0.0f);

  glColor4f(1.0f, 1.0f, 1.0f,0.20f * alpha);
  glVertex3f(global.gl_width      ,                 2.0f   , 0.0f);
  glVertex3f(	        	0.0f      ,                 2.0f   , 0.0f);
  glColor4f(1.0f, 1.0f, 1.0f,0.50f * alpha);
  glVertex3f(       		0.0f      ,                 0.0f   , 0.0f);
  glVertex3f(global.gl_width      ,                 0.0f   , 0.0f);

  glColor4f(1.0f, 1.0f, 1.0f,0.20f * alpha);
  glVertex3f(           2.0f      ,                 0.0f   , 0.0f);
  glVertex3f(           2.0f      , global.gl_height -23   , 0.0f);
  glColor4f(1.0f, 1.0f, 1.0f,0.50f * alpha);
  glVertex3f(	        	0.0f      , global.gl_height -23   , 0.0f);
  glVertex3f(       		0.0f      ,                 0.0f   , 0.0f);
  
  glColor4f(1.0f, 1.0f, 1.0f,0.20f * alpha);
  glVertex3f(global.gl_width      ,                 0.0f   , 0.0f);
  glVertex3f(global.gl_width      , global.gl_height -23   , 0.0f);
  glColor4f(1.0f, 1.0f, 1.0f,0.50f * alpha);
  glVertex3f(global.gl_width-2.0f , global.gl_height -23   , 0.0f);
  glVertex3f(global.gl_width-2.0f ,                 0.0f   , 0.0f);


  glEnd();


  for(int i=0;i<6;i++)
  {
    if(players[i].playing)
    {
      if(players[i].isalive())
        players[i].gl_color(alpha);
      else 
        players[i].gl_color( DEF_ALPHAM * alpha);


      sprintf(string, "P%d %02d", i+1 , players[i].score ) ;
      draw(string , i * 70 + 20 , 5) ;
    }
    else
    {

      glColor4f(0.5f , 0.5f , 0.5f , DEF_ALPHAM * alpha) ;
      sprintf(string, "P%d --", i+1 ) ;
      draw(string , i * 70 + 20 , 5) ;
    }
  }
}

void  __text::countdown(float     status )
{
  int number = status + 0.75 ;
  status = (int)(status*100) % 100;
  status = status / 100 - 0.25f ;

  float center_x =  global.gl_width      / 2 ;
  float center_y = (global.gl_height-16) / 2 ;

  float size_x = (global.gl_height-16) / 14 * 5 ;
  float size_y = (global.gl_height-16) / 2 ;

  glColor4f( 1.0f,1.0f,1.0f, status ) ;

  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, texID );

  glBegin( GL_QUADS );
  glTexCoord2f(  coord_x[number-1]    , coord_y[ 0 ] );
  glVertex3f( center_x - size_x ,     center_y + size_y ,    0.0f ) ;
  glTexCoord2f(  coord_x[number  ]  , coord_y[ 0 ] );
  glVertex3f( center_x + size_x ,     center_y + size_y ,    0.0f ) ;
  glTexCoord2f(  coord_x[number  ]  , coord_y[ 1 ] );
  glVertex3f( center_x + size_x ,     center_y - size_y ,    0.0f ) ;
  glTexCoord2f(  coord_x[number-1]    , coord_y[ 1 ] );
  glVertex3f( center_x - size_x ,     center_y - size_y ,    0.0f ) ;
  glEnd();

  glDisable( GL_TEXTURE_2D );

}
