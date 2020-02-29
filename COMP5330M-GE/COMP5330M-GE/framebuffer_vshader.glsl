#version 460 core

layout (location = 0) in vec3 in_pos;
layout (location = 2) in vec2 in_tex_coords;

out vec2 texture_coordinates;

void main()
{
	gl_Position = vec4(in_pos.xy, 0.0, 1.0);
	texture_coordinates = in_tex_coords;
}
