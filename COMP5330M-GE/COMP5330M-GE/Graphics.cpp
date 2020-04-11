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

void Graphics_Table::__set_model_matrix(Matrix4x4 model_mat)
{
	//set_uniform_mat4(shader, "model", &model_mat[0][0]);
	copy_to_gpu_mem(&model_mat, this->model_matrix_buffer, 0, sizeof(Matrix4x4), GL_UNIFORM_BUFFER);
}

void Graphics_Table::__set_view_matrix(Matrix4x4 view_mat)
{
	//set_uniform_mat4(shader, "view", &view_mat[0][0]);
	copy_to_gpu_mem(&view_mat, this->view_projection_matrix_buffer, 
		offsetof(View_Projection_Block, view_matrix), sizeof(Matrix4x4), 
		GL_UNIFORM_BUFFER);
}

void Graphics_Table::__set_projection_matrix(Matrix4x4 projection_mat)
{
	//set_uniform_mat4(shader, "projection", &projection_mat[0][0]);
	copy_to_gpu_mem(&projection_mat, this->view_projection_matrix_buffer, 
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

void Graphics_Table::__set_material(Material material)
{
	Shader_Material s_material = reformat_for_shader(material);

	copy_to_gpu_mem(&s_material, this->material_buffer, 0, sizeof(Shader_Material), GL_UNIFORM_BUFFER);
}

void Graphics_Table::__set_view_origin(Vector3 view_origin)
{
	Vector4 view_origin_4(view_origin, 0.0);
	copy_to_gpu_mem(&view_origin_4, this->view_projection_matrix_buffer, offsetof(View_Projection_Block, view_position), sizeof(Vector4), GL_UNIFORM_BUFFER);
}

void Graphics_Table::__set_direction_light_direction(GLuint light_number, Vector3 light_direction)
{
	GLintptr offset = offsetof(Lights_Block<Shader_Direction_Light>, lights[light_number].direction);
	Vector4 light_direction_4(light_direction, 0.0);
	copy_to_gpu_mem(&light_direction_4, this->direction_lights_buffer, offset, sizeof(Vector4), GL_UNIFORM_BUFFER);
}

void Graphics_Table::__set_direction_light_blinn_phong_properties(GLuint light_number, Material light_properties)
{
	Shader_Material s_light_properties = reformat_for_shader(light_properties);
	GLintptr offset = offsetof(Lights_Block<Shader_Direction_Light>, lights[light_number].ambient);
	GLsizei range = 2 * sizeof(Vector4) + sizeof(Vector3);
	copy_to_gpu_mem(&s_light_properties, this->direction_lights_buffer, offset, range, GL_UNIFORM_BUFFER);
}

void Graphics_Table::__switch_direction_light(GLuint light_number, GLboolean active)
{
	GLintptr offset = offsetof(Lights_Block<Shader_Direction_Light>, in_use[light_number]);
	copy_to_gpu_mem(&active, this->direction_lights_buffer, offset, sizeof(GLuint), GL_UNIFORM_BUFFER);
}

void Graphics_Table::__switch_point_light(GLuint light_number, GLboolean active)
{
	GLintptr offset = offsetof(Lights_Block<Shader_Point_Light>, in_use[light_number]);
	copy_to_gpu_mem(&active, this->point_lights_buffer, offset, sizeof(GLuint), GL_UNIFORM_BUFFER);
}

void Graphics_Table::__set_point_light_position(GLuint light_number, Vector3 position)
{
	Vector4 position_4(position, 1.0);
	GLintptr offset = offsetof(Lights_Block<Shader_Point_Light>, lights[light_number].position);
	copy_to_gpu_mem(&position_4, this->point_lights_buffer, offset, sizeof(Vector4), GL_UNIFORM_BUFFER);
}

void Graphics_Table::__set_point_light_blinn_phong_properties(GLuint light_number, Material light_properties)
{
	Shader_Material s_light_properties = reformat_for_shader(light_properties);
	GLintptr offset = offsetof(Lights_Block<Shader_Point_Light>, lights[light_number].ambient);
	GLsizei range = 2 * sizeof(Vector4) + sizeof(Vector3);
	copy_to_gpu_mem(&s_light_properties, this->point_lights_buffer, offset, range, GL_UNIFORM_BUFFER);
}

void Graphics_Table::__set_point_light_attenuation_properties(GLuint light_number, GLfloat constant, GLfloat linear, GLfloat quadratic)
{
	Vector3 attenuation_properties(constant, linear, quadratic);
	GLintptr offset = offsetof(Lights_Block<Shader_Point_Light>, lights[light_number].constant);
	copy_to_gpu_mem(&attenuation_properties, this->point_lights_buffer, offset, sizeof(Vector3), GL_UNIFORM_BUFFER);
}

void Graphics_Table::__switch_spot_light(GLuint light_number, GLboolean active)
{
	GLintptr offset = offsetof(Lights_Block<Shader_Spot_Light>, in_use[light_number]);
	copy_to_gpu_mem(&active, this->spot_lights_buffer, offset, sizeof(GLuint), GL_UNIFORM_BUFFER);
}

void Graphics_Table::__set_spot_light_position(GLuint light_number, Vector3 position)
{
	Vector4 position_4(position, 1.0);
	GLintptr offset = offsetof(Lights_Block<Shader_Spot_Light>, lights[light_number].position);
	copy_to_gpu_mem(&position_4, this->spot_lights_buffer, offset, sizeof(Vector4), GL_UNIFORM_BUFFER);
}

void Graphics_Table::__set_spot_light_direction(GLuint light_number, Vector3 direction)
{
	Vector4 direction_4(direction, 1.0);
	GLintptr offset = offsetof(Lights_Block<Shader_Spot_Light>, lights[light_number].direction);
	GLsizei range = sizeof(Vector4);
	copy_to_gpu_mem(&direction_4, this->spot_lights_buffer, offset, range, GL_UNIFORM_BUFFER);
}

void Graphics_Table::__set_spot_light_blinn_phong_properties(GLuint light_number, Material light_properties)
{
	Shader_Material s_light_properties = reformat_for_shader(light_properties);
	GLintptr offset = offsetof(Lights_Block<Shader_Spot_Light>, lights[light_number].ambient);
	GLsizei range = 2 * sizeof(Vector4) + sizeof(Vector3);
	copy_to_gpu_mem(&s_light_properties, this->spot_lights_buffer, offset, range, GL_UNIFORM_BUFFER);
}

void Graphics_Table::__set_spot_light_inner_cutoff(GLuint light_number, GLfloat angle)
{
	GLintptr offset = offsetof(Lights_Block<Shader_Spot_Light>, lights[light_number].inner_cutoff);
	GLsizei range = sizeof(GLfloat);
	copy_to_gpu_mem(&angle, this->spot_lights_buffer, offset, range, GL_UNIFORM_BUFFER);
}

void Graphics_Table::__set_spot_light_outer_cutoff(GLuint light_number, GLfloat angle)
{
	GLintptr offset = offsetof(Lights_Block<Shader_Spot_Light>, lights[light_number].outer_cutoff);
	GLsizei range = sizeof(GLfloat);
	copy_to_gpu_mem(&angle, this->spot_lights_buffer, offset, range, GL_UNIFORM_BUFFER);
}

void Graphics_Table::__set_spot_light_attenuation_properties(GLuint light_number, GLfloat constant, GLfloat linear, GLfloat quadratic)
{
	Vector3 attenuation_properties(constant, linear, quadratic);
	GLintptr offset = offsetof(Lights_Block<Shader_Spot_Light>, lights[light_number].constant);
	GLsizei range = sizeof(Vector3);
	copy_to_gpu_mem(&attenuation_properties, this->spot_lights_buffer, offset, range, GL_UNIFORM_BUFFER);
}

void Graphics_Table::__set_max_height(GLfloat max_height)
{
	copy_to_gpu_mem(&max_height, this->max_height_buffer, 0, sizeof(GLfloat), GL_UNIFORM_BUFFER);
}

void Graphics_Table::__set_max_density(GLfloat max_density)
{
	copy_to_gpu_mem(&max_density, this->max_density_buffer, 0, sizeof(GLfloat), GL_UNIFORM_BUFFER);
}

void Graphics_Table::__set_shader_sampler_uniform(int shader, char* sampler_name, int sampler_unit)
{
	GL_ERROR_CHECK(use_shader(shader));
	GL_ERROR_CHECK(GLint sampler_location = glGetUniformLocation(this->shaders[shader], sampler_name));
	GL_ERROR_CHECK(glUniform1i(sampler_location, sampler_unit));
}

GLuint alloc_and_bind_ubo(GLsizei size, GLuint binding, GLenum target, GLenum usage)
{
	void* init_data = alloc_mem(size); //Zeroes the ubo data
	GLuint ubo_buffer = alloc_gpu_mem(size, target, usage, init_data);
	glBindBufferBase(target, binding, ubo_buffer);
	return ubo_buffer;
}

bool Graphics_Table::__initialise_graphics()
{
	//Initialise opengl
	if (!initialise_opengl(get_window()))
	{
		//TODO: Error handling
		OutputDebugString("Error: Unable to initialise OpenGL!\n");
		return false;
	}
	glEnable(GL_DEPTH_TEST);

	//Create + bind uniform buffers
	*this = {};
	this->model_matrix_buffer = alloc_and_bind_ubo(sizeof(Matrix4x4), 0, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	this->view_projection_matrix_buffer = alloc_and_bind_ubo(sizeof(View_Projection_Block), 1, GL_UNIFORM_BUFFER, GL_STATIC_DRAW);
	this->material_buffer = alloc_and_bind_ubo(sizeof(Shader_Material), 2, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	this->direction_lights_buffer = alloc_and_bind_ubo(sizeof(Lights_Block<Shader_Direction_Light>), 3, GL_UNIFORM_BUFFER, GL_STATIC_DRAW);
	this->spot_lights_buffer = alloc_and_bind_ubo(sizeof(Lights_Block<Shader_Spot_Light>), 4, GL_UNIFORM_BUFFER, GL_STATIC_DRAW);
	this->point_lights_buffer = alloc_and_bind_ubo(sizeof(Lights_Block<Shader_Point_Light>), 5, GL_UNIFORM_BUFFER, GL_STATIC_DRAW);
	this->max_height_buffer = alloc_and_bind_ubo(sizeof(GLfloat), 6, GL_UNIFORM_BUFFER, GL_STATIC_DRAW);
	this->max_density_buffer = alloc_and_bind_ubo(sizeof(GLfloat), 7, GL_UNIFORM_BUFFER, GL_STATIC_DRAW);

	return true;
}

GLuint gen_texture(GLsizei width, GLsizei height, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE, GLvoid* texture_data = NULL)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, texture_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

GLuint gen_texture(GLsizei width, GLsizei height, GLsizei depth, GLenum type = GL_FLOAT, GLenum format = GL_RED)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_3D, texture);
	GL_ERROR_CHECK(glTexImage3D(GL_TEXTURE_3D, 0, format, width, height, depth, 0, format, type, NULL));
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_3D, 0);
	return texture;
}

int Graphics_Table::__create_volume_texture(int width, int height, int depth)
{
	int volume_texture;
	for (int i = 0; i < MAX_VOLUME_COUNT; ++i)
	{
		if (this->volume_textures[i] == 0)
		{
			volume_texture = i;
			break;
		}
	}

	
	this->volume_textures[volume_texture] = gen_texture(width, height, depth);
	return volume_texture;
}

void Graphics_Table::__buffer_volume_data(int volume_texture, int width, int height, int depth, float* volume_data, GLenum type, GLenum format)
{
	glBindTexture(GL_TEXTURE_3D, this->volume_textures[volume_texture]);
	GL_ERROR_CHECK(glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, width, height, depth, format, type, volume_data));
	glBindTexture(GL_TEXTURE_3D, 0);
}

