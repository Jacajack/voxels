#include <iostream>
#include "lobor.hpp"


int main( int argc, char **argv )
{	
	//Create window
	lobor::init_glfw( );
	lobor::Window win( 1024, 768, "Default view", NULL );
	lobor::Window otherwin( 1024 /2, 768/2, "UV map", win );
	win.use( );
	lobor::init_glew( );

	std::cout << sizeof(float) << std::endl;
	
	GLuint vertex_array_ida;
	glGenVertexArrays(1, &vertex_array_ida);
	glBindVertexArray(vertex_array_ida);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	std::cout << glfwGetCurrentContext( ) << std::endl;
	otherwin.use( );
	std::cout << glfwGetCurrentContext( ) << std::endl;
	

	//Create vertex array
	GLuint vertex_array_id;
	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);


	lobor::Shader shader(
		{
			{"shaders/flat/vertex.glsl", GL_VERTEX_SHADER},
			{"shaders/flat/fragment.glsl", GL_FRAGMENT_SHADER},
		},
		{
			"mat_model",
			"mat_view",
			"mat_projection",
			"model_tint",
			"texture_sampler"
		}
	);

	lobor::Shader othershader(
		{
			{"shaders/zval/vertex.glsl", GL_VERTEX_SHADER},
			{"shaders/zval/fragment.glsl", GL_FRAGMENT_SHADER},
		},
		{
			"mat_model",
			"mat_view",
			"mat_projection",
			"model_tint",
			"texture_sampler"
		}
	);

	lobor::TexturedModel unicorn( "models/uni.obj", "models/uni.png" );

	glm::mat4 mat_model( 1.0 );
	glm::mat4 mat_view = glm::lookAt(
		glm::vec3( 4, 2, 4 ),
		glm::vec3( 0, 0, 0 ),
		glm::vec3( 0, 1, 0 )
	);
	glm::mat4 mat_projection = glm::perspective(
		glm::radians( 90.0 ), //FoV
		(double) 1024.0 / 768.0, //Aspect ratio
		0.1, //Near clipping plane
		20.0 //Far clipping plane
	);
	glm::vec4 model_tint( 1, 0, 1, 1 );
	

	shader.use( );
	glUniform4fv( shader.uniform( "model_tint" ), 1, &model_tint[0] );
	glUniformMatrix4fv( shader.uniform( "mat_view" ), 1, GL_FALSE, &mat_view[0][0] );
	glUniformMatrix4fv( shader.uniform( "mat_projection" ), 1, GL_FALSE, &mat_projection[0][0] );
	glUniformMatrix4fv( shader.uniform( "mat_model" ), 1, GL_FALSE, &mat_model[0][0] );

	othershader.use( );
	glUniform4fv( othershader.uniform( "model_tint" ), 1, &model_tint[0] );
	glUniformMatrix4fv( othershader.uniform( "mat_view" ), 1, GL_FALSE, &mat_view[0][0] );
	glUniformMatrix4fv( othershader.uniform( "mat_projection" ), 1, GL_FALSE, &mat_projection[0][0] );
	glUniformMatrix4fv( othershader.uniform( "mat_model" ), 1, GL_FALSE, &mat_model[0][0] );
	//glUniform1i( shader.uniform( "texture_sampler" ), 0 );




	//Enable Z-buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	


	//Game loop
	do
	{
		win.use( );
		shader.use( );
		glClearColor( 0.1, 0.1, 0.1, 0.0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		unicorn.draw( shader );
		win.swap_buffers( );
		
		otherwin.use( );
		othershader.use( );
		glClearColor( 0.1, 0.1, 0.1, 0.0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		unicorn.draw( shader );
		
		glBlitFramebuffer( 0, 0, 1024/4, 768/4, 1024/4, 768/4, 1024/2, 768/2, GL_COLOR_BUFFER_BIT, GL_NEAREST );
		otherwin.swap_buffers( );
		
		
		glfwPollEvents( );
				
	}
	while ( !win.should_close( ) && !otherwin.should_close( ) );
	
	//Destroy
	lobor::destroy( );

	return 0;
}