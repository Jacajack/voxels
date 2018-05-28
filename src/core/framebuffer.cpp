#include "framebuffer.hpp"
#include <GL/glew.h>
#include "../lobor.hpp"

//Just an implicit cast
lobor::Framebuffer::operator GLuint( )
{
	return this->id;
}

//Use framebuffer for reading/writing
void lobor::Framebuffer::use( bool read = true, bool draw = true )
{
	if ( read && draw )
		glBindFramebuffer( GL_FRAMEBUFFER, this->id );
	else
	{
		if ( read )
			glBindFramebuffer( GL_READ_FRAMEBUFFER, this->id );
		else
			glBindFramebuffer( GL_DRAW_FRAMEBUFFER, this->id );
	}
}

//Copy data to this framebuffer
void lobor::Framebuffer::blit( GLuint source, int sx0, int sy0, int sx1, int sy1, int dx0, int dy0, int dx1, int dy1, 
	GLbitfield mask = GL_COLOR_BUFFER_BIT, GLenum filter = GL_NEAREST )
{
	//Save current draw framebuffer
	GLint fbod = 0;
	glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &fbod );

	//Use this framebuffer for drawing
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, this->id );

	//Do da blit
	glBlitFramebuffer( sx0, sy0, sx1, sy1, dx0, dy0, dx1, dy1, mask, filter );

	//Bind the old framebuffer back
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fbod );
}

//Constructors
lobor::Framebuffer::Framebuffer( )
{
	//Generate framebuffer
	glGenFramebuffers( 1, &this->id );

	
}

//Destructor
lobor::Framebuffer::~Framebuffer( )
{
	//Destroy framebuffer
	glDeleteFramebuffers( 1, &this->id );
}