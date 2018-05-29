#include <lobor/lobor.hpp>
#include <iostream>
#include <string>
#include <stdarg.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Convert GL error enum to human readable messages
const char *lobor::gl_error_str( GLenum error )
{
	switch ( error )
	{
		case GL_NO_ERROR:
			return "GL_NO_ERROR";
			break;

		case GL_INVALID_ENUM:
			return "GL_INVALID_ENUM";
			break;

		case GL_INVALID_VALUE:
			return "GL_INVALID_VALUE";
			break;

		case GL_INVALID_OPERATION:
			return "GL_INVALID_OPERATION";
			break;

		case GL_OUT_OF_MEMORY:
			return "GL_OUT_OF_MEMORY";
			break;

		case GL_STACK_UNDERFLOW:
			return "GL_STACK_UNDERFLOW";
			break;

		case GL_STACK_OVERFLOW:
			return "GL_STACK_OVERFLOW";
			break;

		default:
			return NULL;
	}
}

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

//GLFW init
void lobor::init_glfw( )
{
	//GLFW init
	if ( !glfwInit( ) )
	{
		lobor::log( LOBOR_ERROR, "GLFW init failed" );
		throw "GLFW init failed";
	}

	//Window params
	//Antialiasing, OpenGL 3.3
	glfwWindowHint( GLFW_SAMPLES, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
}

//GLEW init
void lobor::init_glew( )
{
	//GLEW init
	glewExperimental = true;
	if ( glewInit( ) != GLEW_OK )
	{
		lobor::log( LOBOR_ERROR, "lobor: GLEW init failed! Are you using any window?" );
		glfwTerminate( );
		throw "GLEW init failed";
	}
};

//Cleanup function
void lobor::destroy( )
{
	glfwTerminate( );
}