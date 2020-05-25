#version 460 core

//The vertex positions in world space
layout(location = 0) in vec4 vertexColour;
layout(location = 1) in float f;

out vec4 outColour;

void main()
{
	outColour = vec4(f, 0.0f, 0.0f, 1.0f);
}
