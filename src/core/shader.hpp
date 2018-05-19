#ifndef IFRIT_SHADER_HPP
#define IFRIT_SHADER_HPP

#include <initializer_list>
#include <vector>
#include <string>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


namespace ifrit
{
	//Struct used just by Shader constructor
	struct ShaderSpec
	{
		std::string filename;
		GLenum shader_type;
	};

	//Shader set
	class Shader
	{
		private:
			GLuint program_id;
			std::map <std::string, GLuint> uniforms;

		public:
			Shader( std::initializer_list <struct ShaderSpec> specs, std::initializer_list <std::string> uniform_names );
			~Shader( );

	};
};

#endif