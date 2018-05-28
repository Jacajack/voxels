#version 330 core

//Color from vertex shader
//Output color
in vec2 v_uv_o;

uniform sampler2D texture_sampler;
layout(location = 0) out vec3 color;


void main()
{


	//vec3 light = vec3( 5, 0, 0 );
	//vec3 light_world = (mat_view * vec4(light, 1)).xyz;

	//vec3 light_dir = v_pos_world - light_world;
	//loat theta = dot(normalize(light_dir), normalize(v_normal_camera));

	//Just apply the color from vertex shader
	//color = vec3( 1, 1, 1 );//model_color;// * cosTheta;

	color = texture(texture_sampler, v_uv_o).rgb;

}
