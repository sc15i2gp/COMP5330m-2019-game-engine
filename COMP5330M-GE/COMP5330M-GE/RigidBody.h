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

	// Constructors
	RigidBody();
	RigidBody(Vector3 initialDisplacement, Vector3 initialVelocity, Vector3 initialAcceleration, float mass, float radius);
};

// Calculate the amount of displacement to translate in a frame
// Velocity verlet is being used here
void updateDisplacement(RigidBody&, Vector3* forces, int numOfForces, float timeStep);

// Collision detection - Cylinder
// Position (x,z) defines the position of the cylinder
// miny and maxy define the height of the cylinder
bool checkSphereCylinderCollision(RigidBody&, float x, float z, float miny, float maxy, float radius);

// Continuous version - ensures sphere does not pass through cylinder
// Might not be needed
float checkSphereCylinderCollisionInPath(RigidBody&, Vector3 pathTaken, float x, float z, float miny, float maxy, float radius);

// Collision detection - Ground mesh
// A triangle defined by three points
bool checkSphereTriangleCollision(RigidBody&, Vector3 v1, Vector3 v2, Vector3 v3);
