#include <iostream>
#include "ifrit.hpp"


int main( int argc, char **argv )
{	std::cout << "Hello world!\n";
	
	ifrit::init( 1024, 768, "Ifrit dev" );
	while ( 1 );
	ifrit::destroy( );

	return 0;
}