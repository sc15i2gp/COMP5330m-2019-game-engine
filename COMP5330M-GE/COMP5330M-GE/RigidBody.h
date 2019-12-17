#pragma once

#include "Maths.h"

class RigidBody
{
public:
	// Constructor
	RigidBody();
	RigidBody(Vector3 initialDisplacement, Vector3 initialVelocity);

	// Update position of centre of rigid body
	void updateDisplacement(Vector3* forces, float timeStep);

	// Current displacement of object
	Vector3 displacement;
	// Current velocity of object
	Vector3 velocity;
};