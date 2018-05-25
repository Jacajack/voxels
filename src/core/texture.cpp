#include "texture.hpp"
#include <string>
#include <GL/glew.h>
#include <libpng16/png.h>
#include "../ifrit.hpp"


//Simple, default allocation constructor
void ifrit::Texture::generate_texture( int width, int height, GLenum type, GLenum format, GLenum iformat, const void *data = NULL )
{
	this->width = width;
	this->height = height;
	this->type = type;
	this->format = format;
	this->iformat = iformat;

	//Generate texture
	glGenTextures( 1, &this->id );

	//Texture settings
	glBindTexture( GL_TEXTURE_2D, this->id );
	glTexImage2D( GL_TEXTURE_2D, 0, iformat, width, height, 0, format, type, data );

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

	ifrit::log( IFRIT_LOG, "generated %dx%d texture", width, height );
}

//Loading data from file
void ifrit::Texture::load_image_data( std::string filename )
{
	//Open texture file like C would
	std::FILE *texfile;
	texfile = std::fopen( filename.c_str( ), "rb" );
	if ( texfile == NULL )
	{
		ifrit::log( IFRIT_ERROR, "could not open file `%s' for reading", filename.c_str( ) );
		throw "cannot load texture file";
	}

	//Init libpng
	png_structp png = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
	if ( png == NULL )
	{
		ifrit::log( IFRIT_ERROR, "libpng init failed" );
		throw "cannot init libpng";
	}

	//Get PNG info
	png_infop info = png_create_info_struct( png );
	if ( info == NULL )
	{
		ifrit::log( IFRIT_ERROR, "cannot get PNG info from `%s'", filename.c_str( ) );
		throw "cannot get PNG info";
	}

	//Libpng error handler
	if ( setjmp( png_jmpbuf( png ) ) )
	{
		png_destroy_read_struct( &png, &info, NULL );
		ifrit::log( IFRIT_ERROR, "libpng error while loading `%s'", filename.c_str( ) );
		throw "libpng error";
	}

	//Read PNG info
	png_init_io( png, texfile );
	png_read_info( png, info );
	this->width = png_get_image_width( png, info );
	this->height = png_get_image_height( png, info );

	//tRNS to alpha
	if ( png_get_valid( png, info, PNG_INFO_tRNS ) )
		png_set_tRNS_to_alpha( png );

	//Determine and convert format
	switch ( png_get_color_type( png, info ) )
	{
		//Grayscale
		case PNG_COLOR_TYPE_GRAY:
			this->format = GL_RGB;
			png_set_gray_to_rgb( png );
			ifrit::log( IFRIT_LOG, "converting grayscale PNG `%s'", filename.c_str( ) );
			break;

		//Grayscale + alpha
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			this->format = GL_RGBA;
			png_set_gray_to_rgb( png );
			ifrit::log( IFRIT_LOG, "converting grayscale+alpha PNG `%s'", filename.c_str( ) );
			break;

		//Palette
		case PNG_COLOR_TYPE_PALETTE:
			this->format = GL_RGB;
			png_set_expand( png );
			ifrit::log( IFRIT_LOG, "converting palette PNG `%s'", filename.c_str( ) );
			break;

		//RGB
		case PNG_COLOR_TYPE_RGB:
			this->format = GL_RGB;
			ifrit::log( IFRIT_LOG, "reading RGB PNG `%s'", filename.c_str( ) );
			break;

		//RGBA
		case PNG_COLOR_TYPE_RGBA:
			this->format = GL_RGBA;
			ifrit::log( IFRIT_LOG, "reading RGBA PNG `%s'", filename.c_str( ) );
			break;

		//Unhandled PNG
		default:
			png_destroy_info_struct( png, &info );
			png_destroy_read_struct( &png, &info , NULL );
			std::fclose( texfile );
			ifrit::log( IFRIT_ERROR, "unhandled PNG type `%s'", filename.c_str( ) );
			throw "unhandled PNG type";
			break;
	}

	png_set_interlace_handling( png );
	png_read_update_info( png, info );

	//Linear texture buffer
	int bpp = this->format == GL_RGB ? 3 : 4;
	this->image_data = new uint8_t[this->width * this->height * bpp];
	this->image_data_loaded = true;

	//And this tricks libpng to read data into my linear bufer
	//Note: BMPs are upside down while PNGs are not
	png_bytep *rows = new png_bytep[this->height];
	uint8_t *p = this->image_data;
	for ( int i = this->height - 1; i >= 0; i-- )
	{
		rows[i] = p;
		p += this->width * bpp;
	}

	//Read data
	png_read_image( png, rows );

	//Cleanup
	delete rows;
	png_read_end( png, NULL );
	png_destroy_info_struct( png, &info );
	png_destroy_read_struct( &png, &info, NULL );
	std::fclose( texfile );

	ifrit::log( IFRIT_LOG, "loaded %dx%d texture from file `%s'", width, height, filename.c_str( ) );
}

//Getters
GLuint ifrit::Texture::get_id( )
{
	return this->id;
}

int ifrit::Texture::get_width( )
{
	return this->width;
}

int ifrit::Texture::get_height( )
{
	return this->height;
}

//Constructor loading from file
ifrit::Texture::Texture( std::string filename )
{
	this->load_image_data( filename );
	this->generate_texture( this->width, this->height, GL_UNSIGNED_BYTE, this->format, GL_RGB, this->image_data );
}

//Just texture allocation
ifrit::Texture::Texture( int width, int height, GLenum type, GLenum format, GLenum iformat )
{
	this->image_data = nullptr;
	this->image_data_loaded = false;
	this->generate_texture( width, height, type, format, iformat );
}

//Texture destructor
ifrit::Texture::~Texture( )
{
	if ( this->texture_loaded )
		glDeleteTextures( 1, &this->id );

	if ( this->image_data_loaded )
		delete this->image_data;
}