#ifndef particles_h
#define particles_h

#include <iostream>
#include <cstdlib>
#include <time.h>
#include <math.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>


enum PT_FORM
{
  PT_START,
  PT_PUNKT,
  PT_END
};

struct particle
{
  particle*  next;
  particle*  prev;

  PT_FORM type;
  int     icon;
  float   size;

  struct
  {
    float x,y,z ;
  } pos,       // Current Position
    vel,       // Current Velocity
    accel;     // Acceleration (not modified while life)
  float brake; // Percentage of velocity remaining

  float r,g,b;
  
  int   lifetime;   // time left to live
  int   fadetime;   // time when to start fading
  int   shrinktime; // time when to shrink
  float shrink;     // final size (Percent, May grow too, i.e. 2.0f = 200% = twice as large as initial)
  
};

class __pt_manager
{
  private:
    particle* pt_start;

    GLuint texID;
    char texture [64]  ;
    
  public:
    __pt_manager();
    ~__pt_manager(); //Note: Delete all the particles
  
    void load(); //textures, you know?
    
    void clear();
    void add(particle* pt);
 
    void render(bool recalc=true);   
    void calculate();
        
    void render_start(particle *);
    void render_punkt(particle *);
}
;

#endif //particles_h
