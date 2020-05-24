#version 460 core

in vec2 texture_coords;
out vec4 colour;

uniform sampler2D quad_texture;

void main()
{
	vec4 sampled_colour = texture(quad_texture, texture_coords);
	colour = vec4(sampled_colour.x, sampled_colour.x, sampled_colour.x, 1.0);
}
