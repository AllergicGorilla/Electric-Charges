CXX = g++
CXXFLAGS = -std=c++11
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE=electric

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): $(SOURCES)
	    $(CXX) $(CXXFLAGS) -c $(SOURCES) $(LIBS)

$(OBJECTS): charge.hpp utilities.hpp
clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)
