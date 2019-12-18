#pragma once

#include "Maths.h"

struct HitBoxes
{
	struct Sphere
	{
		float radius;
	};

	struct Box
	{
		float width;
		float height;
		float depth;
	};
};

struct RigidBody
{
	Vector3 displacement;
	Vector3 velocity;
	Vector3 acceleration;
	float mass;
	HitBoxes* hitBoxes;

	RigidBody();
	RigidBody(Vector3 initialDisplacement, Vector3 initialVelocity, Vector3 initialAcceleration, float mass);
};

// Check for collision
bool checkForCollision(RigidBody&);

// Update position of centre of rigid body
void updateDisplacement(RigidBody&, Vector3* forces, int numOfForces, float timeStep);
