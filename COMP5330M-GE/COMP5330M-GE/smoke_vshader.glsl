#version 460 core

layout (location = 0) in vec3 in_pos;

void main()
{
	gl_Position = vec4(in_pos.xy, 0.0, 1.0);
}
