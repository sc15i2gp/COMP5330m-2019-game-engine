#pragma once
#pragma once
#include "Tools/Maths.h"
#include "Tools/Mesh.h"
#include "string_functions.h"

struct tree_mesh_group
{
	Mesh branch_mesh;
	Mesh leaf_mesh;
};

//String -> Mesh
struct turtle
{
	struct
	{
		Vector3 heading;
		Vector3 left;
		Vector3 up;
	} orientation;
	Vector3 position;
	float width;

	void move_forward(float); //F
	void turn_left(float); //+
	void turn_right(float); //-
	void roll_backward(float); //^
	void roll_forward(float); //&
	void roll_left(float); // /
	void roll_right(float); // '\' 
};

#include "turtle_stack.h"

void run_turtle(char* input, tree_mesh_group* tree, float default_distance = 1.0f, float default_branch_radius = 0.25f, float default_angle = 90.0f);
