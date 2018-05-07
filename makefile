DEBUG = -g -fno-builtin -O0

CXX = g++
CXXFLAGS = -DGLM_ENABLE_EXPERIMENTAL -Wall $(DEBUG)
LIBS = -lpthread -lGL -lglfw -lGLEW -lpng16
SRC = game.cpp renderer.cpp voxel.cpp model.cpp shaderset.cpp

all:
	-rm game
	$(CXX) $(CXXFLGAS) $(SRC) $(LIBS) -o game    

install_deps:
	apt install `cat deps.txt`