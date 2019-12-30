#version 460 core

in vec3 normal;
in vec3 fragment_position;
in vec3 view_positon;

out vec4 colour;

layout(binding = 6) uniform max_height_block
{
	float max_height;
};

//Set colour based on terrain height
void main()
{
	float height = fragment_position.y / max_height;
	colour = vec4(height, height, height, 1.0);
}
