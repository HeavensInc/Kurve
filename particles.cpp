#include "particles.h"

__pt_manager::__pt_manager()
{
  strcpy(texture , "partikel.bmp" );
  
  pt_start = NULL;
  texID = 0;
  
}

__pt_manager::~__pt_manager()
{
  clear();
}

void __pt_manager::clear()
{
  particle* pt = pt_start;
  if( pt == NULL ) return;
  
  while(pt->next != NULL)
  {
    pt = pt->next ;
    delete pt->prev;
  }
  delete pt;
  pt_start = NULL;
}

void __pt_manager::add(particle* pt)
{
  pt->next       = pt_start;

  if(pt_start != NULL)
    pt_start->prev = pt;

  pt_start       = pt;
}

void __pt_manager::calculate()
{
  particle* pt  = pt_start;
  particle* fut = NULL;
  
  while( !(pt == NULL) )
  {
    fut = pt->next;
    
    pt->pos.x = pt->pos.x + pt->vel.x;
    pt->pos.y = pt->pos.y + pt->vel.y;
    pt->pos.z = pt->pos.z + pt->vel.z;
    
    pt->vel.x = pt->vel.x * pt->brake + pt->accel.x ;
    pt->vel.y = pt->vel.y * pt->brake + pt->accel.y ;
    pt->vel.z = pt->vel.z * pt->brake + pt->accel.z ;
  
    pt->lifetime--;
    
    if(pt->lifetime <= 0)
    {
//    std::cout << "DELETING" << std::endl;
    
      if(pt->prev==NULL)
      {
        pt_start = pt->next;
      }else{
        pt->prev->next = pt->next;
      }
      
      if(pt->next != NULL)
      {
        pt->next->prev = pt->prev;
      }
  
      delete pt;    
    }
    
    pt = fut;
  }
}

void __pt_manager::render(bool recalc) // recalc=true
{
  if(recalc) calculate();
  
  particle* pt = pt_start;
  while(pt != NULL)
  {
    switch(pt->type)
    {
      case PT_START:
        render_start(pt);
        break;
      case PT_PUNKT:
        render_punkt(pt);
        break;
    }
    
  
    pt = pt->next;
  }  
}

void __pt_manager::load()
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
//      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
        texture_format, GL_UNSIGNED_BYTE, surface->pixels );
  }else{
    printf("Some Image could not be loaded : %s\n", SDL_GetError());
  }
  if(surface) SDL_FreeSurface( surface ) ;
}
    
void __pt_manager::render_start(particle* pt)
{
  glDisable(GL_TEXTURE_2D);
  glLoadIdentity();
  
  glBegin(GL_TRIANGLE_FAN);

  glColor4f(pt->r,pt->g,pt->b, 0.0f);
  glVertex3f(  pt->pos.x ,   pt->pos.y ,  0 ) ;
  
  glColor4f(0,0,0,pt->lifetime*1.0f/pt->fadetime);
  for(int i=0;i<=100;i++)
  {
    glVertex3f(  pt->pos.x + pt->size * cos(M_PI * i/25) ,          pt->pos.y + pt->size * sin(M_PI * i/25) ,       0.0f ) ;
  }  
  glEnd();

}

void __pt_manager::render_punkt(particle* pt)
{
  glEnable( GL_TEXTURE_2D );
  glLoadIdentity();
  glBindTexture( GL_TEXTURE_2D, texID );
  glColor4f(pt->r,pt->g,pt->b, pt->lifetime*1.0f/pt->fadetime );

  glBegin(GL_QUADS);
  const int imgpos = pt->icon;
  glTexCoord2f( (imgpos-1)*0.25  , 0 );
  glVertex3f(  pt->pos.x-pt->size , pt->pos.y+pt->size , pt->pos.z ) ;
  glTexCoord2f( (imgpos)*0.25  , 0 );
  glVertex3f(  pt->pos.x+pt->size , pt->pos.y+pt->size , pt->pos.z ) ;
  glTexCoord2f( (imgpos)*0.25  , 1 );
  glVertex3f(  pt->pos.x+pt->size , pt->pos.y-pt->size , pt->pos.z ) ;
  glTexCoord2f( (imgpos-1)*0.25 , 1 );
  glVertex3f(  pt->pos.x-pt->size , pt->pos.y-pt->size , pt->pos.z ) ;
    
  glEnd();
  
  glDisable( GL_TEXTURE_2D );
  glLoadIdentity();
}

