#include "texmodel.hpp"
#include <string>
#include <GL/glew.h>
#include "../ifrit.hpp"

void ifrit::TexturedModel::draw( )
{

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
		delete &this->texture;
}