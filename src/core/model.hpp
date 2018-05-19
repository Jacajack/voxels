#ifndef IFRIT_MODEL_HPP
#define IFRIT_MODEL_HPP

#include <vector>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace ifrit
{
	//This is basic renderable model
	//All things in the world that you can see derive from this class... And it's true no matter whether or not you believe in it
	class Model
	{
		protected:
			//Model data
			std::vector <glm::vec3> vertices;
			std::vector <glm::vec3> normals;
			std::vector <unsigned int> indices;
			std::vector <glm::vec2> uvs;
			bool buffers_loaded;

			//Wavefront file loader
			void load_obj_file( std::string filename );

		public:
			//Buffer operations
			void load_buffers( );
			void free_buffers( );

			//Render
			void draw( );

			//Constructor and destructor
			Model( std::string filename );
			~Model( );
	}
};

#endif