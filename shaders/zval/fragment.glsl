#version 330 core

//Color from vertex shader
//Output color
in vec2 v_uv_o;

uniform sampler2D texture_sampler;

out vec3 color;


void main()
{


	//vec3 light = vec3( 5, 0, 0 );
	//vec3 light_world = (mat_view * vec4(light, 1)).xyz;

	//vec3 light_dir = v_pos_world - light_world;
	//loat theta = dot(normalize(light_dir), normalize(v_normal_camera));

	//Just apply the color from vertex shader
	//color = vec3( 1, 1, 1 );//model_color;// * cosTheta;

	color = vec3( abs( v_uv_o ), 0 );//vec3( gl_FragCoord.z * 0.5 + 0.5 );//texture(texture_sampler, v_uv_o).rgb;

}
