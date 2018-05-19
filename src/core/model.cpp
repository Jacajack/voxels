#include "model.hpp"
#include <vector>

//Very simple OBJ file loader
void ifrit::Model::load_obj_file( std::string filename )
{
	//Open input file
	std::ifstream objfile( filename, std::ios::in );

	while ( true )
	{

	}

}

//Model constructor loading data from file
ifrit::Model::Model( std::string filename )
{

}

//Model destructor
ifrit::Model::~Model( )
{
	
}