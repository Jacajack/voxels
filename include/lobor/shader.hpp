#ifndef LOBOR_SHADER_HPP
#define LOBOR_SHADER_HPP

#include <initializer_list>
#include <vector>
#include <string>
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


namespace lobor
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
			std::map <std::string, GLint> uniforms;
			bool ready;

		public:
			//Utilities
			void use( );
			GLuint uniform( std::string name );

			//Constructor and destructor
			Shader( std::initializer_list <struct ShaderSpec> specs, std::initializer_list <std::string> uniform_names );
			~Shader( );
	};
};

#endif