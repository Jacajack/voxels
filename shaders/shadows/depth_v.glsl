#version 330 core
layout(location = 0) in vec3 v_pos_model; //Vertex position (modelspace)
layout(location = 1) in vec2 v_uv;        //Vertex UV

//Model, view and projection matrices
uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;
uniform mat4 sun_view;

//UV map output
out vec2 v_uv_o;

void main( )
{
	gl_Position = sun_view * mat_model  * vec4(v_pos_model, 1);
}
