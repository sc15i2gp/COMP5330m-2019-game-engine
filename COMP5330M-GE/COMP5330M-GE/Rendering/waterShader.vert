#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColour;

layout(std140, binding = 0) uniform model_block
{
	mat4 model;
};

layout(std140, binding = 1) uniform view_projection_block
{
	vec4 view_origin;
	mat4 view;
	mat4 projection;
	float time;
};



layout(location = 0) out vec4 vertexColour;
layout(location = 1) out float f;

void main()
{
	gl_Position = projection * view * model * vec4(inPosition, 1.0);
	vertexColour = inColour;
	f = time;
}
