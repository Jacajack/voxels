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

struct
{
    //Position and orientation
    float x, y, z;
    float pitch = 0, yaw = 0;

    float speed = 10.0;

    //Movement orders
    bool fd = false, bw = false, rt = false, lt = false;

    //Update function
    void update( double dt )
    {
        //Move forward
        if (fd)
        {
            z += sin(pitch) * speed * dt;
            x += sin(yaw) * cos(pitch) * speed * dt;
            y += cos(yaw) * cos(pitch) * speed * dt;
        }

        //Move backwards
        if (bw)
        {
            z -= sin(pitch) * speed * dt;
            x -= sin(yaw) * cos(pitch) * speed * dt;
            y -= cos(yaw) * cos(pitch) * speed * dt;
        }

        //Move left
        if (lt)
        {
            x -= sin(yaw + M_PI / 2.0 ) * speed * dt;
            y -= cos(yaw + M_PI / 2.0) * speed * dt;
        }

        //Move right
        if (rt)
        {
            x -= sin(yaw - M_PI / 2.0 ) * speed * dt;
            y -= cos(yaw - M_PI / 2.0) * speed * dt;
        }

        yaw = ((double)renderer::mouse_x / renderer::window_width) * M_PI;
        pitch = -(((double)renderer::mouse_y / renderer::window_height) - 0.5) * 3.14; //Note: this can't be full PI, because otherwise camera would flip

        //Calculate position and direction and generate new view matrix
        glm::vec3 pos = glm::vec3(x, y, z);
        glm::vec3 dir = glm::vec3( sin(yaw) * cos(pitch), cos(yaw) * cos(pitch), sin(pitch) );
        renderer::view_matrix = glm::lookAt(
            pos, //Where it is?
            pos + dir, //Where to look?
            glm::vec3(0, 0, 1)  //Where is up?
        );
    }
} camera;


void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        camera.fd = true;
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        camera.fd = false;

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        camera.bw = true;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        camera.bw = false;

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        camera.lt = true;
    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        camera.lt = false;

     if (key == GLFW_KEY_D && action == GLFW_PRESS)
        camera.rt = true;
    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        camera.rt = false;

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        camera.yaw +=  0.314;

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        camera.yaw -=  0.314;

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        camera.pitch +=  0.314;

    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        camera.pitch -=  0.314;

}



int main( int argc, char **argv )
{
    //Start renderer thread
    pthread_t renderer_thread;
    pthread_create( &renderer_thread, NULL, renderer::init, NULL );

    camera.x = 0;
    camera.y = 5;
    camera.z = 0;
 
 
    //Wait for render loop to start
    while (!renderer::ready);

    glfwSetKeyCallback(renderer::window, keyboard_callback);

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

        camera.update(dt);

        //Delay
        struct timespec sleeptime;
        sleeptime.tv_sec = 0;
        sleeptime.tv_nsec = 10 * 1000 * 1000; //10ms
        nanosleep(&sleeptime, NULL);
    }

	//Wait for renderer to finish
    pthread_join( renderer_thread, NULL );
}