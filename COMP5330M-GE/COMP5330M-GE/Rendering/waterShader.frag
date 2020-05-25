#version 460 core

//The vertex positions in world space
layout(location = 0) in vec4 vertexColour;
layout(location = 1) in float f;
layout(location = 2) in vec2 textureCoordinate;

out vec4 outColour;

void main()
{
	outColour = vertexColour;
}
