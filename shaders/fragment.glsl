#version 330 core

//Color from vertex shader
//Output color
in vec2 v_uv_o;
out vec3 color;


in vec3 v_eye_camera;
in vec3 v_normal_camera;
in vec3 v_pos_world;
in mat4 mat_view;

uniform sampler2D texture_sampler;

void main()
{
	float cosTheta = abs(dot(normalize(v_normal_camera), normalize(v_eye_camera)));


	//vec3 light = vec3( 5, 0, 0 );
	//vec3 light_world = (mat_view * vec4(light, 1)).xyz;

	//vec3 light_dir = v_pos_world - light_world;
	f//loat theta = dot(normalize(light_dir), normalize(v_normal_camera));

	//Just apply the color from vertex shader
	color = texture(texture_sampler, v_uv_o).rgb * cosTheta;

}