#include "turtle.h"

void turtle::move_forward(float distance)
{
	this->position = this->position + distance * this->orientation.heading;
}

void turtle::turn_left(float angle)
{
	this->orientation.heading = rotate_point_about_axis(this->orientation.heading, this->orientation.up, angle);
	this->orientation.left = rotate_point_about_axis(this->orientation.left, this->orientation.up, angle);
}

void turtle::turn_right(float angle)
{
	turn_left(360.0f - angle);
}

void turtle::roll_backward(float angle)
{
	this->orientation.heading = rotate_point_about_axis(this->orientation.heading, this->orientation.left, angle);
	this->orientation.up = rotate_point_about_axis(this->orientation.up, this->orientation.left, angle);
}

void turtle::roll_forward(float angle)
{
	roll_backward(360.0f - angle);
}

void turtle::roll_left(float angle)
{
	roll_right(360.0f - angle);
}

void turtle::roll_right(float angle)
{
	this->orientation.up = rotate_point_about_axis(this->orientation.up, this->orientation.heading, angle);
	this->orientation.left = rotate_point_about_axis(this->orientation.left, this->orientation.heading, angle);
}


/********************************************************/

/*		Push vertex data to mesh		*/

/********************************************************/

void push_sphere_to_mesh(turtle* t, Mesh* dst_mesh, Mesh* sphere_mesh, float radius)
{
	Vector3 origin = t->position + radius * t->orientation.heading;
	define_sphere_mesh(sphere_mesh, radius, 8, 8, origin);
	attach_mesh_to_mesh(dst_mesh, sphere_mesh);
	sphere_mesh->number_of_indices = 0;
	sphere_mesh->number_of_vertices = 0;
}

//In this function, the origin of the cylinder is the base of the cylinder
void push_cylinder_to_mesh(turtle* t, Mesh* dst_mesh, Mesh* cylinder_mesh, float radius, float height)
{
	Vector3 origin = t->position + (height / 2.0f)*t->orientation.heading;
	define_cylinder_mesh(cylinder_mesh, radius, height, 8, origin, t->orientation.heading, -t->orientation.left);
	attach_mesh_to_mesh(dst_mesh, cylinder_mesh);
	cylinder_mesh->number_of_indices = 0;
	cylinder_mesh->number_of_vertices = 0;
}

//String -> tree model
void run_turtle(char* input, tree_mesh_group* tree, float default_distance, float default_radius, float default_angle)
{
	turtle t = {};

	t.orientation.heading = Vector3{ 0.0f, 1.0f, 0.0f };
	t.orientation.left = Vector3{ -1.0, 0.0, 0.0 };
	t.orientation.up = Vector3{ 0.0, 0.0, 1.0 };
	t.width = default_radius;

	STACK(turtle) t_stack;

	int max_number_of_vertices = 1024;
	int max_number_of_indices = 8192;
	Mesh temp_mesh_buffer = {};
	temp_mesh_buffer.vertices = (Mesh_vertex*)alloc_mem(max_number_of_vertices * sizeof(Mesh_vertex));
	temp_mesh_buffer.face_indices = (GLuint*)alloc_mem(max_number_of_indices * sizeof(GLuint));

	int input_length = number_of_modules(input);
	char* module_str = input;

	int polygon_depth = 0;
	bool polygon_mode = false;

	for (int i = 0; i < input_length; i++)
	{
		int module_param_count = number_of_parameters(module_str);
		switch (*module_str)
		{
		case '[':
			t_stack.push_state(t);
			break;
		case ']':
			t = t_stack.pop_state();
			break;

		case '!':
			t.width = read_real_parameter_value(module_str);
			break;
		case '@':
		{
			float radius = default_radius;
			if (module_param_count > 0) radius = read_real_parameter_value(module_str);
			push_sphere_to_mesh(&t, &tree->leaf_mesh, &temp_mesh_buffer, radius);
			break;
		}
		case 'f':
		{
			float dist = default_distance;
			if (module_param_count > 0) dist = read_real_parameter_value(module_str);
			t.move_forward(dist);
			break;
		}
		case 'F':   
		{
			float length = (module_param_count > 0) ? read_real_parameter_value(module_str) : default_distance;
			float radius = (module_param_count > 1) ? read_real_parameter_value(module_str, 1) : t.width;
			push_cylinder_to_mesh(&t, &tree->branch_mesh, &temp_mesh_buffer, radius, length);
			t.move_forward(length);
			break;
		}
		case '+':
		{
			float angle = default_angle;
			if (module_param_count > 0) angle = read_real_parameter_value(module_str);
			t.turn_left(angle);
			break;
		}
		case '-':
		{
			float angle = default_angle;
			if (module_param_count > 0) angle = read_real_parameter_value(module_str);
			t.turn_right(angle);
			break;
		}
		case '^':
		{
			float angle = default_angle;
			if (module_param_count > 0) angle = read_real_parameter_value(module_str);
			t.roll_backward(angle);
			break;
		}
		case '&':
		{
			float angle = default_angle;
			if (module_param_count > 0) angle = read_real_parameter_value(module_str);
			t.roll_forward(angle);
			break;
		}
		case '\\':
		{
			float angle = default_angle;
			if (module_param_count > 0) angle = read_real_parameter_value(module_str);
			t.roll_left(angle);
			break;
		}
		case '/':
		{
			float angle = default_angle;
			if (module_param_count > 0) angle = read_real_parameter_value(module_str);
			t.roll_right(angle);
			break;
		}
		}
		module_str = find_next_module(module_str);
	}
}