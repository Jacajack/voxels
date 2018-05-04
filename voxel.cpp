#include "voxel.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

//Cube vertices
static const GLfloat voxel_vertices[] = 
{
	0.0, 	1.0, 	0.0,
	-1.0,	0.0,	0.0,
	1.0, 	0.0,	0.0,
};

//Renders the voxel on the screen
//This is very likely to be the worst render routine ever
void Voxel::draw( )
{
	//The model matrix
	glm::mat4 model_matrix = glm::translate(glm::vec3(this->x, this->y, this->z));


	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(voxel_vertices), voxel_vertices, GL_STATIC_DRAW);

	//Attribute 0 - vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, //Attribute ID
		3, //Size
		GL_FLOAT, //Type
		GL_FALSE, //Normalized
		0, //Stride
		NULL //Array buffer offset
	);
	glDrawArrays(GL_TRIANGLES, 0, 3); //Draw 3 vertices 
	glDisableVertexAttribArray(0);
}

//Voxel constructor
Voxel::Voxel( float x, float y, float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}