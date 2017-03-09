#version 410

layout (location=0) in vec4 position;
layout (location=1) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;

out vec3 normalV;

void main()
{
    normalV = normal;
    gl_Position = projection * view * position;
}
