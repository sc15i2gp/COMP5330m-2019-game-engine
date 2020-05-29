#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColour;
layout(location = 2) in vec2 inTexture;

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
layout(location = 2) out vec2 textureCoordinate;

void main()
{
	float xDisplacement = sin(2.0f * inPosition.x + time / 1000.0f) * 0.2f;
	float zDisplacement = cos(2.0f * inPosition.z + time / 1000.0f) * 0.2f;

	float finalY = xDisplacement + zDisplacement;

	gl_Position = projection * view * model * vec4(inPosition.x, finalY, inPosition.z, 1.0);
	vertexColour = inColour;

	textureCoordinate = inTexture;
}
