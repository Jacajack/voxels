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
uniform sampler2D shadowMap;
uniform vec4 model_tint;

uniform mat4 sun_view;

in vec3 v_pos_sun;
in vec4 shadow;

in VS_OUT {
    vec3 FragPos;
    vec4 FragPosLightSpace;
} fs_in;


float shadcal( vec4 fragpos )
{
	vec3 projCoords = fragpos.xyz / fragpos.w;
	projCoords = projCoords * 0.5 + 0.5;
	float bias = 0.005;
	float closestDepth = texture( shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float shadow = 0;


	vec2 texelSize = 1.0 / textureSize( shadowMap, 0 );
	for ( int x = -2; x <= 2; x++ )
	{
		for ( int y = -2; y <= 2; y++ )
		{
			float pcfd = texture( shadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
			shadow += currentDepth - bias > pcfd ? 0 : 1;
		}
	}

	shadow /= 25;


	if ( projCoords.z > 1 )
		shadow = 1;

	return shadow;
}

void main()
{
	//Cosine of angle between model normal and vertex->camera vector
	float cosTheta = clamp(dot(normalize(v_normal_camera), normalize(v_eye_camera)), 0, 1);
	
	vec3 model_color = model_tint.xyz * model_tint.w + texture(texture_sampler, v_uv_o).rgb * (1 - model_tint.w);


	//float visi = texture( shadowMap, vec3(shadow.xy, shadow.z/shadow.w) );

	//float visi = texture(shadowMap, vec3(v_uv_o, 1)); //shadow.z;//texture( shadowMap, vec3(shadow.xy, shadow.z) );

	//vec3 light = vec3( 5, 0, 0 );
	//vec3 light_world = (mat_view * vec4(light, 1)).xyz;

	//vec3 light_dir = v_pos_world - light_world;

	//Just apply the color from vertex shader
	color =  model_color * clamp(
		0.8 * cosTheta * shadcal(fs_in.FragPosLightSpace) +
		0.2 * cosTheta

	, 0, 1);
	//color = vec3(gl_FragCoord.z);
	//float d = texture( shadowMap, )
	//color = vec3(1,0,0);

}