#version 460 core

in vec3 fragment_position;

out vec4 colour;

layout(std140, binding = 1) uniform view_projection_block
{
	vec4 view_origin;
	mat4 view;
	mat4 projection;
};

uniform sampler3D density_field;

void main()
{
	vec3 ray_origin = vec3(view_origin);
	vec3 ray_direction = normalize(fragment_position - ray_origin);
	float total_density = 0.0f;
	/*
	for(int i = 0; i < 1; ++i)
	{//March along ray 10 times
		//Compute spatial position along ray
		vec3 position_along_ray = ray_origin + i*ray_direction;
		//Sample density field
		float density = texture(density_field, position_along_ray).x;
		total_density += density;
	}
	*/
	total_density += texture(density_field, vec3(1.0)).x;
	if(total_density > 0.0) colour = vec4(1.0) - vec4(total_density, total_density, total_density, 0.0);
	else colour = vec4(1.0, 0.0, 0.0, 1.0);
}
