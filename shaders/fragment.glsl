#version 330 core

//Color from vertex shader
//Output color
in vec2 UV;
out vec3 color;

uniform sampler2D texture_sampler;

void main()
{
	//Just apply the color from vertex shader
    color = texture(texture_sampler, UV).rgb;
	//vec3(1.0,1.0,1.0);//fColor;
}