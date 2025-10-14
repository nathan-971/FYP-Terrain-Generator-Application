#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;

out vec3 normal;
out vec3 color;
out vec3 fragPos;
out vec4 fragPosLight;

uniform mat4 model;
uniform mat4 camMatrix;
uniform mat4 lightProjectionMatrix;

void main()
{
	normal = mat3(transpose(inverse(model))) * aNormal; //Temp - Convert to CPU Operation (Inverse is expensive)
	color = aColor;
	fragPos = vec3(model * vec4(aPos, 1.0));
	fragPosLight = lightProjectionMatrix * vec4(aPos, 1.0);
	gl_Position = camMatrix * model * vec4(aPos, 1.0);
}