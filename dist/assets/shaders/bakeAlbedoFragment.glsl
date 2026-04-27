#version 330 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 position;

uniform sampler2D grassAlbedo;
uniform sampler2D stoneAlbedo;

float getSlope(vec3 normalVector)
{
	vec3 upVector = vec3(0.0, 1.0, 0.0);
	float vectorsDot = dot(normalVector, upVector);
	float slopeRadians = acos(clamp(vectorsDot, -1.0, 1.0));
	float slopeDegrees = degrees(slopeRadians);
	
	return slopeDegrees;
}

void main()
{
    vec3 norm = normalize(normal);

    vec3 grass = texture(grassAlbedo, uv * 5).rgb;
    vec3 stone = texture(stoneAlbedo, uv * 5).rgb;

    float slope = getSlope(norm);
    float blend = smoothstep(30.0, 55.0, slope);

    vec3 albedo = mix(grass, stone, blend);

    vec3 result = albedo.rgb;
    FragColor = vec4(result, 1.0);
}