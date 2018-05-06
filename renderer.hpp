#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace renderer
{
	//State variables
	extern bool active;
	extern bool error;

	//Window parameters
	extern int window_width;
	extern int window_height;

	//Shader files
	extern std::string vertex_shader_file;
	extern std::string fragment_shader_file;

	//View matrix
	extern glm::mat4 view_matrix;
	extern glm::mat4 projection_matrix;

	//Renderer thread entry point
	extern void *init(void *data);
};

#endif