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

void main()
{
	//Ambient Light
	vec3 ambient = getAmbient(lightColor);

	//Diffuse Light
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diffuse = getDiffuse(norm, lightDir);

	//Specular Light
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 specular = getSpecular(norm, lightDir, viewDir);

	//Shadow Calculation
	float shadow = 0.0f;
	vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
	if(lightCoords.z <= 1.0f)
	{
		lightCoords = (lightCoords + 1.0f) / 2.0f;

		float closestDepth = texture(shadowMap, lightCoords.xy).r;
		float currentDepth = lightCoords.z;

		if(currentDepth > closestDepth)
		{
			shadow = 1.0f;
		}
	}

	vec3 result = ((diffuse * (1.0f - shadow) + ambient) + specular * (1.0f - shadow)) * color;

	FragColor = vec4(result, 1.0f);
}