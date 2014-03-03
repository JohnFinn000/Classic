
CC=g++
FLAGS= -g
LIBS= -lmenu -lform -lpanel -lncurses++ 

all: Snake

snake.o: snake.cpp snake.h
	${CC} ${FLAGS} -c snake.cpp

Snake: snake.o
	${CC} snake.o -o Snake ${LIBS}

.PHONY: clean
clean:
	rm snake.o
