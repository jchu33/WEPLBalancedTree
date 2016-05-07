CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -g3

HWK4= /c/cs223/Hwk5

all:    Words16
 
#####
# Instructions to make pancake
#####

Words16: Words16.o Tree.o getLine.o
	${CC} ${CFLAGS} -o Words16 Words16.o Tree.o getLine.o

Words16.o: Words16.c Tree.h getLine.h
	${CC} ${CFLAGS} -c Words16.c

Tree.o: Tree.c Tree.h
	${CC} ${CFLAGS} -c Tree.c

getLine.o: getLine.c getLine.h
	${CC} ${CFLAGS} -c getLine.c
