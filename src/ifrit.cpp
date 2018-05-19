#include "ifrit.hpp"
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Ifrit structs
struct ifrit::WindowInformation ifrit::win;
struct ifrit::CoreStatus ifrit::status;

//The renderer
void ifrit::update( )
{
	if ( ifrit::status.active == false ) return;


	//Update buffers and poll events
	glfwSwapBuffers( ifrit::win.window );
	glfwPollEvents( );
	ifrit::status.active = !glfwWindowShouldClose( ifrit::win.window );
}

//3D engine init routine
int ifrit::init( int resx, int resy, std::string title )
{
	//GLFW init
	if ( !glfwInit( ) )
	{
		std::cerr << "ifirt: GLFW init failed\n";
		return 1;
	}

	//Window params
	//Antialiasing, OpenGL 3.3
	glfwWindowHint( GLFW_SAMPLES, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	//Create the window
	ifrit::win.resx = resx;
	ifrit::win.resy = resy;
	ifrit::win.title = title;
	ifrit::win.window = glfwCreateWindow( ifrit::win.resx, ifrit::win.resy, ifrit::win.title.c_str( ), NULL, NULL );
	if ( ifrit::win.window == NULL )
	{
		std::cerr << "ifrit: Window creation failed\n";
		glfwTerminate( );
		return 1;
	}
	glfwMakeContextCurrent( ifrit::win.window );

	//GLEW init
	glewExperimental = true;
	if ( glewInit( ) != GLEW_OK )
	{
		std::cerr << "ifrit: GLEW init failed!\n";
		glfwTerminate( );
		return 1;
	}

	//Input modes
	glfwSetInputMode( ifrit::win.window, GLFW_STICKY_KEYS, GL_TRUE );

	//Consider Ifrit running
	ifrit::status.active = true;

	return 0;
};

//Cleanup function
void ifrit::destroy( )
{
	glfwTerminate( );
}