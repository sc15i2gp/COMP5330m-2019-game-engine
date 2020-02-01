#include "Mesh.h"

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

void define_sphere_mesh(Mesh* sphere_mesh, float radius, int slice_count, int stack_count, Vector3 origin)
{
	sphere_mesh->vertices[0] = sample_sphere_mesh_vertex(radius, 0, 0);
	sphere_mesh->vertices[0].position += origin;
	int sphere_vertex_offset = 1;
	for (float i = 0; i < slice_count; ++i)
	{//For each slice of the sphere
		for (float j = 1; j < stack_count; ++j)
		{//For each stack in the slice
			Mesh_vertex sphere_vertex = sample_sphere_mesh_vertex(radius, i / (float)slice_count, j / (float)stack_count);
			sphere_vertex.position += origin;
			sphere_mesh->vertices[sphere_vertex_offset++] = sphere_vertex;
		}
	}
	sphere_mesh->vertices[sphere_vertex_offset] = sample_sphere_mesh_vertex(radius, 1.0f, 1.0f);
	sphere_mesh->vertices[sphere_vertex_offset++].position += origin;
	//sphere_mesh->vertices[sphere_vertex_offset++] = sample_sphere_mesh_vertex(radius, 1.0f, 1.0f);
	sphere_mesh->number_of_vertices = sphere_vertex_offset;
	//Populate face index array
	//First stack layer
	int sphere_index_offset = 0;
	for (int i = 0; i < slice_count; ++i)
	{//For each triangle in the first stack layer
		sphere_mesh->face_indices[sphere_index_offset] = 0; //First vertex in vertex array, top pole
		sphere_mesh->face_indices[sphere_index_offset + 1] = 1 + i * (stack_count - 1);
		sphere_mesh->face_indices[sphere_index_offset + 2] = 1 + ((i + 1) % slice_count) * (stack_count - 1);
		sphere_index_offset += 3;
	}

	//Mid stack layers
	for (int i = 0; i < slice_count; ++i)
	{//For each slice
		for (int j = 1; j < stack_count - 1; ++j)
		{//For each rectangle in the mid layers of the slice
			GLuint i_0 = i * (stack_count - 1) + j;
			GLuint i_1 = i * (stack_count - 1) + j + 1;
			GLuint i_2 = ((i + 1) % slice_count)*(stack_count - 1) + j + 1;
			GLuint i_3 = ((i + 1) % slice_count)*(stack_count - 1) + j;

			GLuint rect_indices[] = { i_0, i_1, i_2, i_2, i_3, i_0 };
			for (int k = 0; k < 6; ++k)
			{
				sphere_mesh->face_indices[sphere_index_offset++] = rect_indices[k];
			}
		}
	}

	//Final stack layer
	for (int i = 0; i < slice_count; ++i)
	{
		sphere_mesh->face_indices[sphere_index_offset] = 1 + i * (stack_count - 1) + stack_count - 2;
		sphere_mesh->face_indices[sphere_index_offset + 1] = sphere_mesh->number_of_vertices - 1;
		sphere_mesh->face_indices[sphere_index_offset + 2] = 1 + ((i + 1) % slice_count) * (stack_count - 1) + stack_count - 2;
		sphere_index_offset += 3;
	}
	sphere_mesh->number_of_indices = sphere_index_offset;
}

