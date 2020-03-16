#pragma once
#include "Maths.h"
#include "Platform.h"

/*The Up vector, facing positive Y*/
extern Vector3 world_up;

/*
	A class implementing a camera object, 
	with which we can view the scene.

*/
class Camera
{
private:

	Vector3 m_cameraPosition; // The position of the camera, located in the scene
	Vector3 m_cameraForwardVector; // A direction vector which will be the way the camera is currently looking at
	Vector3 m_cameraTarget; // A point which the camera will be looking at
	Vector3 m_cameraRightVector; // The positive X axis of the camera
	Vector3 m_cameraUpVector; // The camera's coordinate frame's up vector

	const float m_cameraSpeed = 0.05f; // Make the speed of the camera slower/faster

	float yaw;
	float pitch;

public:

	Camera();
	Camera(const Vector3& position, const Vector3& target);

	/*Movement*/
	void moveUp();
	void moveDown();

	void moveLeft();
	void moveRight();

	void moveForward();
	void moveBackward();

	/*Getters*/
	Vector3 getPosition() const;
	Vector3 getTarget() const;
	Vector3 getRightVector() const;
	Vector3 getForwardVector() const;
	Vector3 getUpwardVector() const;

	//Two states of arcball code: previous and current, where previous is for the previous mouse drag
	Quaternion arcball_q_previous;
	Quaternion arcball_q_current;
	Matrix4x4 m_current;
	Vector2 drag_origin;
	float movement_sensitivity;
	Vector3 initial_forward;
};