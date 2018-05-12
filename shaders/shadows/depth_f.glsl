#version 330 core


in vec3 v_eye_camera;
in vec3 v_normal_camera;
in vec3 v_pos_world;
in mat4 mat_view;

uniform mat4 sun_view;

in vec3 v_pos_sun;

out vec2 color;

void main()
{
	float depth = gl_FragCoord.z;
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	float moment2 = depth * depth  + 0.5 * ( dx * dx + dy * dy );

	color = vec2( depth, moment2 );;
}