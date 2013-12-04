#
# Application name and list of source files.
#
NAME=iseg
CCSRCS=$(NAME).cpp 

#
# Dependency libraries
#
PKG_LIBS=-lR3Graphics -lR3Shapes -lR2Shapes -lRNBasics

#
# Targets
#
OBJS=$(CCSRCS:.cpp=.o) $(CSRCS:.c=.o) 
INCS=$(HSRCS) $(CCSRCS:.cpp=.h) $(CSRCS:.c=.h)

#
# C flags
#
CC=g++
BASE_CFLAGS=$(USER_CFLAGS) -Wall -I. -I../../gaps/pkgs -I../include -I.. -I../../freeglut
DEBUG_CFLAGS=$(BASE_CFLAGS) -g
OPT_CFLAGS=$(BASE_CFLAGS) -O3 -DNDEBUG
CFLAGS=$(DEBUG_CFLAGS)

#
# Get OS and the architecture type
#
OS=$(shell uname -o)
ARCH=$(shell uname -m)

#
# Directories
#
RELEASE_DIR=../../release
EXE_DIR=../../../exe
LIB_DIR=../../gaps/lib/$(ARCH)
GL_DIR=../../freeglut

#
# Default rules
#

.SUFFIXES: .c .cpp .o

.cpp.o:
	$(CC) $(CFLAGS) -c $<

.c.o:
	gcc $(CFLAGS) -c $<

#
# Set up target app
#
EXE=$(EXE_DIR)/$(NAME)

#
# Set up link options
#
BASE_LDFLAGS=$(USER_LDFLAGS) -L$(LIB_DIR)
DEBUG_LDFLAGS=$(BASE_LDFLAGS) -g
OPT_LDFLAGS=$(BASE_LDFLAGS) -O 
LDFLAGS=$(DEBUG_LDFLAGS)

#
# Set up libs
#
ifeq ("$(OS)","Cygwin")
OPENGL_LIBS=-lglut32 -lglu32 -lopengl32
else
OPENGL_LIBS=-L$(GL_DIR) -lfreeglut -lGLU -lGL -lm
endif
LIBS=$(USER_LIBS) $(PKG_LIBS) $(OPENGL_LIBS)

#
# Make targets
#
opt:
	    $(MAKE) $(EXE) "CFLAGS=$(OPT_CFLAGS)" "LDFLAGS=$(OPT_LDFLAGS)"

debug:
	    $(MAKE) $(EXE) "CFLAGS=$(DEBUG_CFLAGS)" "LDFLAGS=$(DEBUG_LDFLAGS)"

$(EXE):	    $(OBJS) $(LIBDIR)
	    mkdir -p $(EXE_DIR)
	    $(CC) -o $(EXE) $(LDFLAGS) $(USER_OBJS) $(OBJS) $(LIBS)

clean:
	    -  rm -f *~ *.o $(EXE) 


