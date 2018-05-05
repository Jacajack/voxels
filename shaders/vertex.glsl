#version 330 core
layout(location = 0) in vec3 vPosition; //Vertex position (modelspace)
layout(location = 1) in vec2 vUV;       //Vertex UV

//Model, view and projection matrices
uniform mat4 model;
uniform mat4 camera;

//Fragment color output
out vec2 UV;

void main( )
{
	//Project vertex
    gl_Position = camera * model * vec4(vPosition, 1);

	//Just pass on the color to the fragment shader
	//fColor = vColor;
	UV = vUV;
}
