#include "model.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <string.h>
#include "../lobor.hpp"

//Very simple OBJ file loader
void lobor::Model::load_obj_file( std::string filename )
{
	//Open input file
	std::ifstream objfile( filename, std::ios::in );

	//Packed model data
	std::vector <glm::vec3> vertices;
	std::vector <glm::vec2> uvs;
	std::vector <glm::vec3> normals;

	//IDs
	std::vector <unsigned int> vertex_ids;
	std::vector <unsigned int> uv_ids;
	std::vector <unsigned int> normal_ids;


	if ( objfile.is_open( ) )
	{
		//Read file line by line
		std::string line;
		int linecnt = 1;

		while ( std::getline( objfile, line ) )
		{
			//Skip comments
			if ( line[0] == '#' ) continue;

			//Find first space and split line into token type and data
			size_t sep = line.find( " " );
			if ( sep == std::string::npos ) continue;
			std::string token_type = line.substr( 0, sep );
			std::string token_data = line.substr( sep );

			if ( token_type == "v" ) //Vertex data
			{
				glm::vec3 v;
				std::sscanf( token_data.c_str( ), "%f %f %f", &v.x, &v.y, &v.z );
				vertices.push_back( v );
			}
			else if ( token_type == "vt" ) //UV data
			{
				glm::vec2 uv;
				std::sscanf( token_data.c_str( ), "%f %f", &uv.x, &uv.y );
				uvs.push_back( uv );
			}
			else if ( token_type == "vn" ) //Normal data
			{
				glm::vec3 norm;
				std::sscanf( token_data.c_str( ), "%f %f %f", &norm.x, &norm.y, &norm.z );
				normals.push_back( norm );
			}
			else if ( token_type == "f" ) //Face data
			{
				//IDs
				unsigned int ids[9];
				char *linebuf = strdup( token_data.c_str( ) );
				if ( linebuf == NULL )
				{
					lobor::log( LOBOR_ERROR, "stdup() failed!" );
					throw "strdup() returned NULL";
				}

				char *strtok_buf = NULL;
				int token_id = 0;
				for ( const char *token = strtok_r( linebuf, "/ ", &strtok_buf ); token != NULL && token_id < 9; token = strtok_r( NULL, "/ ", &strtok_buf ), token_id++ )
				{
					ids[token_id] = std::atoi( token );
				}

				free( linebuf );

				//Push IDs
				vertex_ids.push_back( ids[0] );
				vertex_ids.push_back( ids[3] );
				vertex_ids.push_back( ids[6] );
				uv_ids.push_back( ids[1] );
				uv_ids.push_back( ids[4] );
				uv_ids.push_back( ids[7] );
				normal_ids.push_back( ids[2] );
				normal_ids.push_back( ids[5] );
				normal_ids.push_back( ids[8] );
			}
			else
			{
				lobor::log( LOBOR_WARN, "unhandled token type '%s' in Wavefront file '%s' at line %d", token_type.c_str( ), filename.c_str( ), linecnt );
			}

			linecnt++;
		}

		//Close the file
		objfile.close( );
	}
	else
	{
		lobor::log( LOBOR_ERROR, "cannot load Wavefront file '%s'", filename.c_str( ) );
		throw "cannot load Wavefront file";
	}

	//Log info
	lobor::log( LOBOR_LOG, "loaded %ld vertices, %ld normals and %ld UVs from %s", vertices.size( ), normals.size( ), uvs.size( ), filename.c_str( ) );

	//Dispatch data
	unsigned int missing_vertices = 0, missing_normals = 0, missing_uvs = 0;
	for ( unsigned int i = 0; i < vertex_ids.size( ); i++ )
	{
		unsigned int vertex_id = vertex_ids[i] - 1;
		unsigned int uv_id = uv_ids[i] - 1;
		unsigned int normal_id = normal_ids[i] - 1;

		if ( vertex_id >= vertices.size( ) )
		{
			missing_vertices++;
			this->vertices.push_back( glm::vec3( 0.0 ) );
		}
		else this->vertices.push_back( glm::vec3( vertices[vertex_id] ) );

		if ( uv_id >= uvs.size( ) )
		{
			missing_uvs++;
			this->uvs.push_back( glm::vec2( 0.0 ) );
		}
		else this->uvs.push_back( glm::vec2( uvs[uv_id] ) );
		
		if ( normal_id >= normals.size( ) )
		{
			missing_normals++;
			this->normals.push_back( glm::vec3( 0.0 ) );
		}
		else this->normals.push_back( glm::vec3( normals[normal_id] ) );
	}
}

