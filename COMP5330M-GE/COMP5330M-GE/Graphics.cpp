#include "Graphics.h"

Graphics_Table __graphics;

//init_data = NULL means that the mem is allocated but no data is buffered
GLuint alloc_gpu_mem(int size, GLenum target, GLenum usage, void* init_data = NULL)
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(target, buffer);
	glBufferData(target, size, init_data, usage);
	glBindBuffer(target, 0);
	return buffer;
}

void copy_to_gpu_mem(void* src_data, GLuint dst_gpu_buffer, GLuint offset, GLsizei range, GLenum target)
{
	glBindBuffer(target, dst_gpu_buffer);
	void* buffer_ptr = glMapBufferRange(target, (GLintptr)offset, range, GL_MAP_WRITE_BIT);
	copy_mem(src_data, buffer_ptr, range);
	glUnmapBuffer(target);
	glBindBuffer(target, 0);
}

void __set_model_matrix(Graphics_Table* graphics, Matrix4x4 model_mat)
{
	//set_uniform_mat4(shader, "model", &model_mat[0][0]);
	copy_to_gpu_mem(&model_mat, graphics->model_matrix_buffer, 0, sizeof(Matrix4x4), GL_UNIFORM_BUFFER);
}

void __set_view_matrix(Graphics_Table* graphics, Matrix4x4 view_mat)
{
	//set_uniform_mat4(shader, "view", &view_mat[0][0]);
	copy_to_gpu_mem(&view_mat, graphics->view_projection_matrix_buffer, 
		offsetof(View_Projection_Block, view_matrix), sizeof(Matrix4x4), 
		GL_UNIFORM_BUFFER);
}

void __set_projection_matrix(Graphics_Table* graphics, Matrix4x4 projection_mat)
{
	//set_uniform_mat4(shader, "projection", &projection_mat[0][0]);
	copy_to_gpu_mem(&projection_mat, graphics->view_projection_matrix_buffer, 
		offsetof(View_Projection_Block, projection_matrix), sizeof(Matrix4x4), 
		GL_UNIFORM_BUFFER);
}

Shader_Material reformat_for_shader(Material m)
{
	Shader_Material s_m;
	s_m.ambient.xyz = m.ambient;
	s_m.diffuse.xyz = m.diffuse;
	s_m.specular = m.specular;
	s_m.shininess = m.shininess;
	return s_m;
}

void __set_material(Graphics_Table* graphics, Material material)
{
	Shader_Material s_material = reformat_for_shader(material);

	copy_to_gpu_mem(&s_material, graphics->material_buffer, 0, sizeof(Shader_Material), GL_UNIFORM_BUFFER);
}

void __set_view_origin(Graphics_Table* graphics, Vector3 view_origin)
{
	Vector4 view_origin_4(view_origin, 0.0);
	copy_to_gpu_mem(&view_origin_4, graphics->view_projection_matrix_buffer, offsetof(View_Projection_Block, view_position), sizeof(Vector4), GL_UNIFORM_BUFFER);
}

void __set_direction_light_direction(Graphics_Table* graphics, GLuint light_number, Vector3 light_direction)
{
	GLintptr offset = offsetof(Lights_Block<Shader_Direction_Light>, lights[light_number].direction);
	Vector4 light_direction_4(light_direction, 0.0);
	copy_to_gpu_mem(&light_direction_4, graphics->direction_lights_buffer, offset, sizeof(Vector4), GL_UNIFORM_BUFFER);
}

void __set_direction_light_blinn_phong_properties(Graphics_Table* graphics, GLuint light_number, Material light_properties)
{
	Shader_Material s_light_properties = reformat_for_shader(light_properties);
	GLintptr offset = offsetof(Lights_Block<Shader_Direction_Light>, lights[light_number].ambient);
	GLsizei range = 2 * sizeof(Vector4) + sizeof(Vector3);
	copy_to_gpu_mem(&s_light_properties, graphics->direction_lights_buffer, offset, range, GL_UNIFORM_BUFFER);
}

void __switch_direction_light(Graphics_Table* graphics, GLuint light_number, GLboolean active)
{
	GLintptr offset = offsetof(Lights_Block<Shader_Direction_Light>, in_use[light_number]);
	copy_to_gpu_mem(&active, graphics->direction_lights_buffer, offset, sizeof(GLuint), GL_UNIFORM_BUFFER);
}

