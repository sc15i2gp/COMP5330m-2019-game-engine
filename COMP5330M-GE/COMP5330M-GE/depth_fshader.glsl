#version 460 core

in vec3 normal;
in vec3 fragment_position;
in vec3 view_position;

out vec4 colour;

void main()
{
	colour = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0);
}
