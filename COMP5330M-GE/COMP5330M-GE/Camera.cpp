#include "Camera.h"

Vector3 world_up = { 0.0f, 1.0f, 0.0f };

Vector2 Camera::screen_coords_to_circle_coords(Vector2 v)
{
	Vector2 screen_dimensions = { get_window_width(window), get_window_height(window) };
	Vector2 w = Vector2(1.0f, 1.0f) - (v / (0.5f * screen_dimensions));
	if (dot(w, w) > 1.0f) w = normalise(w);
	return w;
}

void Camera::update_rightward_and_upward_vectors()
{
	this->rightward = normalise(cross(world_up, -this->forward));
	this->upward = normalise(cross(this->rightward, this->forward));
}

void Camera::set_position_and_target(Vector3 position, Vector3 target)
{
	this->arcball_q_current = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
	this->arcball_q_previous = arcball_q_current;
	this->position = position;
	this->forward = normalise(target - position);
	update_rightward_and_upward_vectors();
}

void Camera::rotate_by_arcball(Vector2 drag_now)
{
	drag_now = screen_coords_to_circle_coords(drag_now);
	Quaternion q_0 = compute_great_circle_point(this->drag_origin);
	Quaternion q_1 = compute_great_circle_point(drag_now);

	if (q_0 != q_1)
	{
		Quaternion drag = compute_rotation_between_quaternions(q_0, q_1);
		this->arcball_q_current = drag * this->arcball_q_previous;
		this->m_current = quaternion_to_matrix(conjugate(this->arcball_q_current));

		Vector4 forward_4 = Vector4(this->initial_forward, 1.0f);
		forward_4 = this->m_current * forward_4;
		this->forward = forward_4.xyz;
		update_rightward_and_upward_vectors();
	}
}

void Camera::rotation_start(Vector2 starting_screen_coords)
{
	this->initial_forward = forward;
	this->drag_origin = screen_coords_to_circle_coords(starting_screen_coords);
}

void Camera::rotation_end()
{
	this->arcball_q_previous = this->arcball_q_current;
}

void Camera::move_forward()
{
	this->position += movement_sensitivity * forward;
}

void Camera::move_backward()
{
	this->position -= movement_sensitivity * forward;
}

void Camera::move_left()
{
	this->position -= movement_sensitivity * rightward;
}

void Camera::move_right()
{
	this->position += movement_sensitivity * rightward;
}

void Camera::move_up()
{
	this->position += movement_sensitivity * world_up;
}

void Camera::move_down()
{
	this->position -= movement_sensitivity * world_up;
}