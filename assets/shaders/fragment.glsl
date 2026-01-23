#version 330 core
out vec4 FragColor;

in vec3 color;
in vec3 normal;
in vec3 fragPos;
in vec4 fragPosLight;

uniform sampler2D shadowMap;
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
    vec3 projectionCoords = fragPosLight.xyz / fragPosLight.w;
    projectionCoords = projectionCoords * 0.5 + 0.5;

    if(projectionCoords.x < 0.0 || projectionCoords.x > 1.0 || projectionCoords.y < 0.0 || projectionCoords.y > 1.0)
	{
		return 0.0;
	}

    float closestDepth = texture(shadowMap, projectionCoords.xy).r;
    float currentDepth = projectionCoords.z;

    float bias = max(0.005, 0.05 * (1.0 - dot(norm, lightDir)));
	if(currentDepth - bias > closestDepth)
	{
		return 1.0;
	}
	else
	{
		return 0.0;
	}
}

void main()
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);

    float shadow = calculateShadow(fragPosLight, norm, lightDir);

    vec3 ambient = getAmbient(lightColor);
    float diffuseFactor = getDiffuse(norm, lightDir);
    vec3 specular = getSpecular(norm, lightDir, viewDir);

    vec3 result = ((diffuseFactor * (1.0 - shadow) + ambient) + specular * (1.0 - shadow)) * color;
    FragColor = vec4(result, 1.0);
}