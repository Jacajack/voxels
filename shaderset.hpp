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
class ShaderSet
{
	private:
		GLuint program_id;
		std::vector <GLuint> shaders;
		int load_txt_file( std::string &content, std::string filename );
		int load_shader( std::string filename, GLenum type );
		bool ready;

	public:
		std::map <std::string, GLint> uniforms;
		void use();
		ShaderSet( std::initializer_list <struct ShaderSpec> shader_specs, std::initializer_list <std::string> uniform_names );
		~ShaderSet();
};


#endif