#include "lobor.hpp"
#include <iostream>
#include <string>
#include <stdarg.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Lobor includes
#include "core/shader.hpp"

//Lobor structs
struct lobor::WindowInformation lobor::win;
struct lobor::CoreStatus lobor::status;

//Logger function
void __attribute__( ( format ( printf, 2, 3 ) ) ) lobor::log( int level, const char *format, ... )
{
	//Select color
	switch ( level )
	{
		case LOBOR_NOTICE:
			std::printf( "lobor: \x1b[37m" );
			break;

		case LOBOR_WARN:
			std::printf( "lobor: \x1b[33m" );
			break;

		case LOBOR_DEBUG:
			std::printf( "lobor_dbg: \x1b[32m" );
			break;

		case LOBOR_DEBUG_WARN:
			std::printf( "lobor_dbg: \x1b[33m" );
			break;

		case LOBOR_ERROR:
		default:
			std::printf( "lobor: \x1b[31m" );
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
void __attribute__( ( format ( printf, 1, 2 ) ) ) lobor::log( const char *format, ... )
{
	va_list ap;
	va_start( ap, format );
	lobor::log( LOBOR_NOTICE, format, ap );
	va_end( ap );
}

//The renderer
void lobor::update( )
{
	if ( lobor::status.active == false ) return;


	//Update buffers and poll events
	glfwSwapBuffers( lobor::win.window );
	glfwPollEvents( );
	lobor::status.active = !glfwWindowShouldClose( lobor::win.window );
}

//3D engine init routine
int lobor::init( int resx, int resy, std::string title )
{
	//GLFW init
	if ( !glfwInit( ) )
	{
		lobor::log( LOBOR_ERROR, "GLFW init failed" );
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
	lobor::win.resx = resx;
	lobor::win.resy = resy;
	lobor::win.title = title;
	lobor::win.window = glfwCreateWindow( lobor::win.resx, lobor::win.resy, lobor::win.title.c_str( ), NULL, NULL );
	if ( lobor::win.window == NULL )
	{
		std::cerr << "lobor: Window creation failed\n";
		glfwTerminate( );
		return 1;
	}
	glfwMakeContextCurrent( lobor::win.window );

	//GLEW init
	glewExperimental = true;
	if ( glewInit( ) != GLEW_OK )
	{
		std::cerr << "lobor: GLEW init failed!\n";
		glfwTerminate( );
		return 1;
	}

	//Input modes
	glfwSetInputMode( lobor::win.window, GLFW_STICKY_KEYS, GL_TRUE );

	//Consider Lobor running
	lobor::status.active = true;

	return 0;
};

//Cleanup function
void lobor::destroy( )
{
	glfwTerminate( );
}