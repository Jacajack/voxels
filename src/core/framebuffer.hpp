#ifndef LOBOR_FRAMEBUFFER_HPP
#define LOBOR_FRAMEBUFFER_HPP

#include <GL/glew.h>
#include "../lobor.hpp"

namespace lobor
{
	//Framebuffer class
	class Framebuffer
	{
		private:
			GLuint id;
			lobor::Texture *texture;
			bool has_own_texture;

		public:
			//Implicit conversion to GLuint - makes life easier
			operator GLuint( );

			void use( bool read, bool draw );
			void blit( GLuint source, int sx0, int sy0, int sx1, int sy1, int dx0, int dy0, int dx1, int dy1, GLbitfield mask, GLenum filter );

			Framebuffer( lobor::Texture &texture );
			Framebuffer( int width, int height );
			~Framebuffer( );
	};
};

#endif