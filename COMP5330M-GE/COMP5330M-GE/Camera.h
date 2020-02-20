#pragma once
#include "Maths.h"
#include "Platform.h"

extern Vector3 world_up;
struct Camera
{
	Vector3 position;
	Vector3 forward;
	Vector3 upward;
	Vector3 rightward;
	Vector3 initial_forward;

	//Two states of arcball code: previous and current, where previous is for the previous mouse drag
	Quaternion arcball_q_previous;
	Quaternion arcball_q_current;
	Matrix4x4 m_current;
	Vector2 drag_origin;
	float movement_sensitivity;

	//Converts given screen coordinates to a vector in the range ([-1, 1], [-1, 1])
	Vector2 screen_coords_to_circle_coords(Vector2 v);

	void update_rightward_and_upward_vectors();

	void set_position_and_target(Vector3 position, Vector3 target);

	void rotate_by_arcball(Vector2 drag_now);
	void rotation_start(Vector2 starting_screen_coords);
	void rotation_end();

	void move_forward();
	void move_backward();
	void move_left();
	void move_right();
	void move_up();
	void move_down();
};
