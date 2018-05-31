#include <lobor/texturepeek.hpp>
#include <lobor/lobor.hpp>

//Performs window update
void lobor::Texturepeek::update( )
{
	//Use default framebuffer
	Window::use( );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glClear( GL_COLOR_BUFFER_BIT );
	
	//Use the texture
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, this->texture );

	//Pick texture sampler
	this->shader.use( );
	glUniform1i( this->shader.uniform( "texture_sampler" ), 0 );

	//Use VAO
	glBindVertexArray( this->vao );
	glEnableVertexAttribArray( 0 ); //Vertex data
	glEnableVertexAttribArray( 1 ); //UV data

	//Attribute 0 - vertex position
	glBindBuffer( GL_ARRAY_BUFFER, this->vertex_buffer_id );
	glVertexAttribPointer(
		0, //Attribute ID
		3, //Size
		GL_FLOAT, //Type
		GL_FALSE, //Normalized
		0, //Stride
		NULL //Array buffer offset
	);

	//Attribute 1 - UV
	glBindBuffer( GL_ARRAY_BUFFER, this->uv_buffer_id );
	glVertexAttribPointer(
		1, //Attribute ID
		2, //Size
		GL_FLOAT, //Type
		GL_FALSE, //Normalized
		0, //Stride
		NULL //Array buffer offset
	);

	//Draw the quad
	glDrawArrays( GL_TRIANGLES, 0, 6 );

	glDisableVertexAttribArray( 0 ); //Vertex data
	glDisableVertexAttribArray( 1 ); //UV data

	//Update the window
	Window::swap_buffers( );
}

//The default constructor
lobor::Texturepeek::Texturepeek( int width, int height, std::string title, GLuint texture, GLFWwindow *share )
	: Window( width, height, title, share ),
	
	//Special shader is needed
	shader( 
		{
			{"shaders/texturepeek/fsh.glsl", GL_FRAGMENT_SHADER},
			{"shaders/texturepeek/vsh.glsl", GL_VERTEX_SHADER}
		},
		{
			"texture_sampler"
		}
	)
{
	//Vertices and UV data
	float vertices[18] = 
	{
		-1, -1, 0, //BL
		1, -1, 0, //BR
		-1, 1, 0, //TL
		-1, 1, 0, //TL
		1, -1, 0, //BR
		1, 1, 0, //TR
	};

	float uvs[12] = 
	{
		0, 0, //BL
		1, 0, //BR
		0, 1, //TL
		0, 1, //TL
		1, 0, //BR
		1, 1, //TR
	};

	this->texture = texture;

	//Use the window
	Window::use( );

	//Each context needs its own VAO
	glGenVertexArrays( 1, &this->vao );
	glBindVertexArray( this->vao );

	//Buffer the quad data
	glGenBuffers( 1, &this->vertex_buffer_id );
	glBindBuffer( GL_ARRAY_BUFFER, this->vertex_buffer_id );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
	glGenBuffers( 1, &this->uv_buffer_id );
	glBindBuffer( GL_ARRAY_BUFFER, this->uv_buffer_id );
	glBufferData( GL_ARRAY_BUFFER, sizeof( uvs ), uvs, GL_STATIC_DRAW );
}

//The destructor
lobor::Texturepeek::~Texturepeek( )
{
	Window::use( );

	glDeleteBuffers( 1, &this->vertex_buffer_id );
	glDeleteBuffers( 1, &this->uv_buffer_id );
	glDeleteVertexArrays( 1, &this->vao );
}