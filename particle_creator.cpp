#include "particle_creator.h"

particle* pt_create_start(float r,float g,float b,
                float x,float y,
                float winkel)
{
  particle* pt = new particle;
  pt->prev = NULL;
  pt->next = NULL;

  pt->type = PT_START ;

  pt->r = r;
  pt->g = g;
  pt->b = b;
  
  pt->pos.x = x;
  pt->pos.y = y;
  pt->pos.z = 0;
  
  pt->vel.x = cos( winkel ) * DEF_STEPLEN / 20;
  pt->vel.y = sin( winkel ) * DEF_STEPLEN / 20;
  pt->vel.z = 0;
  
  pt->accel.x = 0 ;
  pt->accel.y = 0 ;
  pt->accel.z = 0 ;
  
  pt->brake    = 1.0f;
  pt->lifetime = 150 ;
  pt->fadetime = 100  ;
  pt->size     = 30 ;
  
  return pt;
}

particle* pt_create_punkt(float r,float g,float b,
                float x,float y)
{
  particle* pt = new particle;
  pt->prev = NULL;
  pt->next = NULL;

  pt->type = PT_PUNKT ;
  pt->icon = rand() % 4 + 1 ;  

  pt->r = r;
  pt->g = g;
  pt->b = b;
  
  pt->pos.x = x;
  pt->pos.y = y;
  pt->pos.z = 0;
  
  float winkel = 2 * M_PI * rand() / RAND_MAX ;
  pt->vel.x = cos( winkel ) * DEF_STEPLEN  ;
  pt->vel.y = sin( winkel ) * DEF_STEPLEN  ;
  pt->vel.z = 0;
  
  if( x <                    50 ) pt->vel.x =  abs(pt->vel.x);
  if( x > global.gl_width  - 50 ) pt->vel.x = -abs(pt->vel.x);
  if( y <                    50 ) pt->vel.y =  abs(pt->vel.y);
  if( y > global.gl_height - 50 ) pt->vel.y = -abs(pt->vel.y);
  
  pt->accel.x =  0.0 ;
  pt->accel.y =  0.0 ;
  pt->accel.z =  0 ;
  
  pt->brake    = 0.95f;
  pt->lifetime = 150 ;
  pt->fadetime = 100  ;
  pt->size     = 8 ;

  return pt;
}


particle* pt_create_dot(float r,float g,float b,
                float x,float y,
                float winkel)
{
  particle* pt = new particle;
  pt->prev = NULL;
  pt->next = NULL;

  pt->type = PT_DOT ;

  pt->r = r;
  pt->g = g;
  pt->b = b;
  
  pt->pos.x = x;
  pt->pos.y = y;
  pt->pos.z = 0;
  
  float richtung = 2 * M_PI * rand() / RAND_MAX ;
  pt->vel.x = cos( winkel ) * DEF_STEPLEN  / 5  +   cos( richtung ) * DEF_STEPLEN  / 5;
  pt->vel.y = sin( winkel ) * DEF_STEPLEN  / 5  +   sin( richtung ) * DEF_STEPLEN  / 5;
  pt->vel.z = 0;
  
  pt->accel.x = 0 ;
  pt->accel.y = 0 ;
  pt->accel.z = 0 ;
  
  pt->brake    = 1.0f;
  pt->lifetime = 40 ;
  pt->fadetime = 25  ;
  pt->size     = 5 ;
  
  return pt;
}
