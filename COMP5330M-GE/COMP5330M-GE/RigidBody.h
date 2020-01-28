#pragma once

#include "Maths.h"

struct RigidBody
{
	// Displacement of an object is determined by its centre of mass
	Vector3 displacement;
	Vector3 velocity;
	Vector3 acceleration;
	float mass;

	// The rigid body is a sphere, so properties need to be defined
	float radius;

	RigidBody();
	RigidBody(Vector3 initialDisplacement, Vector3 initialVelocity, Vector3 initialAcceleration, float mass, float radius);
};

// Calculate the amount of displacement to translate in a frame
// Velocity verlet is being used here
Vector3 updateDisplacement(RigidBody&, Vector3* forces, int numOfForces, float timeStep);

// Collision detection
bool checkSphereCylinderCollision(RigidBody&, float x, float z, float miny, float maxy, float radius);
