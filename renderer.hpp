#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace renderer
{
	//State variables
	extern bool ready;
	extern bool active;
	extern bool error;

	//Window
	extern GLFWwindow *window;
	extern int window_width;
	extern int window_height;

	//Shader files
	extern std::string vertex_shader_file;
	extern std::string fragment_shader_file;

	//View matrix
	extern glm::mat4 view_matrix;
	extern glm::mat4 projection_matrix;

	//Mouse position
	extern double mouse_x;
	extern double mouse_y;

	//Utilities
	extern void (*on_init_callback)();
	extern void (*on_render_callback)();

	//Renderer thread entry point
	extern void init();
};

#endif