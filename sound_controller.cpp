#include "sound_controller.h"

sound_controller snd_ctl;

sound_controller::sound_controller()
{
  for(int i=0; i<10; i++)
    sounds[i] = NULL ;
  sound_files=0;
}

void sound_controller::init()
{
	int     audio_rate      = 22050;			    //Frequency of audio playback
	Uint16  audio_format    = AUDIO_S16SYS; 	//Format of the audio we're playing
	int     audio_channels  = 2;			        //2 channels = stereo
	int     audio_buffers   = 4096;		        //Size of the audio buffers in memory
	
	sound_working = true;
	
	//Initialize SDL_mixer with our chosen audio settings
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		printf("Unable to initialize audio: %s\n", Mix_GetError());
		sound_working = false;
	}
}

sound_controller::~sound_controller()
{
  if(sound_working)
  {
    for(int i=0; i<10; i++)
      if( sounds[i] != NULL )
      {
      	Mix_FreeChunk(sounds[i]);
      }
  	Mix_CloseAudio();
  }
}

int  sound_controller::LoadSound(char* file)
{
  std::cout << "Trying to load: " << file << std::endl;
  if(sound_working && sound_files < 10)
  {
  	sounds[sound_files] = Mix_LoadWAV( file );
  	if(sounds[sound_files] == NULL) {
      printf("Unable to load WAV file: %s\n", Mix_GetError());
      return -1;
	  }else{
	    sound_files++;
	    return sound_files - 1 ;
    }
  }else{
    return -1 ;
  }
}

void sound_controller::PlaySound(int snd_index)
{
  if(sound_working && snd_index > -1 && snd_index < 10 )
  {
    sound_channel = Mix_PlayChannel(-1, sounds[snd_index], 0);

	  if(sound_channel == -1) {
		  printf("Unable to play WAV file: %s\n", Mix_GetError());
	  }
  }
}

