#pragma once
#include <stdlib.h>
#include "OpenGL.h"
#include "Platform.h"
#include "Maths.h"
#include "Mesh.h"

struct Drawable
{
	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint index_buffer;
	int number_of_indices;
};

struct Material
{
	Vector3 specular;
	Vector3 ambient;
	Vector3 diffuse;
	float shininess;
};

//This definition of the Material struct is necessary so that it is simple to buffer a Material to the shader
//since GLSL has strict alignment rules for struct members, this struct definition matches that alignment
struct Shader_Material
{
	Vector4 ambient;
	Vector4 diffuse;
	Vector3 specular;
	float shininess;
};

struct View_Projection_Block
{
	Vector4 view_position;
	Matrix4x4 view_matrix;
	Matrix4x4 projection_matrix;
};

struct Shader_Direction_Light
{
	Vector4 direction;
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
};

struct Shader_Spot_Light
{
	Vector4 position;
	Vector4 direction;

	Vector4 ambient;
	Vector4 diffuse;
	Vector3 specular;

	float inner_cutoff;
	float outer_cutoff;

	float constant;
	float linear;
	float quadratic;
};

struct Shader_Point_Light
{
	Vector4 position;

	Vector4 ambient;
	Vector4 diffuse;
	Vector3 specular;

	//Terms used to calculate attenuation in computing point light intensity
	float constant;
	float linear;
	float quadratic;

	Vector2 padding;
};

template <typename T>
struct Lights_Block
{
	Vector4 in_use[32];
	T lights[32];
};

//This class contains mostly shader data which needs to persist throughout the game's running
#define MAX_SHADER_COUNT 8
#define MAX_FRAMEBUFFER_COUNT 4
#define MAX_VOLUME_COUNT 4

#define DEFAULT_FRAMEBUFFER -1

class Graphics_Table
{
public:
	bool __initialise_graphics();
	void __set_model_matrix(Matrix4x4);
	void __set_view_matrix(Matrix4x4);
	void __set_projection_matrix(Matrix4x4);
	void __set_material(Material);
	void __set_view_origin(Vector3);
	void __set_direction_light_direction(GLuint light_number, Vector3 light_direction); //0 <= light_number < 32
	void __set_direction_light_blinn_phong_properties(GLuint light_number, Material light_properties);
	void __switch_direction_light(GLuint light_number, GLboolean active);
	void __switch_point_light(GLuint light_number, GLboolean active);
	void __set_point_light_position(GLuint light_number, Vector3 position);
	void __set_point_light_blinn_phong_properties(GLuint light_number, Material light_properties);
	void __set_point_light_attenuation_properties(GLuint light_number, GLfloat constant, GLfloat linear, GLfloat quadratic);
	void __switch_spot_light(GLuint light_number, GLboolean active);
	void __set_spot_light_position(GLuint light_number, Vector3 position);
	void __set_spot_light_direction(GLuint light_number, Vector3 direction);
	void __set_spot_light_blinn_phong_properties(GLuint light_number, Material light_properties);
	void __set_spot_light_inner_cutoff(GLuint light_number, GLfloat angle);
	void __set_spot_light_outer_cutoff(GLuint light_number, GLfloat angle);
	void __set_spot_light_attenuation_properties(GLuint light_number, GLfloat constant, GLfloat linear, GLfloat quadratic);
	void __set_max_height(GLfloat max_height);
	void __set_shader_sampler_uniform(int shader, char* sampler_name, int sampler_unit);
	void __use_shader(int shader);
	int __load_shader_program(const char* v_shader_path, const char* f_shader_path);
	GLuint __buffer_texture(GLuint texture_width, GLuint texture_height, float* texture_data, GLenum format = GL_RGB);
	void __use_texture(GLuint texture, GLuint texture_unit = 0, GLenum texture_target = GL_TEXTURE_2D);
	int __alloc_framebuffer(int width, int height, int framebuffer = -1);
	void __use_framebuffer(int framebuffer = -1);
	void __use_framebuffer_texture(int framebuffer = -1, int texture_unit = 0);
	void __resize_framebuffer(int width, int height, int framebuffer);
	void __resize_framebuffers(int width, int height);
	int __create_volume_texture(int width, int height, int depth);
	void __use_volume_texture(int volume_texture, GLuint texture_unit = 0);
	void __buffer_volume_data(int volume_texture, int width, int height, int depth, float* volume_data, GLenum type = GL_FLOAT, GLenum format = GL_RED);

private:
	//Uniform buffer objects
	GLuint material_buffer;
	GLuint model_matrix_buffer;
	GLuint view_projection_matrix_buffer;
	GLuint direction_lights_buffer;
	GLuint spot_lights_buffer;
	GLuint point_lights_buffer;
	GLuint max_height_buffer;

