#version 330 core

in vec2 v_uv_o;

uniform sampler2D texture_sampler;
layout(location = 0) out vec3 color;


void main()
{
	color = texture( texture_sampler, v_uv_o ).rgb;
	//color = vec3(1,0,0);//texture(texture_sampler, v_uv_o).rgb;
}
