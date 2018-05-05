#version 330 core

//Color from vertex shader
in  vec3 fColor;

//Output color
out vec3 color;

void main()
{
	//Just apply the color from vertex shader
    color = fColor;
}