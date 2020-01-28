#include "RigidBody.h"

RigidBody::RigidBody() {
	displacement = { 0.0,0.0,0.0 };
	velocity = { 0.0,0.0,0.0 };
	acceleration = { 0.0,0.0,0.0 };
	mass = 1.0;
	radius = 1.0;
}

RigidBody::RigidBody(Vector3 initialDisplacement, Vector3 initialVelocity, Vector3 initialAcceleration, float mass, float radius) {
	displacement = initialDisplacement;
	velocity = initialVelocity;
	acceleration = initialAcceleration;
	this->mass = mass;
	this->radius = radius;
}

Vector3 updateDisplacement(RigidBody& r, Vector3* forces, int numOfForces, float timeStep) {
	Vector3 initialDis = r.displacement;
	r.displacement = r.displacement + (timeStep * r.velocity) + ((0.5 * timeStep * timeStep) * r.acceleration);
	Vector3 finalDis = r.displacement;
	Vector3 disDiff = finalDis - initialDis;
	r.velocity = r.velocity + (0.5 * timeStep * r.acceleration);
	Vector3 totalForce = { 0.0,0.0,0.0 };
	for (int i = 0; i < numOfForces; ++i) {
		totalForce += forces[i];
	}
	r.acceleration = totalForce / r.mass;
	r.velocity = r.velocity + (0.5 * timeStep * r.acceleration);
	return disDiff;
}

bool checkSphereCylinderCollision(RigidBody& r, float x, float z, float miny, float maxy, float radius) {
	if (miny <= r.displacement.y <= maxy) {
		Vector3 m = { x, r.displacement.y, z };
		Vector3 distanceBetweenCenters = m - r.displacement;
		return length(distanceBetweenCenters) <= r.radius + radius;
	}
}
