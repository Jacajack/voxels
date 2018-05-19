#ifndef IFRIT_HPP
#define IFRIT_HPP

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//3D engine main namespace
namespace ifrit
{
	//Window data
	struct
	{
		GLFWwindow *win;
		int resx, resy;
		std::string title;
	} window;

	//Init and destroy functions
	int init( int resx, int resy, std::string title );
	void destroy( );

};

#endif