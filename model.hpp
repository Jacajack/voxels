#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector> 

#include <libpng16/png.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderset.hpp"
#include "renderer.hpp"

class Model
{
	friend class CompositeModel;
	friend class Actor;

	protected:
		ShaderSet shaderset;

		//Model data
		std::vector <glm::vec3> vertices;
		std::vector <glm::vec2> uvs;
		std::vector <glm::vec3> normals;

		//Texture or color
		glm::vec4 tint;
		GLuint texture_id;
		unsigned char *texture;
		int texture_width, texture_height, texture_format;
		bool texture_loaded;

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
		virtual void draw( renderer::RenderContext context );
		Model(ShaderSet &program, std::string obj_filename, std::string texture_filename);
		Model(ShaderSet &program, std::string obj_filename, glm::vec3 color);
		~Model();
};

class CompositeModel
{
	protected:
		std::vector <Model*> submodels;

	public:
		CompositeModel(std::initializer_list <Model*> submodels);
		virtual void draw( renderer::RenderContext context );
};

#endif