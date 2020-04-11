#version 460 core

layout (location = 0) in vec3 in_pos;

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
	vec4 vertex_position = vec4(in_pos.xy, 1.0, 1.0);
	mat4 v = inverse(view);
	fragment_position = vec3(v * vec4(in_pos.xy, -1.0, 1.0));
	gl_Position = vec4(in_pos.xy, 0.0, 1.0);
}
