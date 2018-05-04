#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <pthread.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "renderer.hpp"


int main( int argc, char **argv )
{
    //Start renderer thread
    pthread_t renderer_thread;
    pthread_create( &renderer_thread, NULL, renderer::renderer_init, NULL );

	//Wait for renderer to finish
    pthread_join( renderer_thread, NULL );
}