#ifndef LOBOR_TEXTUREPEEK_HPP
#define LOBOR_TEXTUREPEEK_HPP

#include <vector>
#include <GL/glew.h>
#include "window.hpp"
#include "lobor.hpp"

namespace lobor
{
	//Framebuffer class
	class Texturepeek : public Window
	{
		private:
			GLuint vertex_buffer_id;
			GLuint uv_buffer_id;

		protected:
			//The displayed texture
			GLuint texture;
			
			//The vertex array object for displaying textured quad
			GLuint vao;
			lobor::Shader shader;

		public:
			void update( );
			Texturepeek( int width, int height, std::string title, GLuint texture, GLFWwindow *share );
			~Texturepeek( );
	};
};

#endif