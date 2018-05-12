#version 330 core
layout(location = 0) in vec3 v_pos_model; //Vertex position (modelspace)
layout(location = 1) in vec2 v_uv;        //Vertex UV
layout(location = 2) in vec3 v_normal_model; //Vertex normal (modelspace)

//Model, view and projection matrices
uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;
uniform mat4 depth_bias;

//UV map output
out vec2 v_uv_o;

//Vertex position in world and cameraspace
out vec3 v_pos_world;
out vec3 v_pos_camera;

//Vector from vertex to the camera (in camera space)
out vec3 v_eye_camera;
out vec3 v_normal_camera;
uniform mat4 sun_view;

out vec3 v_pos_sun;


out VS_OUT {
    vec3 FragPos;
	vec3 normalLightSpace;
    vec4 FragPosLightSpace;
} vs_out;

void main( )
{
	//Camera transformation
	mat4 mat_camera = mat_projection * mat_view;

	//Vertex position in world
	v_pos_world = (mat_model * vec4(v_pos_model, 1)).xyz;

	//Veretx position in camera space (camera is at (0, 0))
	v_pos_camera = (mat_view * vec4(v_pos_world, 1)).xyz;

	//Position on screen	
	gl_Position = mat_projection * vec4(v_pos_camera, 1);

	//Sun
	//gl_Position = sun_view *  mat_model  * vec4(v_pos_model, 1);

	//From vertex to camera
	v_eye_camera = vec3(0, 0, 0) - v_pos_camera;
	v_normal_camera = (mat_view * mat_model * vec4(v_normal_model, 0)).xyz;

	//Just pass on the color to the fragment shader
	v_uv_o = v_uv;


	vs_out.FragPos = vec3(mat_model * vec4(v_pos_model, 1.0));	
	vs_out.normalLightSpace = (sun_view * mat_model * vec4(v_normal_model, 0)).xyz;
    vs_out.FragPosLightSpace = sun_view * vec4(vs_out.FragPos, 1.0);


}
