#pragma once
#include "OpenGL.h"
#include "Maths.h"

//Draw
/*			glUseProgram(shader);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);*/

//Buffer mesh
/*
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);*/

//Prepare window for rendering
/*			glClearColor(0.98f, 0.85f, 0.86f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, get_window_width(), get_window_height());*/

//Update window
/*		HDC window_device_context = GetDC(window); //Used to swap buffers
					//Swap window buffers
		SwapBuffers(window_device_context);*/
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

Drawable buffer_sphere_mesh(float radius);
Drawable buffer_cube_mesh(float width, float height);
Drawable buffer_cylinder_mesh(float radius, float height);

GLuint load_shader_program(const char* v_shader_path, const char* f_shader_path);

void set_window_clear_colour(Vector3 colour);
void begin_render();

bool initialise_graphics();