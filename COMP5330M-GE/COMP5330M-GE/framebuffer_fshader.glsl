#version 460 core

in vec2 texture_coordinates;
out vec4 colour;

uniform sampler2D scene_texture;

void main()
{
	colour = texture(scene_texture, texture_coordinates);
}
