#include <iostream>
#include "ifrit.hpp"


int main( int argc, char **argv )
{	
	//Create window
	if ( ifrit::init( 1024, 768, "Ifrit dev" ) ) return 1;
	

	//Create vertex array
	GLuint vertex_array_id;
	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);


	ifrit::Shader shader(
		{
			{"shaders/flat/vertex.glsl", GL_VERTEX_SHADER},
			{"shaders/flat/fragment.glsl", GL_FRAGMENT_SHADER},
		},
		{
			"mat_model",
			"mat_view",
			"mat_projection",
			"model_tint",
			"texture_sampler"
		}
	);

	ifrit::Model unicorn( "models/unicorn.obj", true );

	glm::mat4 mat_model( 1.0 );
	glm::mat4 mat_view = glm::lookAt(
		glm::vec3( 4, 2, 4 ),
		glm::vec3( 0, 0, 0 ),
		glm::vec3( 0, 1, 0 )
	);
	glm::mat4 mat_projection = glm::perspective(
		glm::radians( 90.0 ), //FoV
		(double) 1024.0 / 768.0, //Aspect ratio
		0.1, //Near clipping plane
		100.0 //Far clipping plane
	);
	glm::vec4 model_tint( 1, 0, 1, 1 );
	

	shader.use( );
	glUniform4fv( shader.uniform( "model_tint" ), 1, &model_tint[0] );
	glUniformMatrix4fv( shader.uniform( "mat_view" ), 1, GL_FALSE, &mat_view[0][0] );
	glUniformMatrix4fv( shader.uniform( "mat_projection" ), 1, GL_FALSE, &mat_projection[0][0] );
	glUniformMatrix4fv( shader.uniform( "mat_model" ), 1, GL_FALSE, &mat_model[0][0] );
	glUniform1i( shader.uniform( "texture_sampler" ), 0 );




	//Enable Z-buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	


	//Game loop
	do
	{
		glClearColor( 0.2, 0.2, 0.0, 0.0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


		unicorn.draw( );
		ifrit::update( );
	}
	while ( ifrit::status.active == true );
	
	//Destroy
	ifrit::destroy( );

	return 0;
}