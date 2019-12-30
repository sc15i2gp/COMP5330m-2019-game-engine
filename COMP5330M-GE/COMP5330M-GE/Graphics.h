#pragma once
#include <stdlib.h>
#include "OpenGL.h"
#include "Maths.h"

struct Mesh_vertex
{
	Vector3 position;
	Vector3 normal;
};

struct Mesh
{
	int number_of_vertices;
	int number_of_indices;
	Mesh_vertex* vertices;
	GLuint* face_indices;
};

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

#define MAX_SHADER_COUNT 4
struct Graphics_Table
{
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
};

extern Graphics_Table __graphics;

void __set_model_matrix(Graphics_Table*, Matrix4x4);
void __set_view_matrix(Graphics_Table*, Matrix4x4);
void __set_projection_matrix(Graphics_Table*, Matrix4x4);
void __set_material(Graphics_Table*, Material);
void __set_view_origin(Graphics_Table*, Vector3);
void __set_direction_light_direction(Graphics_Table*, GLuint light_number, Vector3 light_direction); //0 <= light_number < 32
void __set_direction_light_blinn_phong_properties(Graphics_Table*, GLuint light_number, Material light_properties);
void __switch_direction_light(Graphics_Table*, GLuint light_number, GLboolean active);
void __switch_point_light(Graphics_Table*, GLuint light_number, GLboolean active);
void __set_point_light_position(Graphics_Table*, GLuint light_number, Vector3 position);
void __set_point_light_blinn_phong_properties(Graphics_Table*, GLuint light_number, Material light_properties);
void __set_point_light_attenuation_properties(Graphics_Table*, GLuint light_number, GLfloat constant, GLfloat linear, GLfloat quadratic);
void __switch_spot_light(Graphics_Table*, GLuint light_number, GLboolean active);
void __set_spot_light_position(Graphics_Table*, GLuint light_number, Vector3 position);
void __set_spot_light_direction(Graphics_Table*, GLuint light_number, Vector3 direction);
void __set_spot_light_blinn_phong_properties(Graphics_Table*, GLuint light_number, Material light_properties);
void __set_spot_light_inner_cutoff(Graphics_Table*, GLuint light_number, GLfloat angle);
void __set_spot_light_outer_cutoff(Graphics_Table*, GLuint light_number, GLfloat angle);
void __set_spot_light_attenuation_properties(Graphics_Table*, GLuint light_number, GLfloat constant, GLfloat linear, GLfloat quadratic);
void __set_max_height(Graphics_Table*, GLfloat max_height);
void __use_shader(Graphics_Table*, int shader);
int __load_shader_program(Graphics_Table*, const char* v_shader_path, const char* f_shader_path);

void draw(Drawable);

//Interleaved positions and normals
Drawable buffer_mesh(Mesh_vertex*, int number_of_vertices, GLuint* indices, int number_of_indices);

Drawable buffer_sphere_mesh(float radius, int slice_count = 16, int stack_count = 16);
Drawable buffer_cube_mesh(float width, float height, float depth);
Drawable buffer_cylinder_mesh(float radius, float height, int slice_count = 16);



void set_window_clear_colour(Vector3 colour);
void begin_render();

bool initialise_graphics();

#define set_model_matrix(m) __set_model_matrix(&__graphics, m)
#define set_view_matrix(m) __set_view_matrix(&__graphics, m)
#define set_projection_matrix(m) __set_projection_matrix(&__graphics, m)
#define set_material(m) __set_material(&__graphics, m)
#define set_view_origin(v) __set_view_origin(&__graphics, v)
#define activate_direction_light(l) __switch_direction_light(&__graphics, l, true)
#define deactivate_direction_light(l) __switch_direction_light(&__graphics, l, false)
#define set_direction_light_direction(l, d) __set_direction_light_direction(&__graphics, l, d)
#define set_direction_light_blinn_phong_properties(l, m) __set_direction_light_blinn_phong_properties(&__graphics, l, m)
#define activate_point_light(l) __switch_point_light(&__graphics, l, true)
#define deactivate_point_light(l) __switch_point_light(&__graphics, l, false)
#define set_point_light_position(l, p) __set_point_light_position(&__graphics, l, p)
#define set_point_light_blinn_phong_properties(l, m) __set_point_light_blinn_phong_properties(&__graphics, l, m)
#define set_point_light_attenuation_properties(l, c, s, q) __set_point_light_attenuation_properties(&__graphics, l, c, s, q)
#define activate_spot_light(l) __switch_spot_light(&__graphics, l, true)
#define deactivate_spot_light(l) __switch_spot_light(&__graphics, l, false)
#define set_spot_light_position(l, p) __set_spot_light_position(&__graphics, l, p)
#define set_spot_light_direction(l, d) __set_spot_light_direction(&__graphics, l, d)
#define set_spot_light_blinn_phong_properties(l, m) __set_spot_light_blinn_phong_properties(&__graphics, l, m)
#define set_spot_light_inner_cutoff(l, i) __set_spot_light_inner_cutoff(&__graphics, l, i)
#define set_spot_light_outer_cutoff(l, o) __set_spot_light_outer_cutoff(&__graphics, l, o)
#define set_spot_light_attenuation_properties(n, c, l, q) __set_spot_light_attenuation_properties(&__graphics, n, c, l, q)
#define set_max_height(h) __set_max_height(&__graphics, h)
#define use_shader(s) __use_shader(&__graphics, s)
#define load_shader_program(v, f) __load_shader_program(&__graphics, v, f)