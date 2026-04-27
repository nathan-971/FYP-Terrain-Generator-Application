#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec2 uv;

out vec3 fragPos;
out vec3 normal;

out vec4 fragPosLight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    fragPos = worldPos.xyz;
    fragPosLight = lightSpaceMatrix * worldPos;
    normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    uv = aUV;
    gl_Position = projection * view * worldPos;
}