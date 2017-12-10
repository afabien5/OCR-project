## Simple SDL mini code
 
CC=gcc
 
CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O0 -g
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image
 
OBJ= pixel_operations.o main.o segmentation.o
DEP= ${SRC:.o=.d}
 
all: main
 
segmentation.c: segmentation.h

main: ${OBJ} segmentation.h
 
clean:
	${RM} ${OBJ} ${DEP} *~
	${RM} main
 
-include ${DEP}
 
# END
