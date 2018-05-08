#version 330 core


in vec3 v_eye_camera;
in vec3 v_normal_camera;
in vec3 v_pos_world;
in mat4 mat_view;

uniform mat4 sun_view;

in vec3 v_pos_sun;

void main()
{
	//fragmentdepth = gl_FragCoord.z;
	//gl_FragDepth = //0.20;//1;//gl_FragCoord.z;
	//color = vec3(0, 1,0);
}