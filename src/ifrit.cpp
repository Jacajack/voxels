#include "ifrit.hpp"
#include <iostream>
#include <string>
#include <stdarg.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Ifrit includes
#include "core/shader.hpp"

//Ifrit structs
struct ifrit::WindowInformation ifrit::win;
struct ifrit::CoreStatus ifrit::status;

//Logger function
void __attribute__( ( format ( printf, 2, 3 ) ) ) ifrit::log( int level, const char *format, ... )
{
	//Select color
	switch ( level )
	{
		case IFRIT_NOTICE:
			std::printf( "ifrit: \x1b[37m" );
			break;

		case IFRIT_WARN:
			std::printf( "ifrit: \x1b[33m" );
			break;

		case IFRIT_DEBUG:
			std::printf( "ifrit_dbg: \x1b[32m" );
			break;

		case IFRIT_ERROR:
		default:
			std::printf( "ifrit: \x1b[31m" );
			break;
	}

	//Print the message
	va_list ap;
	va_start( ap, format );
	std::vprintf( format, ap );
	va_end( ap );
	std::printf( "\x1b[0m\n" );
}

//Log function wrapper
void __attribute__( ( format ( printf, 1, 2 ) ) ) ifrit::log( const char *format, ... )
{
	va_list ap;
	va_start( ap, format );
	ifrit::log( IFRIT_NOTICE, format, ap );
	va_end( ap );
}

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
		ifrit::log( IFRIT_ERROR, "GLFW init failed" );
		//std::cerr << "ifirt: GLFW init failed\n";
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