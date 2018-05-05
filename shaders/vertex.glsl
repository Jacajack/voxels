#version 330 core
layout(location = 0) in vec3 vPosition; //Vertex position (modelspace)
layout(location = 1) in vec3 vColor;    //Vertex color

//Model, view and projection matrices
uniform mat4 MVP;

//Fragment color output
out vec3 fColor;

void main( )
{
	//Project vertex
    gl_Position = MVP * vec4(vPosition, 1);

	//Just pass on the color to the fragment shader
	fColor = vColor;
}
