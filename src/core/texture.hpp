#ifndef IFRIT_TEXTURE_HPP
#define IFRIT_TEXTURE_HPP

#include <string>
#include <inttypes.h>
#include <GL/glew.h>

namespace ifrit
{
	class Texture
	{
		//Just the texture data here
		private:
			uint8_t *image_data;
			bool image_data_loaded;

			int width, height;
			bool texture_loaded;
			
			GLuint id;
			GLenum type, format, iformat;

			//Some constructor-like functions
			void load_image_data( std::string filename );
			void generate_texture( int width, int height, GLenum type, GLenum format, GLenum iformat, const void *data );

		protected:
		public:
			//Getter functions
			GLuint get_id( );
			int get_width( );
			int get_height( );

			Texture( int width, int height, GLenum datatype, GLenum format, GLenum iformat );
			Texture( std::string filename );
			~Texture( );
	};
};

#endif