//orientation vector is the vector around which the cylinder's points are generated
void define_cylinder_mesh(Mesh* cylinder_mesh, float radius, float height, int slice_count, Vector3 origin, Vector3 main_axis, Vector3 right_axis)
{
	//Used to center cylinder around origin
	float h = height / 2.0f;
	float slice_angle = 360.0f / slice_count;
	int vertex_offset = 0;

	right_axis = normalise(right_axis);
	main_axis = normalise(main_axis);

	//The first slice_count + 1 vertices are for the top circle
	//The next slice_count + 1 vertices are for the bottom circle
	//The remaining 2*slice_count vertices are for the side rectangles

	//Top circle
	cylinder_mesh->vertices[vertex_offset++] = { origin + h*main_axis, main_axis };
	for (int i = 0; i < slice_count; ++i)
	{
		Vector3 position = origin + h * main_axis + rotate_point_about_axis(radius*right_axis, main_axis, (float)(i)*slice_angle);
		Vector3 normal = main_axis;
		cylinder_mesh->vertices[vertex_offset++] = { position, normal };
	}

	//Bottom circle
	cylinder_mesh->vertices[vertex_offset++] = { origin - h*main_axis, -main_axis };
	for (int i = 0; i < slice_count; ++i)
	{
		Vector3 position = origin - h * main_axis + rotate_point_about_axis(radius*right_axis, main_axis, (float)(i)*slice_angle);
		Vector3 normal = -main_axis;
		cylinder_mesh->vertices[vertex_offset++] = { position, normal };
	}

	//Side rectangles, top circle
	for (int i = 0; i < slice_count; ++i)
	{
		Vector3 position = origin + h * main_axis + rotate_point_about_axis(radius*right_axis, main_axis, (float)(i)*slice_angle);
		Vector3 normal = rotate_point_about_axis(right_axis, main_axis, (float)(i)*slice_angle);
		cylinder_mesh->vertices[vertex_offset++] = { position, normal };
	}

	//Side rectangles, bottom circle
	for (int i = 0; i < slice_count; ++i)
	{
		Vector3 position = origin - h * main_axis + rotate_point_about_axis(radius * right_axis, main_axis, (float)(i)*slice_angle);
		Vector3 normal = rotate_point_about_axis(right_axis, main_axis, (float)(i)*slice_angle);
		cylinder_mesh->vertices[vertex_offset++] = { position, normal };
	}

	int index_offset = 0;

	//The first 3*2*slice_count indices are for the top and bottom circles
	//The final 3*2*slice_count indices are for the side rectangles

	//Top circle
	int top_circle_offset = 0;
	for (int i = 0; i < slice_count; ++i)
	{
		cylinder_mesh->face_indices[index_offset++] = top_circle_offset;
		cylinder_mesh->face_indices[index_offset++] = top_circle_offset + i + 1;
		cylinder_mesh->face_indices[index_offset++] = top_circle_offset + 1 + ((i + 1) % (slice_count));
	}

	//Bottom circle
	int bottom_circle_offset = slice_count + 1;
	for (int i = 0; i < slice_count; ++i)
	{
		cylinder_mesh->face_indices[index_offset++] = bottom_circle_offset;
		cylinder_mesh->face_indices[index_offset++] = bottom_circle_offset + slice_count - i;
		cylinder_mesh->face_indices[index_offset++] = bottom_circle_offset + slice_count - ((i + 1) % (slice_count));
	}

	//Side rectangles
	int rectangles_t_offset = 2 * slice_count + 2;
	int rectangles_b_offset = 3 * slice_count + 2;
	for (int i = 0; i < slice_count; ++i)
	{
		GLuint top_left = rectangles_t_offset + i;
		GLuint bottom_left = rectangles_b_offset + i;
		GLuint bottom_right = rectangles_b_offset + ((i + 1) % slice_count);
		GLuint top_right = rectangles_t_offset + ((i + 1) % slice_count);
		cylinder_mesh->face_indices[index_offset++] = top_left;
		cylinder_mesh->face_indices[index_offset++] = bottom_left;
		cylinder_mesh->face_indices[index_offset++] = bottom_right;
		cylinder_mesh->face_indices[index_offset++] = bottom_right;
		cylinder_mesh->face_indices[index_offset++] = top_right;
		cylinder_mesh->face_indices[index_offset++] = top_left;
	}
	cylinder_mesh->number_of_vertices = vertex_offset;
	cylinder_mesh->number_of_indices = index_offset;
}

//Add the data in src_mesh to dst_mesh
void attach_mesh_to_mesh(Mesh* dst_mesh, Mesh* src_mesh)
{
	GLuint index_offset = dst_mesh->number_of_indices;
	GLuint vertex_offset = dst_mesh->number_of_vertices;
	for (int i = 0; i < src_mesh->number_of_vertices; ++i)
	{
		dst_mesh->vertices[vertex_offset + i] = src_mesh->vertices[i];
	}
	for (int i = 0; i < src_mesh->number_of_indices; ++i)
	{
		dst_mesh->face_indices[index_offset + i] = src_mesh->face_indices[i] + vertex_offset;
	}
	dst_mesh->number_of_indices += src_mesh->number_of_indices;
	dst_mesh->number_of_vertices += src_mesh->number_of_vertices;
}

int hash(Edge e, int hash_max)
{
	return (e.edge_from * 128 + e.edge_to) % hash_max;
}

bool operator==(Edge e, Edge f)
{
	return e.edge_from == f.edge_from && e.edge_to == f.edge_to;
}

