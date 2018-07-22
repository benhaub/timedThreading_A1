#################################################
# Authour	:	Ben Haubrich		#
# File		:	makefile		#
# Synopsis	:	makefile for square.c	#
#################################################


.PHONY:	clean

#Variable declarations

# Flags for the C compiler
CFLAGS = -Wall -Wextra -std=c89

# Object files
SQUARES_OBJ = squares.o

# C source files for complete compiling
SQUARES_C = $(SQUARES_OBJ:.o=.c)

#Rules

squares: squares.o
	 gcc $(CLFAGS) $(SQUARES_OBJ) -o squares

squares.o: squares.c squares.h
	gcc $(CFLAGS) -c sqaures.c

clean:
	 rm *.o
	 rm core.*






