#include <lobor/framebuffer.hpp>
#include <GL/glew.h>
#include <lobor/lobor.hpp>

//Just an implicit cast
lobor::Framebuffer::operator GLuint( )
{
	return this->id;
}

//Use framebuffer for reading/writing
void lobor::Framebuffer::bind( bool read, bool draw )
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

//Use default framebuffer for reading/writing
void lobor::Framebuffer::unbind( bool read, bool draw )
{
	if ( read && draw )
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	else
	{
		if ( read )
			glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
		else
			glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
	}
}

//Copy data to this framebuffer
void lobor::Framebuffer::blit( GLuint source, int sx0, int sy0, int sx1, int sy1, int dx0, int dy0, int dx1, int dy1, 
	GLbitfield mask, GLenum filter )
{
	//Save current framebuffers
	GLint fbod = 0, fbor = 0;
	glGetIntegerv( GL_READ_FRAMEBUFFER_BINDING, &fbor );
	glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &fbod );

	//Use this framebuffer for drawing
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, this->id );

	//Use specified framebuffer for reading
	glBindFramebuffer( GL_READ_FRAMEBUFFER, source );

	//Do da blit
	glBlitFramebuffer( sx0, sy0, sx1, sy1, dx0, dy0, dx1, dy1, mask, filter );

	//Bind the old framebuffers back
	glBindFramebuffer( GL_READ_FRAMEBUFFER, fbor );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fbod );
}

//Copy data from this framebuffer
void lobor::Framebuffer::blit_to( GLuint dest, int sx0, int sy0, int sx1, int sy1, int dx0, int dy0, int dx1, int dy1, 
	GLbitfield mask, GLenum filter )
{
	//Save current framebuffers
	GLint fbod = 0, fbor = 0;
	glGetIntegerv( GL_READ_FRAMEBUFFER_BINDING, &fbor );
	glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &fbod );

	//Use specified framebuffer for drawing
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, dest );

	//Use this framebuffer for reading
	glBindFramebuffer( GL_READ_FRAMEBUFFER, this->id );

	//Do da blit
	glBlitFramebuffer( sx0, sy0, sx1, sy1, dx0, dy0, dx1, dy1, mask, filter );

	//Bind the old framebuffers back
	glBindFramebuffer( GL_READ_FRAMEBUFFER, fbor );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fbod );
}

//Constructors
lobor::Framebuffer::Framebuffer( int width, int height, int texture_count )
{
	this->width = width;
	this->height = height;

	//Generate framebuffer and bind it
	glGenFramebuffers( 1, &this->id );
	glBindFramebuffer( GL_FRAMEBUFFER, this->id );

	lobor::log( LOBOR_DEBUG, "FBO id: %ld", (long) this->id );

	//Limits
	if ( texture_count > 32 )
	{
		lobor::log( LOBOR_ERROR, "framebuffer can't have more than 32 textures" );
		throw "framebuffer 32 texture limit exceeded";
	}
	if ( texture_count > 8 )
	{
		lobor::log( LOBOR_WARN, "8 texture per framebuffer limit exceeded" );
	}

	//Generate the textures and store them in the `textures' vector
	//TEMP: always RGB 8-bit
	for ( int i = 0; i < texture_count; i++ )
	{
		lobor::Texture tex( width, height, GL_UNSIGNED_BYTE, GL_RGB, GL_RGB );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, tex, 0 );
		this->textures.push_back( tex );
	}

	lobor::Texture dtex( width, height, GL_FLOAT, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dtex, 0 );
	this->textures.push_back( dtex );

	

	GLuint t;
	glGenTextures( 1, &t );
	glBindTexture( GL_TEXTURE_2D, t );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t, 0 );

	GLuint rbo;
	glGenRenderbuffers( 1, &rbo );
	glBindRenderbuffer( GL_RENDERBUFFER, rbo );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 768 );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo );


	lobor::log( LOBOR_INFO, "generated %dx%d framebuffer consisting of %ld textures", width, height, this->textures.size( ) );
}

//Destructor
lobor::Framebuffer::~Framebuffer( )
{
	//Destroy framebuffer
	glDeleteFramebuffers( 1, &this->id );

	//Destruct all the textures
	textures.clear( );
}