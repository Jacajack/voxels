#ifndef LOBOR_FRAMEBUFFER_HPP
#define LOBOR_FRAMEBUFFER_HPP

#include <vector>
#include <GL/glew.h>
#include "lobor.hpp"

namespace lobor
{
	//Framebuffer class
	class Framebuffer
	{
		private:
			int width, height;
			GLuint id;
			std::vector <lobor::Texture> textures;

		public:
			//Implicit conversion to GLuint - makes life easier
			operator GLuint( );

			void use( bool read = true, bool draw = true );
			void blit( GLuint source, int sx0, int sy0, int sx1, int sy1, int dx0, int dy0, int dx1, int dy1, GLbitfield mask = GL_COLOR_BUFFER_BIT, GLenum filter = GL_NEAREST );
			void blit_to( GLuint dest, int sx0, int sy0, int sx1, int sy1, int dx0, int dy0, int dx1, int dy1, GLbitfield mask = GL_COLOR_BUFFER_BIT, GLenum filter = GL_NEAREST );

			Framebuffer( int width, int height, int texture_count );
			~Framebuffer( );
	};
};

#endif