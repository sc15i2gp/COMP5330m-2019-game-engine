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
			if(height < 0.0f)	colour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	else	if(height < 0.3f)	colour = vec4(0.0f, 0.412f, 0.58f, 1.0f);
	else	if(height < 0.8f)	colour = vec4(0.486f, 0.988f, 0.0f, 1.0f);
	else						colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
