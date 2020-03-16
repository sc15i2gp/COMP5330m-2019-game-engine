#version 460 core

//The vertex positions in world space
layout(location = 0) in vec4 vertexColour;

out vec4 outColour;

void main()
{
	outColour = vertexColour;
}
