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

layout(std140, binding = 7) uniform density_block
{
	float max_density;
	float world_volume_coefficient;
};

void main()
{
	vec3 ray_origin = vec3(view_origin);
	vec3 ray_direction = normalize(fragment_position - ray_origin);
	float total_density = 0.0f;
	
	ivec3 density_field_dimensions = textureSize(density_field, 0);

	for(int i = 0; i < 20; ++i)
	{//March along ray n times
		//Compute spatial position along ray
		vec3 position_along_ray = world_volume_coefficient * (ray_origin + i*(ray_direction))/(density_field_dimensions);
		//Sample density field
		float density = texture(density_field, position_along_ray).x;
		total_density += density;
	}
	total_density /= max_density;
	colour = vec4(1.0) - vec4(total_density, total_density, total_density, 0.0);
}
