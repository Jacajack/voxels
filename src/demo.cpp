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
	
	//Win
	GLuint vertex_array_ida;
	glGenVertexArrays(1, &vertex_array_ida);
	glBindVertexArray(vertex_array_ida);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	//Otherwin
	otherwin.use( );
	//Create vertex array
	GLuint vertex_array_id;
	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


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


	win.use( );
	GLint dims[4] = {0};
	glGetIntegerv(GL_VIEWPORT, dims);
	GLint fbWidth = dims[2];
	GLint fbHeight = dims[3];
	std::cout << fbWidth << "\t" << fbHeight << std::endl;

	//lobor::Framebuffer fb( 1024, 768, 2 );
	//fb.use(  );
	//glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	//Some serious manly C code here
	GLuint fb;
	glGenFramebuffers( 1, &fb );

	//Game loop
	do
	{
		//fb.use( );
		win.use( );
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		shader.use( );

		//Default FB gets cleared
		glClearColor( 0.1, 0.1, 0.1, 0.0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		//Unicorn lands in my FB
		glBindFramebuffer( GL_FRAMEBUFFER, fb );
		unicorn.draw( shader );
		
		//We now copy the unicorn back to default FB
		glBindFramebuffer( GL_READ_FRAMEBUFFER, fb );
		glBlitFramebuffer( 0, 0, 1024, 768, 0, 0, 1024, 768, GL_COLOR_BUFFER_BIT, GL_NEAREST );

		//fb.blit_to( 0, 0, 0, 1024, 768, 0, 0, 1024, 768, GL_COLOR_BUFFER_BIT, GL_NEAREST );
		
		win.swap_buffers( );
		
		//fb.use( );
		//glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		otherwin.use( );
		othershader.use( );
		glClearColor( 0.1, 0.1, 0.1, 0.0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		unicorn.draw( shader );
		
		//glBlitFramebuffer( 0, 0, 1024/4, 768/4, 1024/4, 768/4, 1024/2, 768/2, GL_COLOR_BUFFER_BIT, GL_NEAREST );
		otherwin.swap_buffers( );
		
		
		glfwPollEvents( );
		//std::cout << "a\n";
	}
	while ( !win.should_close( ) && !otherwin.should_close( ) );
	
	//Destroy
	lobor::destroy( );

	return 0;
}