#pragma once
#include "Perlin_Noise.h"
#include "Graphics.h"
#include "Maths.h"
#include "l_system.h"
#include "turtle.h"

//Goal: Render a procedurally generated landscape
//Landscape:
//	- Terrain
//	- Trees

//Terrain:
//	- Load an array of regularly spaced vertices in a grid
//	- Use perlin noise to set their y values

struct Terrain_Data
{
	Perlin_Noise_Function perlin_noise;
	Drawable terrain_drawable;
	Mesh mesh;
	float max_height;
	float width;
	float length;
};

enum Tree_Type
{
	TREE_PINE = 0,
	TREE_ROWAN,
	TREE_TYPE_MAX
};
#define MAX_TREES 2048
struct Tree_Distribution
{
	int number_of_trees;
	Vector2 landscape_positions[MAX_TREES];
	Tree_Type tree_types[MAX_TREES]; //The type of tree: it is either rowan or pine
	int tree_ids[MAX_TREES]; //The index of each tree's model in the drawables arrays
};

#define MAX_TREE_DRAWABLES 4
struct Forest_Data
{
	Drawable rowan_branch_drawables[MAX_TREE_DRAWABLES];
	Drawable rowan_leaf_drawables[MAX_TREE_DRAWABLES];
	Drawable pine_branch_drawables[MAX_TREE_DRAWABLES];
	Drawable pine_leaf_drawables[MAX_TREE_DRAWABLES];
	Tree_Distribution tree_distribution;
};

struct Landscape_Data
{
	Forest_Data forest;
	Terrain_Data terrain;

	void draw();
};

Terrain_Data create_terrain(float width, float length, float terrain_cell_length);
Forest_Data create_forest(float width, float length, int number_of_trees);
Landscape_Data create_landscape(float width, float length, float terrain_cell_length, int number_of_trees = MAX_TREES);