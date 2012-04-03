#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>

using namespace std ;
class __name_manager{
    protected:
        char name_storage[6][32];
        char tags_storage[6][32];
    public:
        __name_manager()
        {
            strcpy( name_storage[0] , "Player 1" );
            strcpy( name_storage[1] , "Player 2" );
            strcpy( name_storage[2] , "Player 3" );
            strcpy( name_storage[3] , "Player 4" );
            strcpy( name_storage[4] , "Player 5" );
            strcpy( name_storage[5] , "Player 6" );

            strcpy( tags_storage[0] , "P1" );
            strcpy( tags_storage[1] , "P2" );
            strcpy( tags_storage[2] , "P3" );
            strcpy( tags_storage[3] , "P4" );
            strcpy( tags_storage[4] , "P5" );
            strcpy( tags_storage[5] , "P6" );
        }

        void  load();
        void  set_name(int ID, char* new_name);
        char* get_name(int ID);

        void  set_tag(int ID, char* new_name);
        char* get_tag(int ID);
} ;

