#ifndef LOBOR_WINDOW_HPP
#define LOBOR_WINDOW_HPP

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace lobor
{
	class Window
	{
		protected:
			//Basic window data
			GLFWwindow *window, *context_parent;
			int width, height;
			std::string title;
			bool active;
		
		public:
			Window( int width, int height, std::string title, GLFWwindow *share = NULL );
			~Window( );

			//Implicit conversion to GLFWwindow* - have fun debugging!
			operator GLFWwindow*( );

			int get_width( );
			int get_height( );

			GLFWwindow *get_window( ); //Returns pointer to the GLFWwindow
			bool should_close( ); //Checks if window should close
			void swap_buffers( ); //Calls glfwSwapBuffers
			void use( ); //Makes window context current
	};
};

#endif