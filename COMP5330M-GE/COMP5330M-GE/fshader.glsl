#version 450 core

in vec3 normal;
out vec4 colour;

void main()
{
//colour = vec4(1.0, 0.5, 0.2, 1.0);
colour = vec4(normal, 1.0);
}
