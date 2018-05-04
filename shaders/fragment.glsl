#version 330 core

//Color from vertex shader
in  vec3 fiColor;

//Output color
out vec3 foColor;

void main()
{
	//Just apply the color from vertex shader
    foColor = fiColor;
}