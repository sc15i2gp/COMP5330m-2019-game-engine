#version 460 core

in vec2 texture_coordinates;
in vec3 fragment_position;

out vec4 colour;

uniform sampler2D fire_texture;

void main()
{
	colour = (texture(fire_texture, texture_coordinates) * vec4(1.0, 0.3, 0.0, 1.0));
}
