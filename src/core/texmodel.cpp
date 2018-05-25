#include "texmodel.hpp"
#include <string>
#include <GL/glew.h>
#include "../ifrit.hpp"

/*
	This is the default draw method
	Used uniforms:
		- texture_sampler
*/
void ifrit::TexturedModel::draw( ifrit::Shader &shader )
{
	//Enable texture
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, this->texture->get_id( ) );

	//Pass texture sampler ID as uniform
	glUniform1i( shader.uniform( "texture_sampler" ), 0 );

	//Call base class draw method
	Model::draw( );

	//Disable texure
	glBindTexture( GL_TEXTURE_2D, 0 );
	glDisable( GL_TEXTURE_2D );
}

//The default constructor
ifrit::TexturedModel::TexturedModel( std::string model_filename, std::string texture_filename, bool should_load_buffers = true )
	: Model( model_filename, should_load_buffers )
{
	this->own_texture = true;
	this->texture = new ifrit::Texture( texture_filename );
}

//Constructor using texture ref
ifrit::TexturedModel::TexturedModel( std::string model_filename, ifrit::Texture *texture, bool should_load_buffers = true )
	: Model( model_filename, should_load_buffers )
{
	this->own_texture = false;
	this->texture = texture;
}

//Destructor
ifrit::TexturedModel::~TexturedModel( )
{
	if ( this->own_texture )
		delete this->texture;
}