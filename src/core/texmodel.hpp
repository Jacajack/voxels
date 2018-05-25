#ifndef IFRIT_TEXMODEL_HPP
#define IFRIT_TEXMODEL_HPP

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../ifrit.hpp"

namespace ifrit
{
	//Just a textured model class
	class TexturedModel : public Model
	{
		private:
			bool own_texture;

		protected:
			ifrit::Texture *texture;

		public:
			virtual void draw( ifrit::Shader &shader );

			TexturedModel( std::string model_filename, std::string texture_filename, bool should_load_buffers = true );
			TexturedModel( std::string model_filename, ifrit::Texture *texture, bool should_load_buffers = true );
			~TexturedModel( );
	};
};

#endif