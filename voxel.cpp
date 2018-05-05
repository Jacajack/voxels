#include "voxel.hpp"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

//Cube vertices
static const GLfloat voxel_vertices[] = 
{
	//1a
	1.0, 	1.0, 	1.0,
	1.0,	-1.0,	1.0,
	1.0, 	1.0,	-1.0,
	1.0, 	-1.0, 	-1.0,
	1.0,	1.0,	-1.0,
	1.0, 	-1.0,	1.0,

	//1b
	-1.0, 	1.0, 	1.0,
	-1.0,	-1.0,	1.0,
	-1.0, 	1.0,	-1.0,
	-1.0, 	-1.0, 	-1.0,
	-1.0,	1.0,	-1.0,
	-1.0, 	-1.0,	1.0,

	//2a
	1.0, 	1.0, 	1.0,
	-1.0,	1.0,	1.0,
	-1.0, 	1.0,	-1.0,
	-1.0, 	1.0, 	-1.0,
	1.0,	1.0,	-1.0,
	1.0, 	1.0,	1.0,
	
	//2b
	1.0, 	-1.0, 	1.0,
	-1.0,	-1.0,	1.0,
	-1.0, 	-1.0,	-1.0,
	-1.0, 	-1.0, 	-1.0,
	1.0,	-1.0,	-1.0,
	1.0, 	-1.0,	1.0,

	//3a
	1.0, 	1.0,	1.0,
	-1.0,	1.0,	1.0,
	-1.0,	-1.0,	1.0,
	-1.0, 	-1.0,	1.0,
	1.0,	-1.0,	1.0,
	1.0,	1.0,	1.0,

	//3b
	1.0, 	1.0,	-1.0,
	-1.0,	1.0,	-1.0,
	-1.0,	-1.0,	-1.0,
	-1.0, 	-1.0,	-1.0,
	1.0,	-1.0,	-1.0,
	1.0,	1.0,	-1.0,

};

//Cube colors
static const GLfloat voxel_colors[] = 
{
	//1a
	0.0, 	0.0, 	1.0,
	0.0,	0.0,	1.0,
	0.0, 	0.0,	1.0,
	0.0, 	1.0, 	0.0,
	0.0,	1.0,	0.0,
	0.0, 	1.0,	0.0,

	//1b
	0.0, 	0.0, 	1.0,
	0.0,	0.0,	1.0,
	0.0, 	0.0,	1.0,
	0.0, 	1.0, 	0.0,
	0.0,	1.0,	0.0,
	0.0, 	1.0,	0.0,

	//2a
	1.0, 	0.0, 	0.0,
	1.0,	0.0,	0.0,
	1.0, 	0.0,	0.0,
	1.0, 	1.0, 	0.0,
	1.0,	1.0,	0.0,
	1.0, 	1.0,	0.0,
	
	//2b
	1.0, 	0.0, 	0.0,
	1.0,	0.0,	0.0,
	1.0, 	0.0,	0.0,
	1.0, 	1.0, 	0.0,
	1.0,	1.0,	0.0,
	1.0, 	1.0,	0.0,

	//3a
	1.0, 	0.0, 	1.0,
	1.0,	0.0,	1.0,
	1.0, 	0.0,	1.0,
	0.0, 	1.0, 	1.0,
	0.0,	1.0,	1.0,
	0.0, 	1.0,	1.0,

	//3b
	1.0, 	0.0, 	1.0,
	1.0,	0.0,	1.0,
	1.0, 	0.0,	1.0,
	0.0, 	1.0, 	1.0,
	0.0,	1.0,	1.0,
	0.0, 	1.0,	1.0,
};

//Renders the voxel on the screen
//This is very likely to be the worst render routine ever
void Voxel::draw(GLuint program_id, glm::mat4 camera_matrix)
{
	//The model matrix
	glm::mat4 model_matrix = glm::translate(glm::vec3(this->x, this->y, this->z)) * glm::scale(glm::vec3(this->scale));
	glm::mat4 mvp_matrix = camera_matrix * model_matrix;

	//Apply view and projection matrices
	GLuint model_matrix_id = glGetUniformLocation(program_id, "MVP");
	glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, &mvp_matrix[0][0]);

	//Create vertex buffer (I honestly have no idea if it's meant to be created each time....)
	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	//Load buffer with vertices data
	glBufferData(GL_ARRAY_BUFFER, sizeof(voxel_vertices), voxel_vertices, GL_STATIC_DRAW);

	GLuint color_buffer;
	glGenBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);

	//Load buffer with colors data
	glBufferData(GL_ARRAY_BUFFER, sizeof(voxel_colors), voxel_colors, GL_STATIC_DRAW);

	//Enable attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//Attribute 0 - vertex position
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glVertexAttribPointer(
		0, //Attribute ID
		3, //Size
		GL_FLOAT, //Type
		GL_FALSE, //Normalized
		0, //Stride
		NULL //Array buffer offset
	);

	//Attribute 1 - vertex color
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glVertexAttribPointer(
		1, //Attribute ID
		3, //Size
		GL_FLOAT, //Type
		GL_FALSE, //Normalized
		0, //Stride
		NULL //Array buffer offset
	);
	
	//Draw
	glDrawArrays(GL_TRIANGLES, 0, 3 * 12); //Draw 12 triangles

	//Disable attributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//Free buffers
	glDeleteBuffers(1, &vertex_buffer);
	glDeleteBuffers(1, &color_buffer);
}

//Voxel constructor
Voxel::Voxel( float x, float y, float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->scale = 0.5;
}