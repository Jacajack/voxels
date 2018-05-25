#include "texture.hpp"
#include <string>
#include <GL/glew.h>
#include "../ifrit.hpp"


//Simple, default allocation constructor
ifrit::Texture::Texture( int width, int height, GLenum datatype, GLenum format, GLenum iformat )
{
	this->texture_loaded = false;
	this->width = width;
	this->height = height;

	//Generate texture
	glGenTextures( 1, &this->texture_id );

	//Texture settings
	glBindTexture( GL_TEXTURE_2D, this->texture_id );
	glTexImage2D( GL_TEXTURE_2D, 0, iformat, width, height, 0, format, datatype, NULL );

	//Linear filtering
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	//Clamp to border
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );

	//Generate mipmaps
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0 );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0 );

	//Consider texture loaded
	this->texture_loaded = true;
}

//Constructor loading data from file
ifrit::Texture::Texture( std::string filename )
{

}

//Texture destructor
ifrit::Texture::~Texture( )
{
	if ( this->texture_loaded )
		glDeleteTextures( 1, &this->texture_id );
}