void __switch_point_light(Graphics_Table* graphics, GLuint light_number, GLboolean active)
{
	GLintptr offset = offsetof(Lights_Block<Shader_Point_Light>, in_use[light_number]);
	copy_to_gpu_mem(&active, graphics->point_lights_buffer, offset, sizeof(GLuint), GL_UNIFORM_BUFFER);
}

void __set_point_light_position(Graphics_Table* graphics, GLuint light_number, Vector3 position)
{
	Vector4 position_4(position, 1.0);
	OutputDebugStringf("V: %f %f %f %f\n", position_4.x, position_4.y, position_4.z, position_4.w);
	GLintptr offset = offsetof(Lights_Block<Shader_Point_Light>, lights[light_number].position);
	copy_to_gpu_mem(&position_4, graphics->point_lights_buffer, offset, sizeof(Vector4), GL_UNIFORM_BUFFER);
}

void __set_point_light_blinn_phong_properties(Graphics_Table* graphics, GLuint light_number, Material light_properties)
{
	Shader_Material s_light_properties = reformat_for_shader(light_properties);
	GLintptr offset = offsetof(Lights_Block<Shader_Point_Light>, lights[light_number].ambient);
	GLsizei range = 2 * sizeof(Vector4) + sizeof(Vector3);
	copy_to_gpu_mem(&s_light_properties, graphics->point_lights_buffer, offset, range, GL_UNIFORM_BUFFER);
}

void __set_point_light_attenuation_properties(Graphics_Table* graphics, GLuint light_number, GLfloat constant, GLfloat linear, GLfloat quadratic)
{
	Vector3 attenuation_properties(constant, linear, quadratic);
	GLintptr offset = offsetof(Lights_Block<Shader_Point_Light>, lights[light_number].constant);
	copy_to_gpu_mem(&attenuation_properties, graphics->point_lights_buffer, offset, sizeof(Vector3), GL_UNIFORM_BUFFER);
}

void __switch_spot_light(Graphics_Table* graphics, GLuint light_number, GLboolean active)
{
	GLintptr offset = offsetof(Lights_Block<Shader_Spot_Light>, in_use[light_number]);
	copy_to_gpu_mem(&active, graphics->spot_lights_buffer, offset, sizeof(GLuint), GL_UNIFORM_BUFFER);
}

void __set_spot_light_position(Graphics_Table* graphics, GLuint light_number, Vector3 position)
{
	Vector4 position_4(position, 1.0);
	GLintptr offset = offsetof(Lights_Block<Shader_Spot_Light>, lights[light_number].position);
	copy_to_gpu_mem(&position_4, graphics->spot_lights_buffer, offset, sizeof(Vector4), GL_UNIFORM_BUFFER);
}

void __set_spot_light_direction(Graphics_Table* graphics, GLuint light_number, Vector3 direction)
{
	Vector4 direction_4(direction, 1.0);
	GLintptr offset = offsetof(Lights_Block<Shader_Spot_Light>, lights[light_number].direction);
	GLsizei range = sizeof(Vector4);
	copy_to_gpu_mem(&direction_4, graphics->spot_lights_buffer, offset, range, GL_UNIFORM_BUFFER);
}

void __set_spot_light_blinn_phong_properties(Graphics_Table* graphics, GLuint light_number, Material light_properties)
{
	Shader_Material s_light_properties = reformat_for_shader(light_properties);
	GLintptr offset = offsetof(Lights_Block<Shader_Spot_Light>, lights[light_number].ambient);
	GLsizei range = 2 * sizeof(Vector4) + sizeof(Vector3);
	copy_to_gpu_mem(&s_light_properties, graphics->spot_lights_buffer, offset, range, GL_UNIFORM_BUFFER);
}

void __set_spot_light_inner_cutoff(Graphics_Table* graphics, GLuint light_number, GLfloat angle)
{
	GLintptr offset = offsetof(Lights_Block<Shader_Spot_Light>, lights[light_number].inner_cutoff);
	GLsizei range = sizeof(GLfloat);
	copy_to_gpu_mem(&angle, graphics->spot_lights_buffer, offset, range, GL_UNIFORM_BUFFER);
}

void __set_spot_light_outer_cutoff(Graphics_Table* graphics, GLuint light_number, GLfloat angle)
{
	GLintptr offset = offsetof(Lights_Block<Shader_Spot_Light>, lights[light_number].outer_cutoff);
	GLsizei range = sizeof(GLfloat);
	copy_to_gpu_mem(&angle, graphics->spot_lights_buffer, offset, range, GL_UNIFORM_BUFFER);
}

