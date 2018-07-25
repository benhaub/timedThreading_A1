#########################################################
# Author	:	Ben Haubrich			#
# Files		:	makefile			#
# Synopsis	:	Windows makefile for square.c	#
#########################################################

CC = cl.exe
CFLAGS = /W4
CFLAGS_DEBUG = /Oi /Od /Zi

.PHONY: clean debug

square: square.obj
	$(CC) $(CFLAGS) square.obj

square.obj:	square.exe
	$(CC) $(CFLAGS) /c square.c

# debug compilations need to compile everything, so we won't worry about
# dependancies
# The following commands are usefull once the debuggers starts up:
# <0:000> .lines
# <0:000> l+t
# <0:000> l+s
# <0:000> bp square!main
debug: square.c 
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) square.c

clean:
	del square.ilk square.pdb vc140.pdb
