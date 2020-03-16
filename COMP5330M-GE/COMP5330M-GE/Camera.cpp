#include "Camera.h"

Vector3 world_up = { 0.0f, 1.0f, 0.0f };

Camera::Camera() : m_cameraPosition(0.0f, 0.0f, 10.0f), m_cameraTarget(0.0f, 0.0f, 0.0f)
{
	/*The camera forward vector would be direction which results when subtracting the target - positionOfCamera*/
	/*We also normalize it as we want only the direction of the vector*/
	m_cameraForwardVector = normalise(m_cameraTarget - m_cameraPosition);

	/*The camera right vector is achieved by finding the cross product of the Up vector with the camera's current forward vector(It's direction)*/
	m_cameraRightVector = normalise(cross(world_up, m_cameraForwardVector));

	/*The camera's up vector is similarly foudn using a cross product*/
	m_cameraUpVector = normalise(cross(m_cameraForwardVector, m_cameraRightVector));
}

/*Constructor for user-set paramaters. We only need it's position and target in world space, to compute the other components*/
Camera::Camera(const Vector3 & position, const Vector3 & target) : m_cameraPosition(position), m_cameraTarget(target)
{
	m_cameraForwardVector = normalise(m_cameraTarget - m_cameraPosition);

	m_cameraRightVector = normalise(cross(world_up, m_cameraForwardVector));

	m_cameraUpVector = normalise(cross(m_cameraForwardVector, m_cameraRightVector));
}

/*Upward movement, using Q*/
void Camera::moveUp()
{
	m_cameraPosition += (m_cameraSpeed * m_cameraUpVector);
}

/*Downward movement using E*/
void Camera::moveDown()
{
	m_cameraPosition -= (m_cameraSpeed * m_cameraUpVector);
}

void Camera::moveLeft()
{
	m_cameraPosition -= (m_cameraSpeed * m_cameraRightVector);
}

void Camera::moveRight()
{
	m_cameraPosition += (m_cameraSpeed * m_cameraRightVector);
}

void Camera::moveForward()
{
	/*Note, we subtract from the forward, as we would be moving along negative z towards the screen*/
	m_cameraPosition -= (m_cameraSpeed * m_cameraForwardVector);
}

void Camera::moveBackward()
{
	m_cameraPosition += (m_cameraSpeed * m_cameraForwardVector);
}

Vector3 Camera::getPosition() const
{
	return m_cameraPosition;
}

Vector3 Camera::getTarget() const
{
	return m_cameraTarget;
}

Vector3 Camera::getRightVector() const
{
	return m_cameraRightVector;
}

Vector3 Camera::getForwardVector() const
{
	return m_cameraForwardVector;
}

Vector3 Camera::getUpwardVector() const
{
	return m_cameraUpVector;
}
