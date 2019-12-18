#include "RigidBody.h"

RigidBody::RigidBody() {
	displacement = { 0.0,0.0,0.0 };
	velocity = { 0.0,0.0,0.0 };
	acceleration = { 0.0,0.0,0.0 };
	mass = 1.0;
}

RigidBody::RigidBody(Vector3 initialDisplacement, Vector3 initialVelocity, Vector3 initialAcceleration, float mass) {
	displacement = initialDisplacement;
	velocity = initialVelocity;
	acceleration = initialAcceleration;
	this->mass = mass;
}

bool checkForCollision(RigidBody& r) {
	return r.displacement[1] <= 0.0;
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
		r.velocity.y = r.velocity.y * -cor;
	}
}