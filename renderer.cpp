#include "renderer.hpp"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

bool renderer_active = true;
bool renderer_error = false;

void *renderer_init(void *data)
{
    GLFWwindow *window;

    //GLFW init
	if (!glfwInit())
	{
		std::cerr << "GLFW init failed\n";
		renderer_error = true;
        renderer_active = false;
        return NULL;
	}

	//Window params
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Window creation
	window = glfwCreateWindow(1024, 768, "Crapgame", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Cannot create window\n";
		glfwTerminate();
		renderer_error = true;
        renderer_active = false;
        return NULL;
	}

	//GLEW init
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW init failed\n";
        glfwTerminate();
		renderer_error = true;
        renderer_active = false;
        return NULL;
	}

    //Input mode
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    //Render loop
    while (renderer_active 
        && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS 
        && glfwWindowShouldClose(window) == 0)
    {
        //Clear GL buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		//Swap buffers
		glfwSwapBuffers(window);

		//Poll events
		glfwPollEvents();
    }

    return NULL;
}