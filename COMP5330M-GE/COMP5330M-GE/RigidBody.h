#pragma once

#include "Maths.h"

class RigidBody
{
public:
	// Constructor
	RigidBody();
	RigidBody(Vector3 initialDisplacement, Vector3 initialVelocity, Vector3 initialAcceleration, float mass);

	// Check for collision
	bool checkForCollision();

	// Update position of centre of rigid body
	void updateDisplacement(Vector3* forces, float timeStep);

	// Current displacement of object
	Vector3 displacement;
	// Current velocity of object
	Vector3 velocity;
	// Current acceleration of object
	Vector3 acceleration;
	// Mass of the object
	float mass;
};