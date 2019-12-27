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

	RigidBody();
	RigidBody(Mesh mesh, Vector3 initialVelocity, Vector3 initialAcceleration, float mass);
};

bool cylinderCollision(RigidBody&, Mesh mesh);
// Mesh_vertex groundCollision();

// Update position of centre of rigid body
void updateDisplacement(RigidBody&, Vector3* forces, int numOfForces, float timeStep);
