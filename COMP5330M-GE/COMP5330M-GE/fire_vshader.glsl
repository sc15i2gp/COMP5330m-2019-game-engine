#version 460 core

layout (location = 0) in vec3 in_pos;
layout (location = 2) in vec2 in_tex_coords;

out vec2 texture_coordinates;
out vec3 fragment_position;

layout(std140, binding = 0) uniform model_block
{
	mat4 model;
};

layout(std140, binding = 1) uniform view_projection_block
{
	vec4 view_origin;
	mat4 view;
	mat4 projection;
};

void main()
{
	gl_Position = projection * view * model * vec4(in_pos, 1.0);
	fragment_position = vec3(model * vec4(in_pos, 1.0));
	texture_coordinates = in_tex_coords;
}
