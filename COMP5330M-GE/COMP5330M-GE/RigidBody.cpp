#include "RigidBody.h"

CollidableShape::CollidableShape()
{
}


RigidBody::RigidBody() {
	displacement = { 0.0,0.0,0.0 };
	velocity = { 0.0,0.0,0.0 };
	acceleration = { 0.0,0.0,0.0 };
	mass = 1.0;
	shapeType = 1;
	shape.sphere.radius = 1.0;
}

RigidBody::RigidBody(Vector3 initialDisplacement, Vector3 initialVelocity, Vector3 initialAcceleration, float mass, float radius) {
	displacement = initialDisplacement;
	velocity = initialVelocity;
	acceleration = initialAcceleration;
	this->mass = mass;
	shapeType = 1;
	shape.sphere.radius = radius;
}

RigidBody::RigidBody(Vector3 initialDisplacement, Vector3 initialVelocity, Vector3 initialAcceleration, float mass, float x, float y, Vector3 normal) {
	displacement = initialDisplacement;
	velocity = initialVelocity;
	acceleration = initialAcceleration;
	this->mass = mass;
	shapeType = 2;
	shape.square.x = x;
	shape.square.y = y;
	shape.square.normal = normal;
}

bool checkForCollision(RigidBody& r) {
	if (r.shapeType == 1) {
		return true;
	}
}

void updateDisplacement(RigidBody& r, Vector3* forces, int numOfForces, float timeStep) {
	r.displacement = r.displacement + (timeStep * r.velocity) + (0.5 * (timeStep * timeStep) * r.acceleration);
	r.velocity = r.velocity + (0.5 * timeStep * r.acceleration);
	Vector3 totalForce = { 0.0,0.0,0.0 };
	for (int i = 0; i < numOfForces; ++i) {
		totalForce += forces[i];
	}
	r.acceleration = totalForce / r.mass;
	r.velocity = r.velocity + (0.5 * timeStep * r.acceleration);
	if (checkForCollision(r)) {
		float cor = 0.5;
		Vector3 v = { 0.0,1.0,0.0 };
		normalise(v);
		r.velocity.x = r.velocity.x * v.x * cor;
		r.velocity.y = r.velocity.y * v.y * cor;
		r.velocity.z = r.velocity.z * v.z * cor;
	}
}