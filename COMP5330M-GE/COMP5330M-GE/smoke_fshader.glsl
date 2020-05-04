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
uniform sampler3D temperature_field;

layout(std140, binding = 7) uniform smoke_sim_block
{
	float max_density;
	float max_temperature;
	float world_volume_coefficient;
};

void main()
{
	vec4 min_temp_colour = vec4(0.5, 0.5, 0.5, 0.0);
	vec4 max_temp_colour = vec4(1.0, 0.0, 0.0, 0.0);

	vec3 ray_origin = vec3(view_origin);
	vec3 ray_direction = normalize(fragment_position - ray_origin);
	float total_density = 0.0;
	
	ivec3 density_field_dimensions = textureSize(density_field, 0);

	float total_temperature = 0.0;

	for(int i = 0; i < 20; ++i)
	{//March along ray n times
		//Compute spatial position along ray
		vec3 position_along_ray = world_volume_coefficient * (ray_origin + i*(ray_direction))/(density_field_dimensions);
		//Sample density field
		float temperature = texture(temperature_field, position_along_ray).x;
		total_temperature += (1.0 - total_density/max_density)*temperature;
		float density = texture(density_field, position_along_ray).x;
		total_density += density;
	}
	total_density /= max_density;
	total_temperature /= max_temperature;

	//colour = vec4(1.0) - vec4(total_density, total_density, total_density, 0.0);
	colour = (1.0 - total_temperature)*min_temp_colour + total_temperature*max_temp_colour;
	colour.a = total_density;
}
