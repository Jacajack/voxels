#include "actor.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "model.hpp"

void Actor::draw( )
{
	glm::mat4 model_matrix;
	model_matrix = glm::translate( this->position ) 
		*  glm::rotate( this->rotation.x, glm::vec3( 1, 0, 0 ) )  
		*  glm::rotate( this->rotation.y, glm::vec3( 0, 1, 0 ) )  
		*  glm::rotate( this->rotation.z, glm::vec3( 0, 0, 1 ) )  
		* glm::scale( this->scale );

	for ( Model *model : this->submodels )
	{
		model->shaderset.use( );
		glUniformMatrix4fv( model->shaderset.uniforms["mat_model"], 1, GL_FALSE, &model_matrix[0][0] );
		model->draw( );
	}
}

Actor::Actor( std::initializer_list <Model*> submodels )
	: CompositeModel( submodels )
{
	this->position = glm::vec3( 0.0 );
	this->rotation = glm::vec3( 0.0 );
	this->scale = glm::vec3( 1.0 );
}
