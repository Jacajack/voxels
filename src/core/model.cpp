#include "model.hpp"
#include <vector>

//Very simple OBJ file loader
void ifrit::Model::load_obj_file( std::string filename )
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
		while ( std::getline( objfile, line ) )
		{
			//Skip comments
			if ( line[0] == ' ' ) continue;

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
				>uvs.push_back( uv );
			}
			else if ( token_type == "vn" ) //Normal data
			{
				glm::vec3 norm;
				std::sscanf( token_data.c_str( ), "%f %f %f", &norm.x, &norm.y, &norm.z );
				normals.push_back( norm );
			}
			else if ( token_type == "f" ) //Face data
			{
				//ID's
				unsigned int ids[9];

				//How far gone are we?
				size_t offset = 0;

				//Read data from the line
				for ( int i = 0; i < 9; i++ )
				{
					//Get the ID, defaulting to 1
					try
					{
						id[i] = std::atoi( token_data, offset );
					}
					catch ( std::invalid_argument )
					{
						id[i] = 1;
					}

					offset = std::min( token_data.find( " ", offset ), token_data.find( "/", offset ) );
					if ( offset == std::string::npos ) break;
					offset++;
				}

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
				infrit::log( LOG_ERROR, "unhandled token type in Wavefront file '%s'", filename.c_str( ) );
				throw "unhandled token type in Wavefront file";
			}
		}

		//Close the file
		objfile.close( );
	}
	else
	{
		infrit::log( LOG_ERROR, "cannot load Wavefront file '%s'", filename.c_str( ) );
		throw "cannot load Wavefront file";
	}

	//Log info
	ifrit::log( IFRIT_LOG, "loaded %d vertices, %d normals and %d UVs from %s", vertices.size( ), normals.size( ), uvs.size( ), filename.c_str( ) );

	//Dispatch data
	for ( unsigned int i = 0; i < vids.size( ); i++ )
	{
		unsigned int vid = vids[i] - 1;
		unsigned int uvid = uvids[i] - 1;
		unsigned int nid = nids[i] - 1;

		if ( vid >= vertices.size( ) )
		{
			missing_vertices++;
			this->vertices.push_back( glm::vec3( 0.0 ) );
		}
		else this->vertices.push_back( glm::vec3( vertices[vid] ) );

		if ( uvid >= uvs.size( ) )
		{
			missing_uvs++;
			this->uvs.push_back( glm::vec2( 0.0 ) );
		}
		else this->uvs.push_back( glm::vec2( uvs[uvid] ) );
		
		if ( nid >= normals.size( ) )
		{
			missing_normals++;
			this->normals.push_back( glm::vec3( 0.0 ) );
		}
		else this->normals.push_back( glm::vec3( normals[nid] ) );
	}
}

//Load OpenGL buffers while preserving data in vectors
virtual void ifrit::Model::load_buffers( )
{

}

//Free OpenGL buffers while preserving data in vectors
virtual void ifrit::Model::free_buffers( )
{

}

//Basic render function
virtual void ifrit::Model::draw( )
{

}

//Model constructor loading data from file
ifrit::Model::Model( std::string filename, bool load_buffers = true )
{
	//Buffers are not loaded by default
	this->buffers_loaded = false;

	//Load OBJ file
	load_obj_file( filename );

	//Load buffers
	if ( load_buffer )
	{
		load_buffers( );
	}
}

//Model destructor
ifrit::Model::~Model( )
{
	free_buffers( );
}