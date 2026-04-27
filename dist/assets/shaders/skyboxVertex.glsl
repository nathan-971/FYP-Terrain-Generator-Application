#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 texCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    texCoords = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
    gl_Position = gl_Position.xyww;
}