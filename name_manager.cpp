#include "name_manager.h"

void  __name_manager::set_name(int ID, char* new_name)
{
  bool fill=false;
  for(int i=0;i<8;i++)
  {
    if(new_name[i]==0)
      fill=true;
    
    if(fill)
      new_name[i]=' ';
  }

  new_name[8]=0;
  strcpy( name_storage[ID-1] , new_name );
}


char* __name_manager::get_name(int ID)
{
  if(ID > 0 && ID < 7)
    return name_storage[ID-1] ;

  return (char*)"NO_NAMES" ;
}

void  __name_manager::set_tag(int ID, char* new_name)
{
  bool fill=false;
  for(int i=0;i<2;i++)
  {
    if(new_name[i]==0)
      fill=true;
    
    if(fill)
      new_name[i]=' ';
  }

  new_name[2]=0;
  strcpy( tags_storage[ID-1] , new_name );
}

char* __name_manager::get_tag(int ID)
{
  if(ID > 0 && ID < 7)
    return tags_storage[ID-1] ;

  return (char*)"ER" ;
}

        
void  __name_manager::load()
{
    ifstream filein( "names.cfg" ) ;

    if(! filein)
    {
        std::cout << "\nFileError - Cannot Load Names (Gimme a names.cfg)\n" ; 
        return ;
    }
 
    char	line[256]; 
  	int		tmpi = 0;
  	char	tmps[256];
    
     while( !filein.eof())
    {
        filein.getline(line , 200 , '\n') ;

        if(line[0] == 0 ) continue;

        if(sscanf(line, "N%d=%s", &tmpi, tmps))
        {
          if(tmpi > 0 && tmpi < 7 && strlen(tmps) > 0 )
            set_name(tmpi, tmps);
        }
        
        if(sscanf(line, "T%d=%s", &tmpi, tmps))
        {
          if(tmpi > 0 && tmpi < 7 && strlen(tmps) > 0 )
            set_tag(tmpi, tmps);
        }
    }
}

