#include "shaderset.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Text file loader - used for shader loading
int ShaderSet::load_txt_file(std::string &content, std::string filename)
{
	std::ifstream file(filename, std::ios::in);
	if (file.is_open())
	{
		std::stringstream sstr;
		sstr << file.rdbuf();
		content = sstr.str();
		file.close();
	}
	else
	{
		std::cerr << "Cannot read file '" << filename << "'\n";
		return 1;
	}
	return 0;
}

//Load and compile single shader and return GL handle
int ShaderSet::load_shader( std::string filename, GLenum type )
{
	//Get source code
	std::string src;
	const char *csrc;
	if (this->load_txt_file(src, filename))
		return 1;
	csrc = src.c_str();

	//Create shader
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &csrc, NULL);

	//Compilation results
	int loglength = 0;
	GLint result = GL_FALSE;

	//Compile shader
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglength);
	if (loglength > 0)
	{
		std::vector <char> mesg(loglength + 1);
		glGetShaderInfoLog(shader, loglength, NULL, &mesg[0]);
		std::cerr << ">\t" << &mesg[0] << "\n";
	}

	//Register shader for linking
	this->shaders.push_back(shader);

	return 0;
}

void ShaderSet::use( )
{
	if ( !this->ready ) throw "ShaderSet not ready!";

	glUseProgram( this->program_id );
}

//ShaderSet constructor
ShaderSet::ShaderSet( std::initializer_list <struct ShaderSpec> shader_specs, std::initializer_list <std::string> uniform_names )
{
	this->ready = false;

	//Load all shaders
	for ( struct ShaderSpec shader_spec : shader_specs )
	{
		if ( this->load_shader( shader_spec.filename, shader_spec.shader_type ) )
		{
			this->ready = false;
			throw "file loading error";
		}
	}

	this->program_id = glCreateProgram();

	//Attach all shaders
	for ( unsigned int i = 0; i < this->shaders.size(); i++ )
	{
		glAttachShader( this->program_id, this->shaders[i] );
	}

	//Link
	glLinkProgram( this->program_id );

	//Linking results
	int loglength = 0;
	GLint result = GL_FALSE;

	//Check program
	glGetProgramiv(this->program_id, GL_LINK_STATUS, &result);
	glGetProgramiv(this->program_id, GL_INFO_LOG_LENGTH, &loglength);
	if (loglength > 0)
	{
		std::vector <char> mesg(loglength + 1);
		glGetProgramInfoLog(this->program_id, loglength, NULL, &mesg[0]);
		std::cout << ">\t" << &mesg[0] << "\n";
	}

	//Mark shaders for deletion
	for ( GLuint shader_id : this->shaders )
	{
		glDetachShader( this->program_id, shader_id );
		glDeleteShader( shader_id );
	}
	this->shaders.clear();

	//Load uniforms
	glUseProgram( this->program_id );
	for ( std::string uniform : uniform_names )
		this->uniforms[uniform] = glGetUniformLocation( this->program_id, uniform.c_str( ) );

	this->ready = true;
}

ShaderSet::~ShaderSet( )
{
	if ( !this->ready ) return;
	glDeleteProgram( this->program_id );
}