#ifndef LOBOR_TEXMODEL_HPP
#define LOBOR_TEXMODEL_HPP

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../lobor.hpp"

namespace lobor
{
	//Just a textured model class
	class TexturedModel : public Model
	{
		private:
			bool own_texture;

		protected:
			lobor::Texture *texture;

		public:
			virtual void draw( lobor::Shader &shader );

			TexturedModel( std::string model_filename, std::string texture_filename, bool should_load_buffers = true );
			TexturedModel( std::string model_filename, lobor::Texture *texture, bool should_load_buffers = true );
			~TexturedModel( );
	};
};

#endif