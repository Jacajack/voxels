#ifndef IFRIT_HPP
#define IFRIT_HPP

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//3D engine main namespace
namespace ifrit
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
	#define IFRIT_NOTICE 0
	#define IFRIT_LOG IFRIT_NOTICE
	#define IFRIT_WARN   1
	#define IFRIT_ERROR  2
	#define IFRIT_DEBUG  3
};

#endif