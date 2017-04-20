SHELL = /bin/sh
FLAGS = -Wall -g
CC = g++

all:    TwoPipesTwoChildren TwoPipesThreeChildren DynPipe

TwoPipesTwoChildren:	TwoPipesTwoChildren.cpp
	$(CC) $(FLAGS) -o $@ TwoPipesTwoChildren.cpp

TwoPipesThreeChildren:	TwoPipesThreeChildren.cpp
	$(CC) $(FLAGS) -o $@ TwoPipesThreeChildren.cpp
	
DynPipe:	DynPipe.cpp
	$(CC) $(FLAGS) -o dynpipe DynPipe.cpp

clean:
	rm TwoPipesTwoChildren TwoPipesThreeChildren dynpipe
