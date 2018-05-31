#version 330 core
layout(location = 0) in vec3 v_pos_i; //Vertex position
layout(location = 1) in vec2 v_uv_i;  //Vertex UV

//Vertex pos, UV map output
out vec3 v_pos_o;
out vec2 v_uv_o;


void main( )
{
	v_pos_o = v_pos_i;
	v_uv_o = v_uv_i;
	gl_Position = vec4( v_pos_i, 1 );
}
