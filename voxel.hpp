#ifndef VOXEL_HPP
#define VOXEL_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Voxel
{
	private:

		//Coordinates and size
		float x, y, z;
		float scale;

	public:

		//Render function
		void draw(GLuint program_id, glm::mat4 camera_matrix);
		Voxel( float x, float y, float z );
};

#endif