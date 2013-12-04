NAME=freeglut
CSRCS=\
freeglut_callbacks.c \
freeglut_cursor.c \
freeglut_display.c \
freeglut_ext.c \
freeglut_font.c \
freeglut_font_data.c \
freeglut_gamemode.c \
freeglut_geometry.c \
freeglut_glutfont_definitions.c \
freeglut_init.c \
freeglut_main.c \
freeglut_menu.c \
freeglut_misc.c \
freeglut_overlay.c \
freeglut_state.c \
freeglut_stroke_mono_roman.c \
freeglut_stroke_roman.c \
freeglut_structure.c \
freeglut_teapot.c \
freeglut_videoresize.c \
freeglut_window.c \
freeglut_joystick.c


USER_CFLAGS=-DHAVE_CONFIG_H

OBJS=$(CCSRCS:.C=.o) $(CSRCS:.c=.o) 
INCS=$(HSRCS) $(CCSRCS:.C=.h) $(CSRCS:.c=.h)

CC=g++
BASE_CFLAGS=$(USER_CFLAGS) -Wall -I.  
DEBUG_CFLAGS=$(BASE_CFLAGS) -g
OPT_CFLAGS=$(BASE_CFLAGS) -O3 -DNDEBUG
CFLAGS=$(DEBUG_CFLAGS)


.SUFFIXES: .c .C .o

.C.o:
	$(CC) $(CFLAGS) -c $<

.c.o:
	gcc $(CFLAGS) -c $<



LIB=lib$(NAME).a


opt:
	    $(MAKE) $(LIB) "CFLAGS=$(OPT_CFLAGS)" 

debug:
	    $(MAKE) $(LIB) "CFLAGS=$(DEBUG_CFLAGS)" 

$(LIB):     $(CCSRCS) $(CSRCS) $(OSRCS) $(OBJS) 
	    rm -f $(LIB)
	    ar ur $(LIB) $(OBJS) $(USER_OBJS)

clean:
	    - rm -f *~ *.o $(LIB)