	//Shaders
	GLuint shaders[MAX_SHADER_COUNT];

	//Volume textures
	GLuint volume_textures[MAX_VOLUME_COUNT];

	//Framebuffers
	GLuint framebuffers[MAX_FRAMEBUFFER_COUNT];
	GLuint framebuffer_textures[MAX_FRAMEBUFFER_COUNT];
	GLuint framebuffer_renderbuffers[MAX_FRAMEBUFFER_COUNT];
};

extern Graphics_Table __graphics;

void draw_as_polygons();
void draw_as_wireframes();
void draw(Drawable);

//Interleaved positions and normals
Drawable buffer_mesh(Mesh_vertex*, int number_of_vertices, GLuint* indices, int number_of_indices);
Drawable buffer_mesh(Mesh);

Drawable buffer_sphere_mesh(float radius, int slice_count = 16, int stack_count = 16);
Drawable buffer_cube_mesh(float width, float height, float depth);
Drawable buffer_cylinder_mesh(float radius, float height, int slice_count = 16);


void set_window_clear_colour(Vector3 colour);
void begin_render();

#define set_model_matrix(m)									__graphics.__set_model_matrix(m)
#define set_view_matrix(m)									__graphics.__set_view_matrix(m)
#define set_projection_matrix(m)							__graphics.__set_projection_matrix(m)
#define set_material(m)										__graphics.__set_material(m)
#define set_view_origin(v)									__graphics.__set_view_origin(v)
#define activate_direction_light(l)							__graphics.__switch_direction_light(l, true)
#define deactivate_direction_light(l)						__graphics.__switch_direction_light(l, false)
#define set_direction_light_direction(l, d)					__graphics.__set_direction_light_direction(l, d)
#define set_direction_light_blinn_phong_properties(l, m)	__graphics.__set_direction_light_blinn_phong_properties(l, m)
#define activate_point_light(l)								__graphics.__switch_point_light(l, true)
#define deactivate_point_light(l)							__graphics.__switch_point_light(l, false)
#define set_point_light_position(l, p)						__graphics.__set_point_light_position(l, p)
#define set_point_light_blinn_phong_properties(l, m)		__graphics.__set_point_light_blinn_phong_properties(l, m)
#define set_point_light_attenuation_properties(l, c, s, q)	__graphics.__set_point_light_attenuation_properties(l, c, s, q)
#define activate_spot_light(l)								__graphics.__switch_spot_light(l, true)
#define deactivate_spot_light(l)							__graphics.__switch_spot_light(l, false)
#define set_spot_light_position(l, p)						__graphics.__set_spot_light_position(l, p)
#define set_spot_light_direction(l, d)						__graphics.__set_spot_light_direction(l, d)
#define set_spot_light_blinn_phong_properties(l, m)			__graphics.__set_spot_light_blinn_phong_properties(l, m)
#define set_spot_light_inner_cutoff(l, i)					__graphics.__set_spot_light_inner_cutoff(l, i)
#define set_spot_light_outer_cutoff(l, o)					__graphics.__set_spot_light_outer_cutoff(l, o)
#define set_spot_light_attenuation_properties(n, c, l, q)	__graphics.__set_spot_light_attenuation_properties(n, c, l, q)
#define set_max_height(h)									__graphics.__set_max_height(h)
#define set_shader_sampler_uniform(s, n, u)					__graphics.__set_shader_sampler_uniform(s, n, u)
#define use_shader(s)										__graphics.__use_shader(s)
#define load_shader_program(v, f)							__graphics.__load_shader_program(v, f)
#define initialise_graphics()								__graphics.__initialise_graphics()
#define buffer_texture(w, h, d, f)							__graphics.__buffer_texture(w, h, d, f)
#define use_texture(...)									__graphics.__use_texture(__VA_ARGS__)
#define use_volume_texture(v, u)							__graphics.__use_volume_texture(v, u)
#define create_volume_texture(...)							__graphics.__create_volume_texture(__VA_ARGS__)
#define buffer_volume_data(...)								__graphics.__buffer_volume_data(__VA_ARGS__)
#define alloc_framebuffer(w, h)								__graphics.__alloc_framebuffer(w, h)
#define use_framebuffer(f)									__graphics.__use_framebuffer(f)
#define use_framebuffer_texture(...)						__graphics.__use_framebuffer_texture(__VA_ARGS__)
#define resize_framebuffers(w, h)							__graphics.__resize_framebuffers(w, h)