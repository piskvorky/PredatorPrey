SOURCES = src/*.cpp
CC = g++
CFLAGS = -O2 -Wall -DNDEBUG -ansi -pedantic
UNAME := $(shell uname)
EXEC = predator_prey

ifeq ($(UNAME),Linux)
	LIBRARIES = -lGL -lGLU -lglut
endif

ifeq ($(UNAME),Darwin)
	LIBRARIES = -framework GLUT -framework OpenGL -framework Cocoa
endif

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXEC) $(LIBRARIES)

clean:
	rm $(EXEC)

