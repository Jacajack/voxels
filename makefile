DEBUG = -g -fno-builtin -O0

CXX = g++
CXXFLAGS = -DGLM_ENABLE_EXPERIMENTAL -Wall $(DEBUG)
LIBS = -lpthread -lGL -lglfw -lGLEW -lpng16
SRC = src/demo.cpp src/ifrit.cpp

all:
	-rm game
	$(CXX) $(CXXFLAGS) $(SRC) $(LIBS) -o demo

install_deps:
	apt install `cat deps.txt`