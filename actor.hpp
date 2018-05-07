#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "model.hpp"

#include <initializer_list>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Actor : public CompositeModel
{
	private:
		

	public:
		glm::vec3 rotation;
		glm::vec3 position;
		glm::vec3 scale;

		void draw( );
		Actor( std::initializer_list <Model*> submodels );

};

#endif