void __set_spot_light_attenuation_properties(Graphics_Table* graphics, GLuint light_number, GLfloat constant, GLfloat linear, GLfloat quadratic)
{
	Vector3 attenuation_properties(constant, linear, quadratic);
	GLintptr offset = offsetof(Lights_Block<Shader_Spot_Light>, lights[light_number].constant);
	GLsizei range = sizeof(Vector3);
	copy_to_gpu_mem(&attenuation_properties, graphics->spot_lights_buffer, offset, range, GL_UNIFORM_BUFFER);
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

void __use_shader(Graphics_Table* graphics)
{
	glUseProgram(graphics->shader);
}

GLuint alloc_and_bind_ubo(GLsizei size, GLuint binding, GLenum target, GLenum usage)
{
	void* init_data = alloc_mem(size); //Zeroes the ubo data
	GLuint ubo_buffer = alloc_gpu_mem(size, target, usage, init_data);
	glBindBufferBase(target, binding, ubo_buffer);
	return ubo_buffer;
}

GLuint compile_shader_src(char* shader_src, GLenum shader_type)
{
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shader_src, NULL);
	glCompileShader(shader);
	
	char info[2048] = {};
	GLint success;

	//NOTE: OpenGL/AMD/whoever is really shit, this caused a crash when a specific compiler error occurred on my machine
	//Error was referring to a uniform variable within a uniform block as "uniform_block.var_name" instead of just "var_name"
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, info);
		OutputDebugStringf("Error: Shader compilation failed: \n%s", info);
		return 0;
	}
	return shader;
}

GLuint create_shader_program(char* v_shader_src, char* f_shader_src)
{
	GLuint v_shader = compile_shader_src(v_shader_src, GL_VERTEX_SHADER);
	GLuint f_shader = compile_shader_src(f_shader_src, GL_FRAGMENT_SHADER);

	GLuint shader = glCreateProgram();
	glAttachShader(shader, v_shader);
	glAttachShader(shader, f_shader);
	glLinkProgram(shader);

	GLint success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{//If the shader program wasn't linked
		OutputDebugString("Failed to link shader program\n");
		return 0;
	}
	glDeleteShader(v_shader);
	glDeleteShader(f_shader);
	return shader;
}


