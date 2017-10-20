#OCR 2017/2018
# Compilers and options

# Setting the compiler and the default linker program

CC = gcc   -fsanitize=address
# options for pre-processor (-I, -include, -D ... )
CPPFLAGS = -MMD
# main compilation options
CFLAGS = -Wall -Wextra -std=c99
# Linker options (probably always empty)
LDFLAGS =
# libs and path for linker
LDLIBS =

#built an .o for each file you need.
main: neuro.o 

#indicate source file and make a list

SRC = neuro.c
OBJ = ${SRC:.c=.o}

all: main

main: ${OBJ}

clean:
	${RM} *.d      # remove object files
	${RM} *.o      # remove object files
	${RM} main     # remove main program

-include ${DEP}
# END

