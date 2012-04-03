VERSION = 0.05 - U haz name?

CC      = /usr/bin/g++
W32CC   = /usr/bin/mingw32-g++

CFLAGS  = -Wall -O2 -g "-DVERSION=\"$(VERSION) Linux\""
W32CF   = -Wall -O2 -g "-DVERSION=\"$(VERSION) W32\""

LIBS    = -lSDL -lGL
W32LIBS = -static-libgcc -static-libstdc++ -lSDL -lopengl32

UNUSED  =  -lSDLmain  -lmingw32

DEF = including.h staticwait.h
HDR = player.h constants.h text.h collision.h AI_base.h AI_basic.h AI_fine.h name_manager.h
SRC = kurve.cpp $(HDR:%.h=%.cpp)
OBJ = $(SRC:%.cpp=%.o)
W32 = $(SRC:%.cpp=%.w32)


.PHONY: clean clean-obj rebuild cross-rebuild

kurve: $(OBJ) $(HDR) $(DEF) 
	$(CC) $(CFLAGS) -o kurve $(OBJ) $(LIBS)
	
test: testsuite.o 
	$(CC) $(CFLAGS) -o test testsuite.o constants.o collision.o -lSDL

%.o: %.cpp $(HDR) $(DEF) 
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(W32) kurve kurve.exe

objclean:
	rm -f $(OBJ) $(W32)

rebuild: clean kurve


cross-rebuild: clean cross

cross: $(W32) $(HDR) $(DEF) 
	$(W32CC) $(W32CF) -o kurve.exe $(W32) $(W32LIBS)


%.w32: %.cpp $(HDR) $(DEF) 
	$(W32CC) $(W32CF) -o $@ -c $<
	
	
	
