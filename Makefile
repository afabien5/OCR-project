#OCR 2017/2018
# Compilers and options

# Setting the compiler and the default linker program

CC = gcc   -fsanitize=address
# options for pre-processor (-I, -include, -D ... )
CPPFLAGS = -MMD
# main compilation options
CFLAGS = -Wall -Wextra -std=c99 -O0 -g
# Linker options (probably always empty)
LDFLAGS =
# libs and path for linker
LDLIBS = -lm

#indicate source file and make a list

OBJ = neuron.o
DEP = ${OBJ:.o=.d} 

all: neuron

neuron: ${OBJ}

.PHONY: clean

clean:
	${RM} *.d      # remove object files
	${RM} *.o      # remove object files
	${RM} main     # remove main program

-include ${DEP}
# END