void __load_shader_program(Graphics_Table* graphics, const char* v_shader_path, const char* f_shader_path)
{
	char* v_shader_src = read_file(v_shader_path);
	char* f_shader_src = read_file(f_shader_path);
	OutputDebugString("HERE\n");
	graphics->shader = create_shader_program(v_shader_src, f_shader_src);
	
	//Create + bind uniform buffers
	graphics->model_matrix_buffer = alloc_and_bind_ubo(sizeof(Matrix4x4), 0, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	graphics->view_projection_matrix_buffer = alloc_and_bind_ubo(sizeof(View_Projection_Block), 1, GL_UNIFORM_BUFFER, GL_STATIC_DRAW);
	graphics->material_buffer = alloc_and_bind_ubo(sizeof(Shader_Material), 2, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	graphics->direction_lights_buffer = alloc_and_bind_ubo(sizeof(Lights_Block<Shader_Direction_Light>), 3, GL_UNIFORM_BUFFER, GL_STATIC_DRAW);
	graphics->spot_lights_buffer = alloc_and_bind_ubo(sizeof(Lights_Block<Shader_Spot_Light>), 4, GL_UNIFORM_BUFFER, GL_STATIC_DRAW);
	graphics->point_lights_buffer = alloc_and_bind_ubo(sizeof(Lights_Block<Shader_Point_Light>), 5, GL_UNIFORM_BUFFER, GL_STATIC_DRAW);

	dealloc_mem(v_shader_src);
	dealloc_mem(f_shader_src);
}

//Generates a sphere vertex
//xz_turn = the proportion around the sphere's great circle in the xz plane
//y_turn = the proportion around one of the sphere's great circles in a y plane
Mesh_vertex sample_sphere_mesh_vertex(float radius, float xz_turn, float y_turn)
{
	Mesh_vertex v = {};
	float slice_angle = xz_turn * 360.0f;
	float stack_angle = 90.0f - 180.0f*y_turn;
	v.position[0] = radius * cos_deg(stack_angle) * cos_deg(slice_angle);
	v.position[1] = radius * sin_deg(stack_angle);
	v.position[2] = radius * cos_deg(stack_angle) * sin_deg(slice_angle);

	Vector3 n = {};
	n[0] = v.position[0];
	n[1] = v.position[1];
	n[2] = v.position[2];

	n = normalise(n);

	v.normal[0] = n[0];
	v.normal[1] = n[1];
	v.normal[2] = n[2];

	return v;
}

Drawable buffer_sphere_mesh(float radius, int number_of_slices, int number_of_stacks)
{
	//First and last vertices in array are the vertices at the sphere's poles
	int number_of_vertices = (number_of_stacks - 1)*number_of_slices + 2;
	int size_of_vertex_array = number_of_vertices * sizeof(Mesh_vertex);

	//Populate vertex data array (contains positions and normals)
	Mesh_vertex* sphere_vertex_array = (Mesh_vertex*)alloc_mem(size_of_vertex_array);

	sphere_vertex_array[0] = sample_sphere_mesh_vertex(radius, 0, 0);
	int sphere_vertex_offset = 0;
	for (float i = 0; i < number_of_slices; ++i)
	{//For each slice of the sphere
		float slice_angle = (float)(i)*360.0f / (float)(number_of_slices);
		for (float j = 1; j < number_of_stacks; ++j)
		{//For each stack in the slice
			float stack_angle = 90.0f - (float)(j)*180.0f / (float)(number_of_stacks);
			sphere_vertex_array[sphere_vertex_offset++] = sample_sphere_mesh_vertex(radius, i / (float)number_of_slices, j / (float)number_of_stacks);
		}
	}
	sphere_vertex_array[sphere_vertex_offset++] = sample_sphere_mesh_vertex(radius, 1.0f, 1.0f);
	sphere_vertex_array[sphere_vertex_offset] = sample_sphere_mesh_vertex(radius, 1.0f, 1.0f);
	//Populate face index array
	int number_of_triangles = 2 * (number_of_stacks - 1)*number_of_slices;
	int number_of_indices = 3 * number_of_triangles;
	int size_of_index_array = number_of_indices * sizeof(GLuint);
	GLuint* sphere_index_array = (GLuint*)alloc_mem(size_of_index_array);

	//First stack layer
	int sphere_index_offset = 0;
	for (int i = 0; i < number_of_slices; ++i)
	{//For each triangle in the first stack layer
		sphere_index_array[sphere_index_offset] = 0; //First vertex in vertex array, top pole
		sphere_index_array[sphere_index_offset + 1] = 1 + i * (number_of_stacks - 1);
		sphere_index_array[sphere_index_offset + 2] = 1 + ((i + 1) % number_of_slices) * (number_of_stacks - 1);
		sphere_index_offset += 3;
	}

	//Mid stack layers
	for (int i = 0; i < number_of_slices; ++i)
	{//For each slice
		for (int j = 1; j < number_of_stacks - 1; ++j)
		{//For each rectangle in the mid layers of the slice
			GLuint i_0 = i * (number_of_stacks - 1) + j;
			GLuint i_1 = i * (number_of_stacks - 1) + j + 1;
			GLuint i_2 = ((i + 1) % number_of_slices)*(number_of_stacks - 1) + j + 1;
			GLuint i_3 = ((i + 1) % number_of_slices)*(number_of_stacks - 1) + j;

			GLuint rect_indices[] = { i_0, i_1, i_2, i_2, i_3, i_0 };
			for (int k = 0; k < 6; ++k)
			{
				sphere_index_array[sphere_index_offset++] = rect_indices[k];
			}
		}
	}

	//Final stack layer
	for (int i = 0; i < number_of_slices; ++i)
	{
		sphere_index_array[sphere_index_offset] = 1 + i * (number_of_stacks - 1) + number_of_stacks - 2;
		sphere_index_array[sphere_index_offset + 1] = number_of_vertices - 1;
		sphere_index_array[sphere_index_offset + 2] = 1 + ((i + 1) % number_of_slices) * (number_of_stacks - 1) + number_of_stacks - 2;
		sphere_index_offset += 3;
	}
	OutputDebugStringf("Index array size: %d Index offset: %d\n", 3 * number_of_triangles, sphere_index_offset);
	//Buffer data
	Drawable sphere_mesh = buffer_mesh(sphere_vertex_array, number_of_vertices, sphere_index_array, number_of_indices);

	for (int i = 0; i < number_of_vertices; ++i)
	{
		Vector3 n = sphere_vertex_array[i].normal;
		Vector3 p = sphere_vertex_array[i].position;
		if (n == Vector3{ 0.0f, 0.0f, 0.0f })
		{
			OutputDebugStringf("0 normal at vertex %d with position: %f %f %f\n", i, p[0], p[1], p[2]);
		}
	}
	dealloc_mem(sphere_vertex_array);
	dealloc_mem(sphere_index_array);
	return sphere_mesh;
}

Drawable buffer_cube_mesh(float width, float height, float depth)
{
	//These used to center the cube about the origin
	float w = width / 2.0f;
	float h = height / 2.0f;
	float d = depth / 2.0f;
	Mesh_vertex vertices[] =
	{
		//Front plane
		{{-w, h, d}, {0.0f, 0.0f, 1.0f}}, //Top left
		{{-w, -h, d}, {0.0f, 0.0f, 1.0f}}, //Bottom left
		{{w, -h, d}, {0.0f, 0.0f, 1.0f}}, //Bottom right
		{{w, h, d}, {0.0f, 0.0f, 1.0f}}, //Top right

		//Back plane
		{{w, h, -d}, {0.0f, 0.0f, -1.0f}}, //Top left
		{{w, -h, -d}, {0.0f, 0.0f, -1.0f}}, //Bottom left
		{{-w, -h, -d}, {0.0f, 0.0f, -1.0f}}, //Bottom right
		{{-w, h, -d}, {0.0f, 0.0f, -1.0f}}, //Top right

		//Top plane
		{{-w, h, -d}, {0.0f, 1.0f, 0.0f}}, //Top left
		{{-w, h, d}, {0.0f, 1.0f, 0.0f}}, //Bottom left
		{{w, h, d}, {0.0f, 1.0f, 0.0f}}, //Bottom right
		{{w, h, -d}, {0.0f, 1.0f, 0.0f}}, //Top right

		//Bottom plane
		{{-w, -h, d}, {0.0f, -1.0f, 0.0f}}, //Top left
		{{-w, -h, -d}, {0.0f, -1.0f, 0.0f}}, //Bottom left
		{{w, -h, -d}, {0.0f, -1.0f, 0.0f}}, //Bottom right
		{{w, -h, d}, {0.0f, -1.0f, 0.0f}}, //Top right

		//Left plane
		{{-w, h, -d}, {-1.0f, 0.0f, 0.0f}}, //Top left
		{{-w, -h, -d}, {-1.0f, 0.0f, 0.0f}}, //Bottom left
		{{-w, -h, d}, {-1.0f, 0.0f, 0.0f}}, //Bottom right
		{{-w, h, d}, {-1.0f, 0.0f, 0.0f}}, //Top right

		//Right plane
		{{w, h, d}, {1.0f, 0.0f, 0.0f}}, //Top left
		{{w, -h, d}, {1.0f, 0.0f, 0.0f}}, //Bottom left
		{{w, -h, -d}, {1.0f, 0.0f, 0.0f}}, //Bottom right
		{{w, h, -d}, {1.0f, 0.0f, 0.0f}} //Top right
	};
	int number_of_vertices = 4 * 6;

	GLuint indices[] =
	{
		0, 1, 2, 2, 3, 0, //Front plane
		4, 5, 6, 6, 7, 4, //Back plane
		8, 9, 10, 10, 11, 8, //Top plane
		12, 13, 14, 14, 15, 12, //Bottom plane
		16, 17, 18, 18, 19, 16, //Left plane
		20, 21, 22, 22, 23, 20 //Right plane
	};
	int number_of_indices = 6 * 6;

	return buffer_mesh(vertices, number_of_vertices, indices, number_of_indices);
}

Drawable buffer_cylinder_mesh(float radius, float height, int slice_count)
{
	//Used to center cylinder around origin
	float h = height / 2.0f;
	float slice_angle = 360.0f / slice_count;

	//The cylinder is made up of 2 circles and slice_count number of rectangles around the side
	//Each vertex needs to be defined twice, once with a normal for one circle and another for the rectangles
	//The number of vertices in a circle is slice_count + 1 (1 for the center vertex), 
	//therefore the number of vertices in circles = 2*slice_count+2
	//The number of vertices used for the rectangles = 2*slice_count
	//Therefore, the total number of vertices = 4*slice_count + 2
	int number_of_vertices = 4*slice_count + 2;
	Mesh_vertex* vertices = (Mesh_vertex*)alloc_mem(number_of_vertices*sizeof(Mesh_vertex));

	int vertex_offset = 0;

	//The first slice_count + 1 vertices are for the top circle
	//The next slice_count + 1 vertices are for the bottom circle
	//The remaining 2*slice_count vertices are for the side rectangles

	//Top circle
	vertices[vertex_offset++] = { {0.0f, h, 0.0f}, {0.0f, 1.0f, 0.0f} };
	for (int i = 0; i < slice_count ; ++i)
	{
		float x = radius*cos_deg(i*slice_angle);
		float z = -radius*sin_deg(i*slice_angle);
		Vector3 position(x, h, z);
		Vector3 normal(0.0f, 1.0f, 0.0f);
		vertices[vertex_offset++] = { position, normal };
	}

	//Bottom circle
	vertices[vertex_offset++] = { {0.0f, -h, 0.0f}, {0.0f, -1.0f, 0.0f} };
	for (int i = 0; i < slice_count; ++i)
	{
		float x = radius * cos_deg(i*slice_angle);
		float z = -radius * sin_deg(i*slice_angle);
		Vector3 position(x, -h, z);
		Vector3 normal(0.0f, -1.0f, 0.0f);
		vertices[vertex_offset++] = { position, normal };
	}

	//Side rectangles, top circle
	for (int i = 0; i < slice_count; ++i)
	{
		float x = radius * cos_deg(i*slice_angle);
		float z = -radius * sin_deg(i*slice_angle);
		Vector3 position(x, h, z);
		Vector3 normal = normalise(Vector3(x, 0.0f, z));
		vertices[vertex_offset++] = { position, normal };
	}

	//Side rectangles, bottom circle
	for (int i = 0; i < slice_count; ++i)
	{
		float x = radius * cos_deg(i*slice_angle);
		float z = -radius * sin_deg(i*slice_angle);
		Vector3 position(x, -h, z);
		Vector3 normal = normalise(Vector3(x, 0.0f, z));
		vertices[vertex_offset++] = { position, normal };
	}

	//Number of indices per triangle = 3
	//Number of triangles in the cylinder circles = 2*slice_count
	//Number of triangles in the side rectangles = 2*slice_count
	//Number of indices in the cylinder = 3*(2*slice_count + 2*slice_count) = 3*4*slice_count
	int number_of_indices = 3*4*slice_count;
	GLuint* indices = (GLuint*)alloc_mem(number_of_indices * sizeof(GLuint));

	int index_offset = 0;

	//The first 3*2*slice_count indices are for the top and bottom circles
	//The final 3*2*slice_count indices are for the side rectangles

	//Top circle
	int top_circle_offset = 0;
	for (int i = 0; i < slice_count; ++i)
	{
		indices[index_offset++] = top_circle_offset;
		indices[index_offset++] = top_circle_offset + i + 1;
		indices[index_offset++] = top_circle_offset + 1 + ((i + 1) % (slice_count));
	}

	//Bottom circle
	int bottom_circle_offset = slice_count + 1;
	for (int i = 0; i < slice_count; ++i)
	{
		indices[index_offset++] = bottom_circle_offset;
		indices[index_offset++] = bottom_circle_offset + slice_count - i;
		indices[index_offset++] = bottom_circle_offset + slice_count - ((i+1) % (slice_count));
	}

	//Side rectangles
	int rectangles_t_offset = 2 * slice_count + 2;
	int rectangles_b_offset = 3 * slice_count + 2;
	for (int i = 0; i < slice_count; ++i)
	{
		GLuint top_left = rectangles_t_offset + i;
		GLuint bottom_left = rectangles_b_offset + i;
		GLuint bottom_right = rectangles_b_offset + ((i+1) % slice_count);
		GLuint top_right = rectangles_t_offset + ((i+1) % slice_count);
		indices[index_offset++] = top_left;
		indices[index_offset++] = bottom_left;
		indices[index_offset++] = bottom_right;
		indices[index_offset++] = bottom_right;
		indices[index_offset++] = top_right;
		indices[index_offset++] = top_left;
	}

	Drawable cylinder = buffer_mesh(vertices, number_of_vertices, indices, number_of_indices);
	dealloc_mem(indices);
	dealloc_mem(vertices);
	return cylinder;
}