//Load OpenGL buffers while preserving data in vectors
void lobor::Model::load_buffers( )
{
	if ( this->buffers_loaded ) return;

	//Create and load vertex buffer
	glGenBuffers( 1, &this->vertex_buffer_id );
	glBindBuffer( GL_ARRAY_BUFFER, this->vertex_buffer_id );
	glBufferData( GL_ARRAY_BUFFER, sizeof( this->vertices[0] ) * this->vertices.size( ), &this->vertices[0], GL_STATIC_DRAW );

	//Create and load UV buffer
	glGenBuffers( 1, &this->uv_buffer_id );
	glBindBuffer( GL_ARRAY_BUFFER, this->uv_buffer_id );
	glBufferData( GL_ARRAY_BUFFER, sizeof( this->uvs[0] ) * this->uvs.size( ), &this->uvs[0], GL_STATIC_DRAW );

	//Create and load normal buffer
	glGenBuffers( 1, &this->normal_buffer_id );
	glBindBuffer( GL_ARRAY_BUFFER, this->normal_buffer_id );
	glBufferData( GL_ARRAY_BUFFER, sizeof( this->normals[0] ) * this->normals.size( ), &this->normals[0], GL_STATIC_DRAW );

	this->buffers_loaded = true;
}

//Free OpenGL buffers while preserving data in vectors
void lobor::Model::free_buffers( )
{
	if ( !this->buffers_loaded ) return;

	glDeleteBuffers( 1, &this->vertex_buffer_id );
	glDeleteBuffers( 1, &this->uv_buffer_id );
	glDeleteBuffers( 1, &this->normal_buffer_id );


	this->buffers_loaded = false;
}

//Basic render function
void lobor::Model::draw( )
{
	if ( !this->buffers_loaded ) load_buffers( );

	glEnableVertexAttribArray( 0 ); //Vertex data
	glEnableVertexAttribArray( 1 ); //UVs
	glEnableVertexAttribArray( 2 ); //Normals

	//Attribute 0 - vertex position
	glBindBuffer( GL_ARRAY_BUFFER, this->vertex_buffer_id );
	glVertexAttribPointer(
		0, //Attribute ID
		3, //Size
		GL_FLOAT, //Type
		GL_FALSE, //Normalized
		0, //Stride
		NULL //Array buffer offset
	);

	//Attribute 1 - UV
	glBindBuffer( GL_ARRAY_BUFFER, this->uv_buffer_id );
	glVertexAttribPointer(
		1, //Attribute ID
		2, //Size
		GL_FLOAT, //Type
		GL_FALSE, //Normalized
		0, //Stride
		NULL //Array buffer offset
	);

	//Attribute 2 - Normals
	glBindBuffer( GL_ARRAY_BUFFER, this->normal_buffer_id );
	glVertexAttribPointer(
		2, //Attribute ID
		3, //Size
		GL_FLOAT, //Type
		GL_FALSE, //Normalized
		0, //Stride
		NULL //Array buffer offset
	);

	glDrawArrays( GL_TRIANGLES, 0, this->vertices.size( ) );

	glDisableVertexAttribArray( 0 ); //Vertex data
	glDisableVertexAttribArray( 1 ); //UV
	glDisableVertexAttribArray( 2 ); //Normals
}

//Model constructor loading data from file
lobor::Model::Model( std::string filename, bool should_load_buffers )
{
	//Buffers are not loaded by default
	this->buffers_loaded = false;

	//Load OBJ file
	load_obj_file( filename );

	//Load buffers
	if ( should_load_buffers )
	{
		load_buffers( );
	}
}

//Model destructor
lobor::Model::~Model( )
{
	free_buffers( );
}