CC=g++
CFLAGS=-c --std=c++0x -Wno-deprecated -O3 -Wall

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
    LDFLAGS=-lGL -lGLU -lglut -lboost_system -lboost_thread-mt -lboost_program_options
else ifeq ($(UNAME), Darwin)
    LDFLAGS=-framework GLUT -framework OpenGL -lboost_system -lboost_thread-mt -lboost_program_options
else
    $(error Can only compile on Linux or OS X/Darwin. You are on: $(UNAME))
endif

SOURCES=GL.cpp
HEADERS=simulation.h particle.h node.h bhtree.h vec3d.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=nbody

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(HEADERS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE)
