#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "shaderset.hpp"

namespace renderer
{
	//All data needed for object rendering
	struct RenderContext
	{
		glm::mat4 view_matrix;
		glm::mat4 projection_matrix;
		glm::mat4 sun_view;
		glm::mat4 depth_bias;
		ShaderSet *shaderset = NULL;

		bool force_shaders;
	};

	//State variables
	extern bool ready;
	extern bool active;
	extern bool error;

	//Window
	extern GLFWwindow *window;
	extern int window_width;
	extern int window_height;

	//View matrix
	extern glm::mat4 view_matrix;
	extern glm::mat4 projection_matrix;
	
	//TEMP sun
	extern glm::vec3 sun_pos;
	extern glm::vec3 sun_dir;

	//Mouse position
	extern double mouse_x;
	extern double mouse_y;

	//Utilities
	extern void (*on_init_callback)();
	extern void (*on_render_callback)();

	//Renderer thread entry point
	extern void init();
	extern void lock( );
	extern void unlock( );
};

#endif