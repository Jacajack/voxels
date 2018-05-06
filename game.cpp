#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <pthread.h>
#include <sys/time.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "renderer.hpp"


int main( int argc, char **argv )
{
    //Start renderer thread
    pthread_t renderer_thread;
    pthread_create( &renderer_thread, NULL, renderer::init, NULL );

    //Rotate camera around z axis
    struct timeval t0, t1, tdiff;
    double dt;
    gettimeofday(&t0, NULL);
    t1 = t0;

    double angle = 0;
    while (renderer::active)
    {
        t0 = t1;
        gettimeofday(&t1, NULL);
        timersub(&t1, &t0, &tdiff);
        dt = tdiff.tv_sec + tdiff.tv_usec / 1000000.0;
        angle += dt * 3.14;

	    renderer::view_matrix = glm::lookAt(
			glm::vec3(6 * sin(angle), 6 * cos(angle), 3 * sin(angle / 4)), //Eye
			glm::vec3(0, 0, 0), //Center
			glm::vec3(0, 0, 1)  //Head orientation
		);

        //Delay
        struct timespec sleeptime;
        sleeptime.tv_sec = 0;
        sleeptime.tv_nsec = 10 * 1000 * 1000; //10ms
        nanosleep(&sleeptime, NULL);
    }

	//Wait for renderer to finish
    pthread_join( renderer_thread, NULL );
}