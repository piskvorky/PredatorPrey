SOURCES = src/*.cpp
CC = g++
CFLAGS = -O2 -Wall -DNDEBUG
UNAME := $(shell uname)
EXEC = predator_prey

ifeq ($(UNAME),Linux)
	LIBRARIES = -lGL -lGLU -lglut
endif

ifeq ($(UNAME),Darwin)
	LIBRARIES = -framework GLUT -framework OpenGL -framework Cocoa
endif

all:
	$(CC) $(CFLAGS) $(LIBRARIES) $(SOURCES) -o $(EXEC)

clean:
	rm $(EXEC)

