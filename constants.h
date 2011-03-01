#ifndef constants_h
#define constants_h

//#define DEBUG(a) a;
#define DEBUG(a) ;

#define DEF_TIMESTEP    10
#define DEF_STEPLEN   3.0f
#define DEF_TRWIDTH   2.0f
#define DEF_PLAYERS      6  // maybe at some time
#define DEF_SBUF       128
//#define DEF_WMAX        96
#define DEF_WMAX        72

#define DEF_BDIST      150
#define DEF_WALL      1.5f
#define DEF_SCORES      25

#define DEF_DEADFADE    30
#define DEF_MENUFADE  0.1f
#define DEF_NUMFADE    100
#define DEF_ALPHA    0.75f
#define DEF_ALPHAM   0.40f


struct __globals
{
  int sdl_height;
  int sdl_width ;
  int gl_height ;
  int gl_width  ;

  int playercount;
  int livecount ;
};

extern __globals global ;

#endif
