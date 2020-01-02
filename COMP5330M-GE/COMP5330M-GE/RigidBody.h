#pragma once

#include "Maths.h"
#include "Graphics.h"

struct RigidBody
{
	Mesh mesh;
	// Displacement of an object is determined by its centre of mass
	Vector3 displacement;
	Vector3 velocity;
	Vector3 acceleration;
	float mass;

	// Min and max dimensions of the body - for collisions
	float minx;
	float maxx;
	float miny;
	float maxy;
	float minz;
	float maxz;

	RigidBody();
	RigidBody(Mesh mesh, Vector3 initialVelocity, Vector3 initialAcceleration, float mass);
};

bool cylinderCollision(RigidBody&, Mesh mesh);

// Update position of centre of rigid body
void updateDisplacement(RigidBody&, Vector3* forces, int numOfForces, float timeStep);
void updateVectorPositions(RigidBody&, Vector3 difference);
