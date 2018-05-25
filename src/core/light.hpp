#ifndef IFRIT_LIGHT_HPP
#define IFRIT_LIGHT_HPP

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../ifrit.hpp"

namespace ifrit
{
	//Basic light class
	class Light
	{
		private:
			//Basic indirectional light source parameters
			glm::vec3 position;
			glm::vec3 color;
			float brightness, diffuse, specular;

			//Directional light source
			bool cast_shadows;
			glm::mat4 view_matrix;
			ifrit::Texture *shadow_map;


		public:
			void render_shadow_map( ifrit::Scene &scene );
			GLuint shadow_map_id( );

			//Constructor and destructor
			Light( glm::vec3 position, float brightness = 1.0, float diffuse = 1.0, float specular = 0.0, glm::vec3 color = glm::vec3( 1.0 ) );
			Light( glm::mat4 view_matrix, int shadow_map_size, float brightness = 1.0, float diffuse = 1.0, float specular = 0.0, glm::vec3 color = glm::vec3( 1.0 ) );
			~Light( );

	};
};

#endif