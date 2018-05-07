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
bool renderer::active = false;
bool renderer::ready = false;
bool renderer::error = false;

//Window
GLFWwindow *renderer::window;
int renderer::window_width = 1024;
int renderer::window_height = 768;

//Shader files
std::string renderer::vertex_shader_file = "shaders/vertex.glsl";
std::string renderer::fragment_shader_file = "shaders/fragment.glsl";

//Camera matrix
glm::mat4 renderer::view_matrix;

//Projection matrix
glm::mat4 renderer::projection_matrix;

//Mouse
double renderer::mouse_x, renderer::mouse_y;

//Utilities
void (*renderer::on_init_callback)() = nullptr;
void (*renderer::on_render_callback)() = nullptr;

//Text file loader - used for shader loading
static int load_txt_file(std::string &content, std::string filename)
{
	std::ifstream file(filename, std::ios::in);
	if (file.is_open())
	{
		std::stringstream sstr;
		sstr << file.rdbuf();
		content = sstr.str();
		file.close();
	}
	else
	{
		std::cerr << "Cannot read file '" << filename << "'\n";
		return 1;
	}
	return 0;
}

//Load and compile single shader and return GL handle
static int load_shader(GLuint &shader, std::string filename, GLenum type)
{
	//Create shader
	shader = glCreateShader(type);

	//Get source code
	std::string src;
	const char *csrc;
	if (load_txt_file(src, filename))
		return 1;
	csrc = src.c_str();
	glShaderSource(shader, 1, &csrc, NULL);

	//Compilation results
	int loglength = 0;
	GLint result = GL_FALSE;

	//Compile shader
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglength);
	if (loglength > 0)
	{
		std::vector <char> mesg(loglength + 1);
		glGetShaderInfoLog(shader, loglength, NULL, &mesg[0]);
		std::cerr << ">\t" << &mesg[0] << "\n";
	}

	return 0;
}

//Shader loader
static int load_shaders(GLuint *program_id, std::string vertex_shader_file, std::string fragment_shader_file)
{
	GLuint vertex_shader_id; //= glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id;// = glCreateShader(GL_FRAGMENT_SHADER);

	load_shader(vertex_shader_id, "shaders/vertex.glsl", GL_VERTEX_SHADER);
	load_shader(fragment_shader_id, "shaders/fragment.glsl", GL_FRAGMENT_SHADER);

	//Create program
	std::cerr << "Creating program...\n";
	*program_id = glCreateProgram();
	glAttachShader(*program_id, vertex_shader_id);
	glAttachShader(*program_id, fragment_shader_id);
	glLinkProgram(*program_id);

	//Compilation and linking results
	int loglength = 0;
	GLint result = GL_FALSE;

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
static void render_loop(GLFWwindow *window, GLuint program_id)
{
	glm::mat4 locked_view_matrix;
	glm::mat4 camera_matrix;
	glm::mat4 model_matrix;

	//TEMP
	Voxel voxa( 0, 0, 0 );
	Voxel voxb( 0, 0, 1 );
	Voxel voxc( 0, 1, 0 );
	Voxel voxd( 1, 0, 0 );
	Model monkey("models/uni.obj", "models/uni.png");

	//Get GLSL handles
	GLuint glsl_model_matrix_id = glGetUniformLocation(program_id, "mat_model");
	GLuint glsl_view_matrix_id = glGetUniformLocation(program_id, "mat_view");
	GLuint glsl_projection_matrix_id = glGetUniformLocation(program_id, "mat_projection");
	GLuint glsl_texture_uniform_id = glGetUniformLocation(program_id, "texture_sampler");

	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	//Use default shaders
	glUseProgram(program_id);

	//Render loop
	while (renderer::active && glfwWindowShouldClose(window) == 0)
	{
		//This has to be locked during rendering otherwise blinking may appear
		locked_view_matrix = renderer::view_matrix;

		//Update mouse pos
		glfwGetCursorPos(renderer::window, &renderer::mouse_x, &renderer::mouse_y);

		//Clear GL buffers
		glClearColor( 0.1, 0.1, 0.1, 0.0 );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Apply view and projection matrices
		glUniformMatrix4fv(glsl_view_matrix_id, 1, GL_FALSE, &locked_view_matrix[0][0]);
		glUniformMatrix4fv(glsl_projection_matrix_id, 1, GL_FALSE, &renderer::projection_matrix[0][0]);


		//Render whole map
		//voxa.draw(program_id, camera_matrix);
		//voxb.draw(model_matrix_id);
		//voxc.draw(program_id, camera_matrix);
		//voxd.draw(model_matrix_id);
		

		//Swap buffers
		glm::mat4 aa(1.0);
		glUniformMatrix4fv(glsl_model_matrix_id, 1, GL_FALSE, &aa[0][0]);
		monkey.draw(glsl_texture_uniform_id);

		glfwSwapBuffers(renderer::window);

		//Poll events
		glfwPollEvents();
	}
}

//Renderer init
void renderer::init()
{
	renderer::active = true;

	//GLFW init
	if (!glfwInit())
	{
		std::cerr << "GLFW init failed\n";
		renderer::error = true;
		renderer::active = false;
	}

	//Window params
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Window creation
	renderer::window = glfwCreateWindow(renderer::window_width, renderer::window_height, "Not only voxels, but it's already called 'voxels'...", NULL, NULL);
	if (renderer::window == NULL)
	{
		std::cout << "Cannot create window\n";
		glfwTerminate();
		renderer::error = true;
		renderer::active = false;
	}

	//GLEW init
	glfwMakeContextCurrent(renderer::window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW init failed\n";
		glfwTerminate();
		renderer::error = true;
		renderer::active = false;
	}

	//Load shaders
	GLuint program_id;
	if (load_shaders(&program_id, renderer::vertex_shader_file, renderer::fragment_shader_file))
	{
		glfwTerminate();
		renderer::error = true;
		renderer::active = false;
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

	//External config
	if ( renderer::on_init_callback != nullptr ) renderer::on_init_callback( );
	
	//Start rendering
	renderer::ready = true;
	render_loop(renderer::window, program_id);   

	//Cleanup
	//TODO
	glfwTerminate();

	renderer::active = false;
	renderer::ready = false;
}