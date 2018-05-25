#ifndef LOBOR_HPP
#define LOBOR_HPP

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "core/shader.hpp"
#include "core/model.hpp"
#include "core/texture.hpp"
#include "core/texmodel.hpp"
#include "core/scene.hpp"

//3D engine main namespace
namespace lobor
{
	//Window data
	extern struct WindowInformation
	{
		GLFWwindow *window;
		int resx, resy;
		std::string title;
	} win;

	//Status data
	extern struct CoreStatus
	{
		bool active;
	} status;

	//Renderer and updater function
	extern void update( );

	//Init and destroy functions
	extern int init( int resx, int resy, std::string title );
	extern void destroy( );

	//Utils
	extern void __attribute__( ( format ( printf, 2, 3 ) ) ) log( int level, const char *format, ... );
	extern void __attribute__( ( format ( printf, 1, 2 ) ) ) log( const char *format, ... );
	#define LOBOR_NOTICE 0
	#define LOBOR_LOG LOBOR_NOTICE
	#define LOBOR_INFO LOBOR_NOTICE
	#define LOBOR_WARN   1
	#define LOBOR_ERROR  2
	#define LOBOR_DEBUG  3
	#define LOBOR_DEBUG_WARN 4
};

#endif