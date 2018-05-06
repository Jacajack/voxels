#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector> 

#include <libpng16/png.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

class Model
{
	private:
		//Model data
		std::vector <glm::vec3> vertices;
		std::vector <glm::vec2> uvs;
		std::vector <glm::vec3> normals;

		//Texture
		GLuint texture_id;
		unsigned char *texture;
		int texture_width, texture_height;

		//Vertex and UV buffer
		GLuint vertex_buffer_id;
		GLuint uv_buffer_id;
		GLuint normal_buffer_id;
		
		//OpenGL buffers ready acknowledgement
		bool buffers_loaded;

		int load_obj(std::string filename, bool verbose);
		int load_texture(std::string filename, bool verbose);
		void init_buffers();
		void free_buffers();

	public:
		void draw(GLuint texture_uniform_id);
		Model(std::string obj_filename, std::string texture_filename);
		~Model();
};

#endif