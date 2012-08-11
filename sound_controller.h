#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include <iostream>

class sound_controller
{
  private:
    	Mix_Chunk*  sounds[10];
      int         sound_files;
    	int         sound_channel;
    	bool        sound_working;
  public:
     sound_controller();
     ~sound_controller();
     
     void init();
     
     int LoadSound(char* file);
     void PlaySound(int snd_index);
};

extern sound_controller snd_ctl;
