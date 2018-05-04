#version 330 core
layout(location = 0) in vec3 vPosition; //Vertex position (modelspace)
layout(location = 1) in vec3 vColor;    //Vertex color

//Model, view and projection matrices
uniform mat4 mmvp;

//Fragment color output
out vec3 fColor;

void main( )
{
	//Project vertex
    //gl_Position = mmvp * vec4(vPosition, 1);
    
	//Idiot map
	gl_Position.xyz = vPosition;
	gl_Position.w = 1.0;


	//Just pass on the color to the fragment shader
	fColor = vColor;
}
