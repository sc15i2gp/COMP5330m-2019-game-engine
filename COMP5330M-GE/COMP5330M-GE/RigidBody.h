#pragma once

#include "Maths.h"

struct Sphere
{
	float radius;
};

struct Square 
{
	float x;
	float y;
	Vector3 normal;
};

struct CollidableShape
{
	union
	{
		Sphere sphere;
		Square square;
	};
	CollidableShape();
};

struct RigidBody
{
	Vector3 displacement;
	Vector3 velocity;
	Vector3 acceleration;
	float mass;
	int shapeType;
	CollidableShape shape;

	RigidBody();
	RigidBody(Vector3 initialDisplacement, Vector3 initialVelocity, Vector3 initialAcceleration, float mass, float radius);
	RigidBody(Vector3 initialDisplacement, Vector3 initialVelocity, Vector3 initialAcceleration, float mass, float x, float y, Vector3 normal);
};

// Check for collision
bool checkForCollision(RigidBody&);

// Update position of centre of rigid body
void updateDisplacement(RigidBody&, Vector3* forces, int numOfForces, float timeStep);