void Edge_Map::alloc(int max_entry_number)
{
	this->edge_entry_buffer = (Edge_Entry*)alloc_mem(max_entry_number * sizeof(Edge_Entry));
	this->entries = (Edge_Entry**)alloc_mem(max_entry_number * sizeof(Edge_Entry*));
	for (int i = 0; i < max_entry_number; ++i)
	{
		this->edge_entry_buffer[i] = Edge_Entry{};
		this->entries[i] = NULL;
	}
	this->max_number_of_entries = max_entry_number;
	this->edge_entries_in_use = 0;
}

void Edge_Map::dealloc()
{
	dealloc_mem(this->edge_entry_buffer);
	dealloc_mem(this->entries);
}

Edge_Entry* Edge_Map::get_entry(Edge e)
{
	int edge_entry_index = hash(e, this->max_number_of_entries);

	Edge_Entry* desired_entry = NULL;
	for (Edge_Entry* map_entry = this->entries[edge_entry_index]; map_entry; map_entry = map_entry->next)
	{
		if (map_entry->edge == e)
		{
			desired_entry = map_entry;
			break;
		}
	}
	return desired_entry;
}

void Edge_Map::push(Edge e)
{
	//Find position in entries to store edge
	int edge_entry_index = hash(e, this->max_number_of_entries);

	//Find end of linked list in index
	Edge_Entry** entry_loc = entries + edge_entry_index;
	while (*entry_loc) entry_loc = &((*entry_loc)->next);

	Edge_Entry* free_entry = this->edge_entry_buffer + (this->edge_entries_in_use++);
	free_entry->edge = e;

	*entry_loc = free_entry;
}

bool Edge_Map::has_entry(Edge e)
{
	return get_entry(e) != NULL;
}

int next_edge_index(int index)
{
	return (index % 3 < 2) ? index + 1 : index - 2;
}

int prev_edge_index(int index)
{
	return (index % 3 > 0) ? index - 1 : index + 2;
}

void Mesh::compute_normal_vectors()
{
	for (int i = 0; i < this->number_of_vertices; ++i)
	{//For each vertex in the mesh
		//Compute its normal
		Vector3 normal;
		int faces_processed = 0; 
		int face_index_index = this->first_edges[i]; //Position in face_indices of vertex i's first occurrence
		int first_face_index_index = face_index_index;
		do
		{			
			//Loop over face
			Vector3 triangle_vertices[3] = {};
			for (int j = 0; j < 3; ++j)
			{ 
				triangle_vertices[j] = this->vertices[this->face_indices[face_index_index]].position;
				face_index_index = next_edge_index(face_index_index);
			}
			//Compute face normal
			Vector3 v = triangle_vertices[1] - triangle_vertices[0];
			Vector3 w = triangle_vertices[2] - triangle_vertices[0];
			normal += normalise(cross(v, w));
			++faces_processed;

			//Move to previous edge's other edge
			face_index_index = this->other_edges[prev_edge_index(face_index_index)];
		} while (face_index_index != first_face_index_index && face_index_index != -1);
		normal /= (float)faces_processed;
		this->vertices[i].normal = normal;
	}
}

void Mesh::compute_first_edge_list()
{
	for (int i = 0; i < this->number_of_vertices; ++i)
	{
		this->first_edges[i] = -1;
	}

	for (int i = 0; i < this->number_of_indices; ++i)
	{
		int index = (int)this->face_indices[i];
		if (this->first_edges[index] == -1) this->first_edges[index] = i;
	}
}

void Mesh::compute_other_edge_list()
{
	Edge_Map other_edges_map;
	other_edges_map.alloc(this->number_of_indices);

	//Clear other_edges to -1
	for (int i = 0; i < this->number_of_indices; ++i)
	{
		this->other_edges[i] = -1;
	}

	//Find other edges list using the edge map
	for (int i = 0; i < this->number_of_indices; ++i)
	{
		GLuint edge_from = this->face_indices[i];
		GLuint edge_to = (i % 3 < 2) ? this->face_indices[i + 1] : this->face_indices[i - 2];

		Edge current_edge = { edge_from, edge_to, i };
		Edge other_edge = { edge_to, edge_from };

		if (other_edges_map.has_entry(current_edge))
		{
			OutputDebugStringf("Warning: Given mesh has the same edge multiple times, finding neighbouring edges is now unreliable\n");
		}
		else if (other_edges_map.has_entry(other_edge))
		{
			//Pair current_edge and other_edge
			int current_edge_index = i;
			int other_edge_index = other_edges_map.get_entry(other_edge)->edge.index;

			this->other_edges[current_edge_index] = other_edge_index;
			this->other_edges[other_edge_index] = current_edge_index;
		}

		other_edges_map.push(current_edge);
	}

	other_edges_map.dealloc();
}
