#include "ifrit.hpp"
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
	ifrit::window.resx = resx;
	ifrit::window.resy = resy;
	ifrit::window.title = title;
	ifrit::window.win = glfwCreateWindow( ifrit::window.resx, ifrit::window.resy, ifrit::window.title.c_str( ), NULL, NULL );
	if ( ifrit::window.win == NULL )
	{
		std::cerr << "ifrit: Window creation failed\n";
		glfwTerminate( );
		return 1;
	}
	glfwMakeContextCurrent( ifrit::window.win );

	//GLEW init
	glewExperimental = true;
	if ( glewInit( ) != GLEW_OK )
	{
		std::cerr << "ifrit: GLEW init failed!\n";
		glfwTerminate( );
		return 1;
	}

	return 0;
};

//Cleanup function
void ifrit::destroy( )
{
	glfwTerminate( );
}