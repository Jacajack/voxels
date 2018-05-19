#include <iostream>
#include "ifrit.hpp"


int main( int argc, char **argv )
{	
	//Create window
	if ( ifrit::init( 1024, 768, "Ifrit dev" ) ) return 1;
	
	//Game loop
	do
	{
		ifrit::update( );
	}
	while ( ifrit::status.active == true );
	
	//Destroy
	ifrit::destroy( );

	return 0;
}