#version 460 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;

out vec3 normal;
out vec3 fragment_position;
out vec3 view_position;

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
	normal = mat3(transpose(inverse(model))) * in_normal;
	fragment_position = vec3(model * vec4(in_pos, 1.0));
	view_position = vec3(view_origin);
}
