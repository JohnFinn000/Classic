
CC=g++
FLAGS= -g
LIBS=-lncurses -lmenu -lform -lpanel

snake.o: snake.cpp snake.h
	${CC} ${FLAGS} snake.cpp ${LIBS}

.PHONY: clean
clean:
	rm snake.o
