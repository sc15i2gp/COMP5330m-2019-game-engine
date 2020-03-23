#version 460 core

layout(location = 0) in vec3 in_pos;
layout(location = 2) in vec2 in_texture_coords;

out vec2 texture_coords;

void main()
{
	gl_Position = vec4(in_pos, 1.0);
	texture_coords = in_texture_coords;
}
