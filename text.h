#ifndef text_h
#define text_h

#include "including.h"
#include "player.h"

#include "name_manager.h"
#include "particles.h"

struct coordinaten
{
    int x ;
    int y ;
};

coordinaten getcoord(char letter);


class __text{
    protected:
        GLuint texID;
        char texture [64]  ;
        __name_manager name_manager;
    public:
        __text()
        {
            strcpy(texture , "zahlen_u_buchstaben.bmp" );
        }

        void  load();
        void  draw(char* text , int x , int y);
        void  gamemenu (player_c* players, float alpha = 1.0f) ;
        void  settings (float alpha);
        void  scores   (player_c* players, float alpha = 1.0f) ;
        void  scoreboard(player_c* players, int) ;
        void  countdown(float     status ) ;
      	void  helper(int select = 0, bool reset = false, bool fade = false) ;

} ;

extern __text text ;

#endif
