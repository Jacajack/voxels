#include "shader.hpp"
#include <initializer_list>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../ifrit.hpp"

//Slurp file into std::string
static int slurp_file( std::string &str, std::string filename )
{
	std::ifstream file( filename, std::ios::in );
	if ( file.is_open( ) )
	{
		//Read file into string
		std::stringstream sstr;
		sstr << file.rdbuf( );
		str = sstr.str( );
		file.close( );
	}
	else
	{
		//Bail out
		ifrit::log( IFRIT_ERROR, "cannot read file '%s'", filename.c_str( ) );
		return 1;
	}

	//Success
	return 0;
}

//Uniform getter
GLuint ifrit::Shader::uniform( std::string name )
{
	return this->uniforms[name];
}

//Use program
void ifrit::Shader::use( )
{
	glUseProgram( this->program_id );
}

//Shader set constructor
ifrit::Shader::Shader( std::initializer_list <struct ShaderSpec> specs, std::initializer_list <std::string> uniform_names )
{
	//Shader is not ready by default
	this->ready = false;

	//Temporary vector for shader handles
	std::vector <GLuint> shaders;

	//Load all shaders
	for ( struct ifrit::ShaderSpec spec : specs )
	{
		//Shader source code
		std::string src;
		const char *csrc;
		if ( slurp_file( src, spec.filename ) ) throw "Loading shader file failed";
		csrc = src.c_str( );
		
		//Create shader and load source code
		GLuint shader = glCreateShader( spec.shader_type );
		glShaderSource( shader, 1, &csrc, NULL );

		//Variables for compilation results
		int loglength = 0;
		GLint result = GL_FALSE;

		//Attempt to compile shader
		glCompileShader( shader );
		glGetShaderiv( shader, GL_COMPILE_STATUS, &result );
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &loglength );
		
		//Output compilation results
		if ( loglength > 0 )
		{
			std::vector <char> message( loglength + 1 );
			glGetShaderInfoLog( shader, loglength, NULL, &message[0] );
			ifrit::log( IFRIT_ERROR, "shader file '%s' compilation failed:\n%s", spec.filename.c_str( ), &message[0] );
			throw "shader compilation failed";
		}

		//Push back the shader
		shaders.push_back( shader );
	}

	//Link the shaders
	this->program_id = glCreateProgram( );

	//Attach all shaders to the program
	for ( GLuint shader : shaders )
		glAttachShader( this->program_id, shader );

	//Link
	glLinkProgram( this->program_id );

	//Get linking results
	int loglength = 0;
	GLint result = GL_FALSE;
	glGetProgramiv( this->program_id, GL_COMPILE_STATUS, &result );
	glGetProgramiv( this->program_id, GL_INFO_LOG_LENGTH, &loglength );
	
	//Output compilation results
	if ( loglength > 0 )
	{
		std::vector <char> message( loglength + 1 );
		glGetShaderInfoLog( this->program_id, loglength, NULL, &message[0] );
		ifrit::log( IFRIT_ERROR, "shader linking failed:\n%s", &message[0] );
		throw "shader linking failed";
	}

	//Detach shaders
	for ( GLuint shader : shaders )
	{
		glDetachShader( this->program_id, shader );
		glDeleteShader( shader );
	}

	//Load shader uiforms
	glUseProgram( this->program_id );
	for ( std::string uniform : uniform_names )
	{
		this->uniforms[uniform] = glGetUniformLocation( this->program_id, uniform.c_str( ) );
		ifrit::log( IFRIT_DEBUG, "uniform '%s' at %ld", uniform.c_str( ), (long) this->uniforms[uniform] );
	}

	//Shader loaded
	this->ready = true;
}

//Shader destructor
ifrit::Shader::~Shader( )
{
	if ( this->ready ) glDeleteProgram( this->program_id );
}