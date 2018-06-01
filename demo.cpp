#include <iostream>
#include <lobor/lobor.hpp>


void controls( lobor::Window &win, glm::mat4 &vmat )
{
	static glm::vec3 pos( 0 );

	const double mspeed = 0.005;
	const float speed = 3;

	static double yaw = 0, pitch = 0;

	static double lastt = glfwGetTime( );
	double dt = glfwGetTime( ) - lastt;
	lastt = glfwGetTime( );

	glfwMakeContextCurrent( win );
	
	double cdx, cdy;
	glfwGetCursorPos( win, &cdx, &cdy );
	cdx -= win.get_width( ) / 2.0;
	cdy -= win.get_height( ) / 2.0;
	yaw += mspeed * cdx;
	pitch += mspeed * cdy;

	glm::vec3 dir(
		cos( yaw ) * cos( pitch ),
		sin( pitch ),
		sin( yaw ) * cos( pitch )
	);

	if ( glfwGetKey( win, GLFW_KEY_W ) == GLFW_PRESS )
		pos += float( speed * dt ) * dir;
	if ( glfwGetKey( win, GLFW_KEY_S ) == GLFW_PRESS )
		pos -= float( speed * dt ) * dir;

	vmat = glm::lookAt( pos, pos + dir, glm::vec3( 0, 1, 0 ) );

	glfwSetCursorPos( win, win.get_width( ) / 2.0, win.get_height( ) / 2.0 );
}

int main( int argc, char **argv )
{	
	//Create window
	lobor::init_glfw( );
	lobor::Window win( 1024, 768, "Default view", NULL );
	win.use( );
	lobor::init_glew( );
	glfwSetInputMode(win, GLFW_STICKY_KEYS, GL_TRUE);

	std::cout << sizeof(float) << std::endl;
	
	//Win
	GLuint vertex_array_ida;
	glGenVertexArrays(1, &vertex_array_ida);
	glBindVertexArray(vertex_array_ida);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	LOBOR_CHECK_GL_ERROR;

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

	LOBOR_CHECK_GL_ERROR;


	lobor::TexturedModel unicorn( "models/uni.obj", "models/uni.png" );

	LOBOR_CHECK_GL_ERROR;

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

	LOBOR_CHECK_GL_ERROR;

	win.use( );
	GLint dims[4] = {0};
	glGetIntegerv(GL_VIEWPORT, dims);
	GLint fbWidth = dims[2];
	GLint fbHeight = dims[3];
	std::cout << fbWidth << "\t" << fbHeight << std::endl;

	//Some serious manly C code here
	win.use( );
	GLuint fb;
	glGenFramebuffers( 1, &fb );
	LOBOR_CHECK_GL_ERROR;
	glBindFramebuffer( GL_FRAMEBUFFER, fb );
	LOBOR_CHECK_GL_ERROR;

	
	GLuint t, u;
	glGenTextures( 1, &t );
	glBindTexture( GL_TEXTURE_2D, t );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t, 0 );
	
	glGenTextures( 1, &u );
	glBindTexture( GL_TEXTURE_2D, u );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, u, 0 );
	LOBOR_CHECK_GL_ERROR;

	GLuint rbo;
	glGenRenderbuffers( 1, &rbo );
	glBindRenderbuffer( GL_RENDERBUFFER, rbo );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 768 );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo );
	LOBOR_CHECK_GL_ERROR;
	
	const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glBindFramebuffer( GL_FRAMEBUFFER, fb );
	LOBOR_CHECK_GL_ERROR;
	glDrawBuffers( 2, draw_buffers );
	LOBOR_CHECK_GL_ERROR;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb);
	if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    	printf("There is a problem with the FBO\n");

	lobor::Texturepeek peek( 1024/2, 768/2, "peek", u, win );

	

	//Game loop
	do
	{
		//fb.use( );
		win.use( );
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		LOBOR_CHECK_GL_ERROR;
		shader.use( );

		//Default FB gets cleared
		glClearColor( 0.1, 0.1, 0.1, 0.0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		//Unicorn lands in my FB
		glBindFramebuffer( GL_FRAMEBUFFER, fb );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		LOBOR_CHECK_GL_ERROR;
		unicorn.draw( shader );
		
		//We now copy the unicorn back to default FB
		glBindFramebuffer( GL_READ_FRAMEBUFFER, fb );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		glBlitFramebuffer( 0, 0, 1024, 768, 0, 0, 1024, 768, GL_COLOR_BUFFER_BIT, GL_NEAREST );

		//fb.blit_to( 0, 0, 0, 1024, 768, 0, 0, 1024, 768, GL_COLOR_BUFFER_BIT, GL_NEAREST );
		
		//win.swap_buffers( );


		controls( win, mat_view );
		glUniformMatrix4fv( shader.uniform( "mat_view" ), 1, GL_FALSE, &mat_view[0][0] );
		
		

		win.swap_buffers( );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		peek.update( );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		LOBOR_CHECK_GL_ERROR;

		glfwPollEvents( );
	}
	while ( !win.should_close( ) && !peek.should_close( ) );
	
	//Destroy
	lobor::destroy( );

	return 0;
}