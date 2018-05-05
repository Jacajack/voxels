all:
	-rm game
	g++  -DGLM_ENABLE_EXPERIMENTAL -o game game.cpp renderer.cpp voxel.cpp model.cpp -lGL -lglfw -lGLEW -lpthread -lpng16 -g -fno-builtin -O0 -Wall

