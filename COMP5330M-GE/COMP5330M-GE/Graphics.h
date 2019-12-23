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
	float specular;
	float ambient;
	float diffuse;
	float shininess;
};

void set_model_matrix(GLuint shader, Matrix4x4);
void set_view_matrix(GLuint shader, Matrix4x4);
void set_projection_matrix(GLuint shader, Matrix4x4);
void set_material(Material);
void use_shader(GLuint);
void draw(Drawable);

//Separate positions and normals
Drawable buffer_mesh(Vector3* positions, Vector3* normals, int number_of_vertices, GLuint* indices, int number_of_indices);

//Interleaved positions and normals
Drawable buffer_mesh(Mesh_vertex*, int number_of_vertices, GLuint* indices, int number_of_indices);

Drawable buffer_sphere_mesh(float radius, int slice_count = 16, int stack_count = 16);
Drawable buffer_cube_mesh(float width, float height, float depth);
Drawable buffer_cylinder_mesh(float radius, float height, int slice_count = 16);

GLuint load_shader_program(const char* v_shader_path, const char* f_shader_path);

void set_window_clear_colour(Vector3 colour);
void begin_render();

bool initialise_graphics();