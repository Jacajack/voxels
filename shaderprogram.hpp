#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <vector>
#include <string>
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct ShaderSpec
{
	std::string filename;
	GLenum shader_type;
};

//Shader program class
class ShaderProgram
{
	private:
		GLuint program_id;
		std::vector <GLuint> shaders;
		int load_txt_file( std::string &content, std::string filename );
		int load_shader( std::string filename, GLenum type );
		bool ready;

	public:
		std::map <std::string, GLuint> uniforms;
		void use();
		ShaderProgram( std::initializer_list <struct ShaderSpec> shader_specs, std::initializer_list <std::string> uniform_names );
		~ShaderProgram();
};


#endif