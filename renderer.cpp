#include "renderer.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

//TEMP
#include "voxel.hpp"
#include "model.hpp"

//State variables
bool renderer::active = true;
bool renderer::error = false;

//Window properties
int renderer::window_width = 1024;
int renderer::window_height = 768;

//Shader files
std::string renderer::vertex_shader_file = "shaders/vertex.glsl";
std::string renderer::fragment_shader_file = "shaders/fragment.glsl";

//Camera matrix
glm::mat4 renderer::view_matrix;

//Projection matrix
glm::mat4 renderer::projection_matrix;

//Shader loader
static int load_shaders(GLuint *program_id, std::string vertex_shader_file, std::string fragment_shader_file)
{
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	//Read vertex shader
	std::string vertex_shader_str;
	std::ifstream vertex_shader_stream(vertex_shader_file, std::ios::in);
	if (vertex_shader_stream.is_open())
	{
		std::stringstream sstr;
		sstr << vertex_shader_stream.rdbuf();
		vertex_shader_str = sstr.str();
		vertex_shader_stream.close();
	}
	else
	{
		std::cout << "Cannot read vertex shader - " << vertex_shader_file << "\n";
		return 1;
	}

	//Read fragment shader
	std::string fragment_shader_str;
	std::ifstream fragment_shader_stream(fragment_shader_file, std::ios::in);
	if (fragment_shader_stream.is_open())
	{
		std::stringstream sstr;
		sstr << fragment_shader_stream.rdbuf();
		fragment_shader_str = sstr.str();
		fragment_shader_stream.close();
	}
	else
	{
		std::cout << "Cannot read fragment shader - " << fragment_shader_file << "\n";
		return 1;
	}


	//Compilation and linking results
	int loglength = 0;
	GLint result = GL_FALSE;
	

	//Compile vertex shader	
	std::cout << "Compiling vertex shader...\n";
	const char *vertex_shader_code = vertex_shader_str.c_str();
	glShaderSource(vertex_shader_id, 1, &vertex_shader_code, NULL);
	glCompileShader(vertex_shader_id);

	//Check vertex shader
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &loglength);
	if (loglength > 0)
	{
		std::vector <char> mesg(loglength + 1);
		glGetShaderInfoLog(vertex_shader_id, loglength, NULL, &mesg[0]);
		std::cout << ">\t" << &mesg[0] << "\n";
	}

	//Compile fragment shader	
	std::cout << "Compiling fragment shader...\n";
	const char *fragment_shader_code = fragment_shader_str.c_str();
	glShaderSource(fragment_shader_id, 1, &fragment_shader_code, NULL);
	glCompileShader(fragment_shader_id);

	//Check fragment shader
	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &loglength);
	if (loglength > 0)
	{
		std::vector <char> mesg(loglength + 1);
		glGetShaderInfoLog(fragment_shader_id, loglength, NULL, &mesg[0]);
		std::cout << ">\t" << &mesg[0] << "\n";
	}

	//Create program
	std::cout << "Creating program...\n";
	*program_id = glCreateProgram();
	glAttachShader(*program_id, vertex_shader_id);
	glAttachShader(*program_id, fragment_shader_id);
	glLinkProgram(*program_id);

	//Check program
	glGetProgramiv(*program_id, GL_LINK_STATUS, &result);
	glGetProgramiv(*program_id, GL_INFO_LOG_LENGTH, &loglength);
	if (loglength > 0)
	{
		std::vector <char> mesg(loglength + 1);
		glGetProgramInfoLog(*program_id, loglength, NULL, &mesg[0]);
		std::cout << ">\t" << &mesg[0] << "\n";
	}

	glDetachShader(*program_id, vertex_shader_id);
	glDetachShader(*program_id, fragment_shader_id);
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	//Succesful exit
	return 0;
}

//Render loop
static void renderer_loop(GLFWwindow *window, GLuint program_id)
{
	glm::mat4 camera_matrix;
	glm::mat4 model_matrix;

	//TEMP
	Voxel voxa( 0, 0, 0 );
	Voxel voxb( 0, 0, 1 );
	Voxel voxc( 0, 1, 0 );
	Voxel voxd( 1, 0, 0 );
	Model monkey("models/face.obj", "models/face.png");

	//Get GLSL handles
	GLuint model_matrix_id = glGetUniformLocation(program_id, "model");
	GLuint camera_matrix_id = glGetUniformLocation(program_id, "camera");
	GLuint texture_uniform_id = glGetUniformLocation(program_id, "texture_sampler");

	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	//Render loop
	while (renderer::active 
		&& glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS 
		&& glfwWindowShouldClose(window) == 0)
	{
		//Clear GL buffers
		glClearColor( 0.1, 0.1, 0.1, 0.0 );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Use shaders
		glUseProgram(program_id);

		//Apply view and projection matrices
		camera_matrix = renderer::projection_matrix * renderer::view_matrix;
		glUniformMatrix4fv(camera_matrix_id, 1, GL_FALSE, &camera_matrix[0][0]);


		//Render whole map
		//voxa.draw(program_id, camera_matrix);
		//voxb.draw(model_matrix_id);
		//voxc.draw(program_id, camera_matrix);
		//voxd.draw(model_matrix_id);
		

		//Swap buffers
		glm::mat4 aa(1.0);
		glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, &aa[0][0]);
		monkey.draw(texture_uniform_id);

		glfwSwapBuffers(window);

		//Poll events
		glfwPollEvents();
	}
}

//Renderer init
void *renderer::renderer_init(void *data)
{
	GLFWwindow *window;

	//GLFW init
	if (!glfwInit())
	{
		std::cerr << "GLFW init failed\n";
		renderer::error = true;
		renderer::active = false;
		return NULL;
	}

	//Window params
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Window creation
	window = glfwCreateWindow(renderer::window_width, renderer::window_height, "Not only voxels, but it's already called 'voxels'...", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Cannot create window\n";
		glfwTerminate();
		renderer::error = true;
		renderer::active = false;
		return NULL;
	}

	//GLEW init
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW init failed\n";
		glfwTerminate();
		renderer::error = true;
		renderer::active = false;
		return NULL;
	}

	//Load shaders
	GLuint program_id;
	if (load_shaders(&program_id, renderer::vertex_shader_file, renderer::fragment_shader_file))
	{
		glfwTerminate();
		renderer::error = true;
		renderer::active = false;
		return NULL;
	}

	//Default view matrix
	renderer::view_matrix = glm::lookAt(
			glm::vec3(4, 4, 3), //Eye
			glm::vec3(0, 0, 0), //Center
			glm::vec3(0, 0, 1)  //Head orientation
		);

	//Default projection matrix
	renderer::projection_matrix = glm::perspective(
			glm::radians( 45.0 ), //FoV
			(double) renderer::window_width / renderer::window_height, //Aspect ratio
			0.1, //Near clipping plane
			100.0 //Far clipping plane
		);

	//Enable Z-buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	//Create vertex array
	GLuint vertex_array_id;
	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);

	//Input mode
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	
	//Start rendering
	renderer_loop(window, program_id);   

	//Cleanup
	//TODO

	renderer::active = false;
	return NULL;
}