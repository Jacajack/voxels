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
#include "actor.hpp"

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

glm::vec3 renderer::sun_pos, renderer::sun_dir;

//Mouse
double renderer::mouse_x, renderer::mouse_y;

//Utilities
void (*renderer::on_init_callback)() = nullptr;
void (*renderer::on_render_callback)() = nullptr;

//Render loop
static bool render_lock_rq = false, render_lock_ack = false;
static void render_loop(GLFWwindow *window)
{
	glm::mat4 camera_matrix;
	glm::mat4 model_matrix;

	std::initializer_list <std::string> shared_uniforms = 
	{
		"mat_model",
		"mat_view",
		"mat_projection",
		"texture_sampler",
		"model_tint",
		"sun_view",
		"depth_bias",
		"shadowMap",
	};

	
	ShaderSet *prog = new ShaderSet(
		{
			{"shaders/shadows/vertex.glsl", GL_VERTEX_SHADER},
			{"shaders/shadows/fragment.glsl", GL_FRAGMENT_SHADER}
		}, 
		shared_uniforms
	);
	
	

	ShaderSet *depth_prog = new ShaderSet(
		{
			{"shaders/shadows/depth_v.glsl", GL_VERTEX_SHADER},
			{"shaders/shadows/depth_f.glsl", GL_FRAGMENT_SHADER}
		},
		shared_uniforms
	);

	//ShaderSet *prog = depth_prog;

	//TEMP
	//Model monkey(*prog, "models/uni.obj", glm::vec3(1.0, 1.0, 0.0));
	
	Actor unicorn(
		{
			new Model(*prog, "models/uni.obj",  "models/uni.png" /*/  glm::vec3(1.0, 0.5, 0.7) */ )
		}
	);
	unicorn.rotation = glm::vec3(M_PI_2, M_PI, M_PI_2 );
	unicorn.position.y = 2.5;
	unicorn.position.x = 10;
	unicorn.scale = glm::vec3(1);

	Actor tree(
		{
			new Model( *prog, "models/tree-top.obj", glm::vec3( 0.6, 1.0, 0.6 ) ),
			new Model( *prog, "models/tree-trunk.obj", glm::vec3( 0.448, 0.228, 0.162 ) )
		}
	);
	tree.rotation.y = M_PI;

	Actor ground(
		{
			new Model( *prog, "models/ground.obj", glm::vec3( 1, 1, 1 ) ),
		}
	);
	ground.scale = glm::vec3( 10, 1, 10 );


	std::vector <Actor*> forest;
	for ( int i = 0; i < 64; i++ )
	{
		Actor *t = new Actor( tree );

		t->position.x = (rand() % 1000 / 1000.0) * 100.0 - 50;
		t->position.z = (rand() % 1000 / 1000.0) * 100.0 - 50;
		t->rotation.y = (rand() % 1000 / 1000.0) * 2 * M_PI;
		t->scale = glm::vec3(0.8 + 0.4 * (rand() % 1000 / 1000.0) );

		forest.push_back(t);
		//t->position = glm::vec3( 5, 0, 0);
	}

	renderer::RenderContext context;

	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	//Shadow buffer
	

	GLuint fbuffer;
	glGenFramebuffers(1, &fbuffer);	
	GLuint depthtex;
	glGenTextures( 1, &depthtex );
	glBindTexture( GL_TEXTURE_2D, depthtex );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RG32F, 4096, 4096, 0, GL_RG, GL_FLOAT, 0 );
	//glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0 );
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->texture_width, this->texture_height, 0, this->texture_format, GL_UNSIGNED_BYTE, this->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	float border_color[] = {1, 1, 1, 1};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

	glBindFramebuffer( GL_FRAMEBUFFER, fbuffer );	
	glBindTexture( GL_TEXTURE_2D, depthtex );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, depthtex, 0 );


	GLuint rendbuf;
	glGenRenderbuffers(1, &rendbuf);
	glBindRenderbuffer( GL_RENDERBUFFER, rendbuf );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 4096, 4096 );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rendbuf );

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthtex, 0 );
	//glDrawBuffer( GL_NONE );
	//glReadBuffer( GL_NONE );


	//Render loop
	while (renderer::active && glfwWindowShouldClose(window) == 0)
	{
		//Render loop spinlock
		if ( render_lock_rq ) render_lock_ack = 1;
		while ( render_lock_rq );
		render_lock_ack = false;

		//This has to be locked during rendering otherwise blinking may appear
		context.view_matrix = renderer::view_matrix;
		context.projection_matrix = renderer::projection_matrix;
		//context.sun_view = /*glm::ortho <float> (-20, 20, -20, 20, -20, 20 ) * */  
		/*glm::perspective( glm::radians( 90.0 ), 1.0, 0.1, 100.0 );*/

		glm::vec3 sun_p = renderer::sun_pos;
		glm::vec3 sun_d = renderer::sun_dir;

		context.sun_view =
		glm::ortho <float> (-20, 20, -20, 20, 1, 50 ) * 
		glm::lookAt( 
			sun_p,
			sun_p + sun_d,
			glm::vec3( 0, 1, 0 )
		);
		context.depth_bias = glm::mat4(
			0.5, 0.0, 0.0, 0.0, 
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
		);

		//Update mouse pos
		glfwGetCursorPos(renderer::window, &renderer::mouse_x, &renderer::mouse_y);





		//We're gonna generate the shadows
		context.shaderset = depth_prog;
		context.force_shaders = false;
		depth_prog->use();

		glViewport(0, 0, 4096, 4096 );
		
		glBindFramebuffer( GL_FRAMEBUFFER, fbuffer );

		//glViewport(0, 0, renderer::window_width, renderer::window_height );
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glEnable( GL_CULL_FACE );
		//glCullFace( GL_BACK );
		glClearColor( 0.0, 0, 0.0, 0.0 );
		glClearDepth( 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		//glEnable(GL_POLYGON_OFFSET_FILL);
		//glPolygonOffset(1.0f, 1.0f);
		
		glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

		//Shadow rendering
		unicorn.draw(context);
		tree.draw(context);
		ground.draw(context);
		for ( int i = 0; i < forest.size(); i++ )
			forest[i]->draw(context );





///*


		
		//Proper render
		context.shaderset = prog;
		context.force_shaders = false;
		prog->use();
		glViewport(0, 0, renderer::window_width, renderer::window_height );
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glCullFace( GL_BACK );
		glDisable( GL_CULL_FACE );
		glClearColor( 0.1, 0.1, 0.1, 0.0 );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glDisable(GL_POLYGON_OFFSET_FILL);
		//glPolygonOffset( 0.0, 0 );
		
		glActiveTexture( GL_TEXTURE1 );
		glBindTexture( GL_TEXTURE_2D, depthtex );
		glUniform1i( prog->uniforms["shadowMap"], 1 );
		

		//Shadow rendering
		unicorn.draw(context);
		tree.draw(context);
		ground.draw(context);
		for ( int i = 0; i < forest.size(); i++ )
			forest[i]->draw(context );
		

//*/		


		

		//Poll events and swap buffers
		glfwSwapBuffers(renderer::window);
		glfwPollEvents();
	}

	//delete prog;
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

void renderer::lock( )
{
	if ( render_lock_ack == false )
	{
		render_lock_rq = true;
		while ( render_lock_ack == false );
	}
}

void renderer::unlock( )
{
	render_lock_rq = false;
}