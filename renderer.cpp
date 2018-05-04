#include "renderer.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

//Shader files
const char *vertex_shader_file = "shaders/vertex.glsl";
const char *fragment_shader_file = "shaders/fragment.glsl";

//State variables
bool renderer_active = true;
bool renderer_error = false;

//Shader loader
static int load_shaders(GLuint *program_id, const char *vertex_shader_file, const char *fragment_shader_file)
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
static void renderer_loop(GLFWwindow *window)
{
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
}

//Renderer init
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

	//Load shaders
	GLuint program_id;
	if (load_shaders(&program_id, vertex_shader_file, fragment_shader_file))
	{
		glfwTerminate();
		renderer_error = true;
        renderer_active = false;
        return NULL;
	}

    //Input mode
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	
	//Start rendering
	renderer_loop(window);   

    return NULL;
}