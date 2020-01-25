#pragma once
#include "OpenGL.h"
#include "Platform.h"
#include "Maths.h"

struct Mesh_vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 texture_coords;
};

struct Mesh
{
	int number_of_vertices;
	int number_of_indices;
	Mesh_vertex* vertices;
	int* first_edges;
	GLuint* face_indices; //Each element in this list describes the vertex in vertices that an edge originates from
	int* other_edges; //Each element in this list corresponds to a single element in the face_indices list

	void compute_first_edge_list();
	void compute_other_edge_list();
	void compute_normal_vectors();
};

//Edge structures used to compute directed edge lists

struct Edge
{
	GLuint edge_from;
	GLuint edge_to;
	int index;
};

int hash(Edge, int);


struct Edge_Entry
{
	Edge edge;
	Edge_Entry* next;
};

bool operator==(Edge, Edge);

struct Edge_Map
{
	int edge_entries_in_use;
	Edge_Entry* edge_entry_buffer;

	int max_number_of_entries;
	Edge_Entry** entries;

	void alloc(int);
	void dealloc();

	Edge_Entry* get_entry(Edge);
	void push(Edge);
	bool has_entry(Edge);
};