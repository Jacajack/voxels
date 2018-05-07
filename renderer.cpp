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
#include "shaderset.hpp"

//State variables
bool renderer::active = false;
bool renderer::ready = false;
bool renderer::error = false;

//Window
GLFWwindow *renderer::window;
int renderer::window_width = 1024;
int renderer::window_height = 768;

//Camera matrix
glm::mat4 renderer::view_matrix;

//Projection matrix
glm::mat4 renderer::projection_matrix;

//Mouse
double renderer::mouse_x, renderer::mouse_y;

//Utilities
void (*renderer::on_init_callback)() = nullptr;
void (*renderer::on_render_callback)() = nullptr;

//Render loop
static void render_loop(GLFWwindow *window)
{
	glm::mat4 locked_view_matrix;
	glm::mat4 camera_matrix;
	glm::mat4 model_matrix;

	ShaderSet *prog = new ShaderSet(
		{
			{"shaders/vertex.glsl", GL_VERTEX_SHADER},
			{"shaders/fragment.glsl", GL_FRAGMENT_SHADER}
		}, 
		{
			"mat_model",
			"mat_view",
			"mat_projection",
			"texture_sampler",
			"model_tint"
		}
	);

	//TEMP
	Model monkey(*prog, "models/uni.obj", glm::vec3(1.0, 1.0, 0.0));

	//Get GLSL handles


	GLuint glsl_model_matrix_id = prog->uniforms["mat_model"]; //glGetUniformLocation(program_id, "mat_model");
	GLuint glsl_view_matrix_id = prog->uniforms["mat_view"]; //glGetUniformLocation(program_id, "mat_view");
	GLuint glsl_projection_matrix_id = prog->uniforms["mat_projection"]; //glGetUniformLocation(program_id, "mat_projection");

	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	//Use default shaders
	//glUseProgram(program_id);
	prog->use( );

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
		monkey.draw();

		glfwSwapBuffers(renderer::window);

		//Poll events
		glfwPollEvents();
	}

	delete prog;
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
	render_loop(renderer::window);   

	//Cleanup
	//TODO
	glfwTerminate();

	renderer::active = false;
	renderer::ready = false;
}