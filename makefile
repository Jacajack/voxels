all:
	-rm game
	g++  -DGLM_ENABLE_EXPERIMENTAL -o game game.cpp renderer.cpp voxel.cpp model.cpp -lGL -lglfw -lGLEW -lpthread

