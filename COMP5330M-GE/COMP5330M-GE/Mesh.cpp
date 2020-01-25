#include "Mesh.h"

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
