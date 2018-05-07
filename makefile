all:
	-rm game
	g++  -DGLM_ENABLE_EXPERIMENTAL -o game game.cpp renderer.cpp voxel.cpp model.cpp shaderprogram.cpp -lpthread -lGL -lglfw -lGLEW -lpng16 -g -fno-builtin -O0 -Wall

