#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>

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
    if (action == GLFW_PRESS)
        switch (key)
        {
            case GLFW_KEY_W:
                camera.fd = true;
                break;

            case GLFW_KEY_S:
                camera.bw = true;
                break;

            case GLFW_KEY_A:
                camera.lt = true;
                break;

            case GLFW_KEY_D:
                camera.rt = true;
                break;

            case GLFW_KEY_ESCAPE:
                renderer::active = false;
                break;
        }

    if (action == GLFW_RELEASE)
        switch (key)
        {
            case GLFW_KEY_W:
                camera.fd = false;
                break;

            case GLFW_KEY_S:
                camera.bw = false;
                break;

            case GLFW_KEY_A:
                camera.lt = false;
                break;

            case GLFW_KEY_D:
                camera.rt = false;
                break;
        }
}

void renderer_init()
{
    glfwSetInputMode(renderer::window, GLFW_STICKY_KEYS, GL_TRUE);
	//glfwSetInputMode(renderer::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

int main( int argc, char **argv )
{
    //Start renderer thread
    renderer::on_init_callback = renderer_init;
    std::thread renderer_thread(renderer::init);

    camera.x = 0;
    camera.y = 5;
    camera.z = 0;
    camera.update(0);
 
 
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
    renderer_thread.join();
}