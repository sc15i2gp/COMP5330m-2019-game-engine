#version 450 core

float cos_deg(float f)
{
	float pi = 3.14159265;
	return cos(f * pi / 180.0);
}

in vec3 normal;
in vec3 frag_pos;
out vec4 colour;

//Blinn-Phong material
struct bp_material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct direction_light
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct point_light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//Terms used to calculate attenuation in computing point light intensity
	float constant;
	float linear;
	float quadratic;
};

struct spot_light
{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float inner_cutoff_angle;
	float outer_cutoff_angle;

	float constant;
	float linear;
	float quadratic;
};

uniform bp_material material;

uniform direction_light direction_lights[32];
uniform spot_light spot_lights[32];
uniform point_light point_lights[32];

vec3 compute_direction_light_intensity(direction_light d_light, bp_material material, vec3 normal, vec3 view_direction)
{
	vec3 light_direction = normalize(-d_light.direction);
	vec3 halfway = normalize(light_direction+view_direction);

	float diffuse_impact = max(dot(normal, light_direction), 0.0);
	float specular_impact = pow(max(dot(normal, halfway), 0.0), material.shininess);

	vec3 ambient = d_light.ambient * material.ambient;
	vec3 diffuse = diffuse_impact * d_light.diffuse * material.diffuse;
	vec3 specular = specular_impact * d_light.specular * material.specular;
	
	return ambient + diffuse + specular;
}

vec3 compute_point_light_intensity(point_light p_light, bp_material material, vec3 normal, vec3 fragment_position, vec3 view_direction)
{
	vec3 light_direction = normalize(p_light.position - fragment_position);
	vec3 halfway = normalize(p_light.position + view_direction);

	float diffuse_impact = max(dot(normal, light_direction), 0.0);
	float specular_impact = pow(max(dot(normal, halfway), 0.0), material.shininess);

	float distance = length(p_light.position - fragment_position);
	float attenuation = 1.0 / (p_light.constant + p_light.linear * distance + p_light.quadratic * distance * distance);

	vec3 ambient = attenuation * p_light.ambient * material.ambient;
	vec3 diffuse = attenuation * diffuse_impact * p_light.diffuse * material.diffuse;
	vec3 specular = attenuation * specular_impact * p_light.specular * material.specular;

	return ambient + diffuse + specular;
}

vec3 compute_spot_light_intensity(spot_light s_light, bp_material material, vec3 normal, vec3 fragment_position, vec3 view_direction)
{
	vec3 light_direction = normalize(s_light.position - fragment_position);
	
	float distance = length(s_light.position - fragment_position);
	float attenuation = 1.0 / (s_light.constant + s_light.linear * distance + s_light.quadratic * distance * distance);

	vec3 halfway = normalize(light_direction + view_direction);
	float diffuse_impact = max(dot(normal, light_direction), 0.0);
	float specular_impact = pow(max(dot(normal, halfway), 0.0), material.shininess);

	float theta = dot(light_direction, normalize(-s_light.direction));
	float epsilon = cos_deg(s_light.inner_cutoff_angle) - cos_deg(s_light.outer_cutoff_angle);
	float intensity = clamp((theta - cos_deg(s_light.outer_cutoff_angle)) / epsilon, 0.0, 1.0);

	vec3 ambient = attenuation * s_light.ambient * material.ambient;
	vec3 diffuse = intensity * attenuation * diffuse_impact * s_light.diffuse * material.diffuse;
	vec3 specular = intensity * attenuation * specular_impact * s_light.specular * material.specular;

	return ambient + diffuse + specular;
}

void main()
{
vec3 view_pos = vec3(0.0, 0.0, 1.0);

direction_light d_light;
d_light.direction = vec3(0.0, -1.0, 0.0);
d_light.ambient = vec3(0.0, 0.3, 1.0);
d_light.diffuse = vec3(0.0, 0.3, 1.0);
d_light.specular = vec3(0.0, 0.3, 1.0);

bp_material material;
material.ambient = vec3(0.247, 0.199, 0.075);
material.diffuse = vec3(0.752, 0.606, 0.226);
material.specular = vec3(0.628, 0.556, 0.367);
material.shininess = 51.2;

vec3 view_direction = normalize(view_pos - frag_pos);
vec3 light_intensity = compute_direction_light_intensity(d_light, material, normal, view_direction);

colour = vec4(light_intensity, 1.0);
}
