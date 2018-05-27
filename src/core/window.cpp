#include "window.hpp"
#include <string>
#include "../lobor.hpp"

//Just returns the window pointer
GLFWwindow *lobor::Window::get_window( )
{
	return this->window;
}

//Checks whether the window should close
bool lobor::Window::should_close( )
{
	return glfwWindowShouldClose( this->window );
}

//Just swap the buffers
void lobor::Window::swap_buffers( )
{
	glfwSwapBuffers( this->window );
}

//Makes window context current
void lobor::Window::use( )
{
	glfwMakeContextCurrent( this->window );
}

//Default constructor
lobor::Window::Window( int width, int height, std::string title, GLFWwindow *share = NULL )
{
	this->active = false;
	this->width = width;
	this->height = height;
	this->title = title;
	this->context_parent = share;
	this->window = glfwCreateWindow( width, height, title.c_str( ), NULL, share );
	if ( this->window == NULL )
	{
		lobor::log( LOBOR_ERROR, "cannot create window" );
		throw "cannot create window";
	}
	else
	{
		this->active = true;
	}
}

//Default destructor
lobor::Window::~Window( )
{
	this->active = false;
	glfwDestroyWindow( this->window );
}