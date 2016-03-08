CC=g++ -std=c++0x
CFLAGS=-c -Wno-deprecated -O3
LDFLAGS=-framework GLUT -framework OpenGL -lboost_system -lboost_thread-mt -lboost_program_options
SOURCES=GL.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=nbody

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) simulation.h particle.h vec3d.h bhtree.h 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE)
