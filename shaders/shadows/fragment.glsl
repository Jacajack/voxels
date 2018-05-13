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
	vec3 normalLightSpace;
    vec4 FragPosLightSpace;
} fs_in;

float linstep( float low, float high, float v )
{
	return clamp( ( v - low ) / ( high - low ), 0, 1 );
}

float shadcal( vec4 fragpos, vec3 normal )
{
	vec3 projCoords = fragpos.xyz / fragpos.w;
	projCoords = projCoords * 0.5 + 0.5;

	vec2 moments = texture2D( shadowMap, projCoords.xy).rg;
	float currentDepth = projCoords.z;
	
	//return moments.x;//currentDepth -moments.x > 0 ? 0.5 : 0;// ? 0 : 1;

	float p = step( currentDepth, moments.x );
	float variance = max( moments.y - moments.x * moments.x, 0.0001 );
	float d = currentDepth - moments.x;
	float pMax = linstep( 0.2, 1.0, variance / (variance + d*d) );

	//return moments.y;
	return min(max(p, pMax), 1.0);
	//return p;

/*

	float bias = 0.005;

	//fragpos - in sun space
	//normal - in sun space
	//vec3 fragsun = (vec4(0,0,0,0) - fragpos).xyz;
	//float cosTheta = clamp(dot(normalize(normal), normalize(fragsun)), 0, 1);

	//return cosTheta;

	//float bias = 0.005*tan(acos(cosTheta));
	//bias = clamp(bias, 0,0.1);
	
	
	float closestDepth = texture( shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float shadow = 0;
*/
/*
	vec2 texelSize = 1.0 / textureSize( shadowMap, 0 );
	for ( int x = -1; x <= 1; x++ )
	{
		for ( int y = -1; y <= 1; y++ )
		{
			float pcfd = texture( shadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
			shadow += currentDepth - bias > pcfd ? 0 : 1;
		}
	}


	shadow /= 9;
*/
/*
	float tex  = texture( shadowMap, projCoords.xy ).r;
	//return tex;
	
	
	shadow = currentDepth - bias > tex ? 0 : 1;
*/
/*
	if ( projCoords.z > 1 )
		shadow = 1;

	return shadow;
*/
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
		0.8 *  shadcal(fs_in.FragPosLightSpace, fs_in.normalLightSpace) 
		+ 0.2 * cosTheta
	, 0, 1);
	//color = vec3(gl_FragCoord.z);
	//float d = texture( shadowMap, )
	//color = vec3(1,0,0);

}