GLuint Graphics_Table::__create_texture(GLuint texture_width, GLuint texture_height, float* texture_data, GLenum format)
{
	return gen_texture(texture_width, texture_height, format, GL_FLOAT, texture_data);
}

void Graphics_Table::__buffer_texture_data(GLuint texture, GLuint width, GLuint height, float* data, GLenum format)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_FLOAT, data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics_Table::__use_texture(GLuint texture, GLuint texture_unit, GLenum texture_target)
{
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(texture_target, texture);
}

void Graphics_Table::__use_volume_texture(int volume_texture, GLuint texture_unit)
{
	this->__use_texture(this->volume_textures[volume_texture], texture_unit, GL_TEXTURE_3D);
}

void draw_as_polygons()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void draw_as_wireframes()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh_vertex), (void*)(2 * sizeof(Vector3)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_drawable.index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, number_of_indices * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	mesh_drawable.number_of_indices = number_of_indices;

	return mesh_drawable;
}

Drawable buffer_mesh(Mesh m)
{
	return buffer_mesh(m.vertices, m.number_of_vertices, m.face_indices, m.number_of_indices);
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

void Graphics_Table::__use_shader(int shader)
{
	glUseProgram(this->shaders[shader]);
}

GLuint compile_shader_src(char* shader_src, GLenum shader_type)
{
	GL_ERROR_CHECK(GLuint shader = glCreateShader(shader_type));
	GL_ERROR_CHECK(glShaderSource(shader, 1, &shader_src, NULL));
	GL_ERROR_CHECK(glCompileShader(shader));

	char info[2048] = {};
	GLint success;

	//NOTE: OpenGL/AMD/whoever is really shit, this caused a crash when a specific compiler error occurred on my machine
	//Error was referring to a uniform variable within a uniform block as "uniform_block.var_name" instead of just "var_name"
	GL_ERROR_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
	
	if (!success)
	{
		GL_ERROR_CHECK(glGetShaderInfoLog(shader, 2048, NULL, info));
		OutputDebugStringf("Error: Shader compilation failed: \n%s", info);
		return 0;
	}
	else
	{
		GL_ERROR_CHECK(glGetShaderInfoLog(shader, 2048, NULL, info));
		OutputDebugStringf("Shader %u compilation was successful\n%s", shader, info);
	}
	return shader;
}

GLuint create_shader_program(char* v_shader_src, char* f_shader_src)
{
	GLuint v_shader = compile_shader_src(v_shader_src, GL_VERTEX_SHADER);
	GLuint f_shader = compile_shader_src(f_shader_src, GL_FRAGMENT_SHADER);

	GL_ERROR_CHECK(GLuint shader = glCreateProgram());
	GL_ERROR_CHECK(glAttachShader(shader, v_shader));
	GL_ERROR_CHECK(glAttachShader(shader, f_shader));
	GL_ERROR_CHECK(glLinkProgram(shader));

	char info[2048] = {};
	GLint success;
	GL_ERROR_CHECK(glGetProgramiv(shader, GL_LINK_STATUS, &success));
	if (!success)
	{//If the shader program wasn't linked
		GL_ERROR_CHECK(glGetProgramInfoLog(shader, 2048, NULL, info));
		OutputDebugStringf("Failed to link shader program:\n%s", info);
		return 0;
	}
	else
	{
		GL_ERROR_CHECK(glGetProgramInfoLog(shader, 2048, NULL, info));
		OutputDebugStringf("Linked shader program %u successfully\n", shader, info);
	}
	GL_ERROR_CHECK(glDeleteShader(v_shader));
	GL_ERROR_CHECK(glDeleteShader(f_shader));
	return shader;
}

int Graphics_Table::__load_shader_program(const char* v_shader_path, const char* f_shader_path)
{
	char* v_shader_src = read_file(v_shader_path);
	char* f_shader_src = read_file(f_shader_path);

	int shader = -1;
	for (int i = 0; i < MAX_SHADER_COUNT; ++i)
	{
		if (this->shaders[i] == 0)
		{
			shader = i;
			break;
		}
	}
	this->shaders[shader] = create_shader_program(v_shader_src, f_shader_src);

	dealloc_mem(v_shader_src);
	dealloc_mem(f_shader_src);

	return shader;
}

int Graphics_Table::__alloc_framebuffer(int width, int height, int framebuffer)
{
	if(framebuffer == -1)
	{
		for (int i = 0; i < MAX_FRAMEBUFFER_COUNT; ++i)
		{
			if (this->framebuffers[i] == 0)
			{
				framebuffer = i;
				break;
			}
		}
	}

	glGenFramebuffers(1, this->framebuffers + framebuffer);
	glGenRenderbuffers(1, this->framebuffer_renderbuffers + framebuffer);
	OutputDebugStringf("Generated framebuffer %u\n", this->framebuffers[framebuffer]);
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffers[framebuffer]);

	this->framebuffer_textures[framebuffer] = gen_texture(width, height);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->framebuffer_textures[framebuffer], 0);
	
	glBindRenderbuffer(GL_RENDERBUFFER, this->framebuffer_renderbuffers[framebuffer]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->framebuffer_renderbuffers[framebuffer]);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		OutputDebugStringf("Framebuffer buggered\n");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return framebuffer;
}

