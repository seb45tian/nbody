CC=g++
CFLAGS=-c -Wno-deprecated -O3
LDFLAGS=-framework GLUT -framework OpenGL -lboost_system
SOURCES=GL.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=nbody

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) simulation.h particle.h vec3d.h 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE)
