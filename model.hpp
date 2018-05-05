#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector> 

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

class Model
{
	private:
		//Model data
		std::vector <glm::vec3> vertices;
		std::vector <glm::vec2> uvs;
		std::vector <glm::vec3> normals;

		//GL stuff
		GLuint vertex_buffer;
		bool buffer_loaded;

		int loadFromFile(std::string filename, bool verbose);
		void init_buffers();
		void free_buffers();

	public:
		void draw();
		Model(std::string filename);
		~Model();
};

#endif