void Graphics_Table::__resize_framebuffer(int width, int height, int framebuffer)
{
	glBindTexture(GL_TEXTURE_2D, this->framebuffer_textures[framebuffer]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, this->framebuffer_renderbuffers[framebuffer]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Graphics_Table::__resize_framebuffers(int width, int height)
{
	for (int i = 0; i < MAX_FRAMEBUFFER_COUNT; ++i)
	{
		GLuint framebuffer = this->framebuffers[i];
		if (framebuffer) this->__resize_framebuffer(width, height, i);
	}
}

void Graphics_Table::__use_framebuffer(int framebuffer_index)
{
	GLuint framebuffer = (framebuffer_index != -1) ? this->framebuffers[framebuffer_index] : 0;
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void Graphics_Table::__use_framebuffer_texture(int framebuffer, int texture_unit)
{
	use_texture((framebuffer != -1) ? this->framebuffer_textures[framebuffer] : 0, texture_unit);
}

Drawable buffer_sphere_mesh(float radius, int number_of_slices, int number_of_stacks)
{
	Mesh sphere_mesh = {};
	//First and last vertices in array are the vertices at the sphere's poles
	sphere_mesh.number_of_vertices = (number_of_stacks - 1)*number_of_slices + 2;
	int size_of_vertex_array = sphere_mesh.number_of_vertices * sizeof(Mesh_vertex);


	int number_of_triangles = 2 * (number_of_stacks - 1)*number_of_slices;
	sphere_mesh.number_of_indices = 3 * number_of_triangles;
	int size_of_index_array = sphere_mesh.number_of_indices * sizeof(GLuint);
	sphere_mesh.face_indices = (GLuint*)alloc_mem(size_of_index_array);

	//Populate vertex data array (contains positions and normals)
	sphere_mesh.vertices = (Mesh_vertex*)alloc_mem(size_of_vertex_array);

	define_sphere_mesh(&sphere_mesh, radius, number_of_slices, number_of_stacks);

	//Buffer data
	Drawable sphere_drawable = buffer_mesh(sphere_mesh);

	dealloc_mem(sphere_mesh.vertices);
	dealloc_mem(sphere_mesh.face_indices);

	return sphere_drawable;
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
	Mesh cylinder_mesh = {};

	//The cylinder is made up of 2 circles and slice_count number of rectangles around the side
	//Each vertex needs to be defined twice, once with a normal for one circle and another for the rectangles
	//The number of vertices in a circle is slice_count + 1 (1 for the center vertex), 
	//therefore the number of vertices in circles = 2*slice_count+2
	//The number of vertices used for the rectangles = 2*slice_count
	//Therefore, the total number of vertices = 4*slice_count + 2
	cylinder_mesh.number_of_vertices = 4*slice_count + 2;
	cylinder_mesh.vertices = (Mesh_vertex*)alloc_mem(cylinder_mesh.number_of_vertices*sizeof(Mesh_vertex));

	//Number of indices per triangle = 3
	//Number of triangles in the cylinder circles = 2*slice_count
	//Number of triangles in the side rectangles = 2*slice_count
	//Number of indices in the cylinder = 3*(2*slice_count + 2*slice_count) = 3*4*slice_count
	cylinder_mesh.number_of_indices = 3 * 4 * slice_count;
	cylinder_mesh.face_indices = (GLuint*)alloc_mem(cylinder_mesh.number_of_indices * sizeof(GLuint));

	define_cylinder_mesh(&cylinder_mesh, radius, height, slice_count);

	Drawable cylinder = buffer_mesh(cylinder_mesh);
	dealloc_mem(cylinder_mesh.face_indices);
	dealloc_mem(cylinder_mesh.vertices);
	return cylinder;
}