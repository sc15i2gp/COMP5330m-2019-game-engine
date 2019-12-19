#include "Graphics.h"

void set_uniform_mat4(GLuint shader, const char* uniform_name, GLfloat* matrix)
{
	GLint matrix_location = glGetUniformLocation(shader, uniform_name);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix);
}

void set_model_matrix(GLuint shader, Matrix4x4 model_mat)
{
	set_uniform_mat4(shader, "model", &model_mat[0][0]);
}

void set_view_matrix(GLuint shader, Matrix4x4 view_mat)
{
	set_uniform_mat4(shader, "view", &view_mat[0][0]);
}

void set_projection_matrix(GLuint shader, Matrix4x4 projection_mat)
{
	set_uniform_mat4(shader, "projection", &projection_mat[0][0]);
}

bool initialise_graphics()
{
	//Initialise opengl
	if (!initialise_opengl(get_window()))
	{
		//TODO: Error handling
		OutputDebugString("Error: Unable to initialise OpenGL!\n");
		return false;
	}
	glEnable(GL_DEPTH_TEST);
	return true;
}

void draw(Drawable d)
{
	glBindVertexArray(d.vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, d.vertex_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.index_buffer);

	glDrawElements(GL_TRIANGLES, d.number_of_indices, GL_UNSIGNED_INT, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Drawable buffer_mesh(Mesh_vertex* vertices, int number_of_vertices, GLuint* indices, int number_of_indices)
{
	Drawable mesh_drawable = {};
	glGenVertexArrays(1, &mesh_drawable.vertex_array);
	glGenBuffers(1, &mesh_drawable.vertex_buffer);
	glGenBuffers(1, &mesh_drawable.index_buffer);

	glBindVertexArray(mesh_drawable.vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, mesh_drawable.vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, number_of_vertices * sizeof(Mesh_vertex), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh_vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh_vertex), (void*)sizeof(Vector3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_drawable.index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, number_of_indices * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	mesh_drawable.number_of_indices = number_of_indices;

	return mesh_drawable;
}

void set_window_clear_colour(Vector3 c)
{
	glClearColor(c.r, c.g, c.b, 1.0f);
}

void begin_render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, get_window_width(), get_window_height());
}

void use_shader(GLuint shader)
{
	glUseProgram(shader);
}

GLuint load_shader_program(const char* v_shader_path, const char* f_shader_path)
{
	char* v_shader_src = read_file(v_shader_path);
	char* f_shader_src = read_file(f_shader_path);

	GLuint shader = create_shader_program(v_shader_src, f_shader_src);

	free(v_shader_src);
	free(f_shader_src);

	return shader;
}