all:
	-rm game
	g++  -DGLM_ENABLE_EXPERIMENTAL -o game game.cpp renderer.cpp -lGL -lglfw -lGLEW -lpthread

