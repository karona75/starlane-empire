OBJS = se.o ose.o main.o introduction.o screen.o rollover.o mouse.o mainmenu.o cfgnew.o font.o video.o intro.o basic_rollover.o state_rollover.o menu_rollover.o starmap.o color_rollover.o assistant.o sound.o settings.o image.o flc_decoder.o raceobjs.o rules.o
CFLAGS = -Wall -O1 -g -I/usr/include/lua5.2 `xml2-config --cflags` `sdl-config --cflags` -Wno-deprecated
#CFLAGS = -Wall -Werror -O4 -march=k6 -mcpu=k6
CC = g++

starlane-empire:	$(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o starlane-empire `sdl-config --libs` -lSDL_image -lSDL_mixer -llua  -lm -lSDL_ttf `xml2-config --libs` -lGL -lGLU  -L/usr/include
$(OBJS):	%.o:	%.cc
	$(CC) -c $<  $(CFLAGS)
clean:	
	rm -f *.o core starlane-empire
