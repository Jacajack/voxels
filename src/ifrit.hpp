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
};

#endif