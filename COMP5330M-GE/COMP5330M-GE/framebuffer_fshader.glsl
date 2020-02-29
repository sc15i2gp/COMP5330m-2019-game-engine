#version 460 core

in vec2 texture_coordinates;
out vec4 colour;

uniform sampler2D scene_texture;
uniform sampler2D smoke_texture;

void main()
{
	vec4 scene_colour = texture(scene_texture, texture_coordinates);
	vec4 smoke_colour = texture(smoke_texture, texture_coordinates);
	colour = scene_colour * smoke_colour;
}
