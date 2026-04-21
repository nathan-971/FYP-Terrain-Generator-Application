#version 330 core
out vec4 FragColor;

in vec2 uv;

in vec3 normal;
in vec3 fragPos;

in vec4 fragPosLight;

uniform sampler2D shadowMap;

uniform sampler2D grassAlbedo;
uniform sampler2D stoneAlbedo;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float ambientStrength;
uniform float specularStrength;

uniform int shininess;

vec3 getAmbient(vec3 lightColor)
{
	return ambientStrength * lightColor;
}

float getDiffuse(vec3 norm, vec3 lightDir)
{
	return max(dot(norm, lightDir), 0.0);
}

vec3 getSpecular(vec3 norm, vec3 lightDir, vec3 viewDir)
{
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	return specularStrength * spec * lightColor;
}

float calculateShadow(vec4 fragPosLight, vec3 norm, vec3 lightDir)
{
    vec3 projCoords = fragPosLight.xyz / fragPosLight.w;
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0)
	{
		return 0.0;
	}

    float currentDepth = projCoords.z;
    float bias = max(0.0005 * (1.0 - dot(norm, lightDir)), 0.00005);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(
                shadowMap,
                projCoords.xy + vec2(x, y) * texelSize
            ).r;

            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;
    return shadow;
}

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
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);

    // Texturing
    vec3 grass = texture(grassAlbedo, uv * 5).rgb;
    vec3 stone = texture(stoneAlbedo, uv * 5).rgb;

    float slope = getSlope(norm);
    float blend = smoothstep(30.0, 55.0, slope);

    vec3 albedo = mix(grass, stone, blend);

    //Shadow
    float shadow = calculateShadow(fragPosLight, norm, lightDir);

    //Lighting
    vec3 ambient = getAmbient(lightColor);
    float diffuseFactor = getDiffuse(norm, lightDir);
    vec3 specular = getSpecular(norm, lightDir, viewDir);
    vec3 lighting = ambient + diffuseFactor * (1.0 - shadow);

    vec3 result = albedo * lighting + specular * (1.0 - shadow);
    FragColor = vec4(result, 1.0);
}