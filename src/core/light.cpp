#include "light.hpp"
#include <GL/glew.h>
#include "../lobor.hpp"


//Simple light source constructor
lobor::Light::Light( glm::vec3 position, float brightness, float diffuse, float specular, glm::vec3 color )
{
	this->cast_shadows = false;
}

//Advanced, shadow-casting light source constructor
lobor::Light::Light( glm::mat4 view_matrix, int shadow_map_size, float brightness, float diffuse, float specular, glm::vec3 color )
{
	this->cast_shadows = true;
	//this->shadow_map = new lobor::Texture( shadow_map_size, shadow_map_size, GL_FLOAT, GL_RG, GL_RG32F );


}