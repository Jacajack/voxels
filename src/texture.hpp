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
			uint8_t *texture;
			int width, height;
			bool texture_loaded;
			GLuint texture_id;

		protected:
		public:
			Texture( int width, int height );
			Texture( int width, int height, std::string filename );
			~Texture( );
	};
};

#endif