CXX = g++
CXXFLAGS = -std=c++11
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE=Electric

$(EXECUTABLE): all
	rm -rf $(OBJECTS)

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LIBS)

$(OBJECTS): $(SOURCES)
	    $(CXX) $(CXXFLAGS) -c $(SOURCES) $(LIBS)

$(OBJECTS): charge.hpp utilities.hpp
clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)
