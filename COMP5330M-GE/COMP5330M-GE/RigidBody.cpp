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

bool RigidBody::checkForCollision() {
	return displacement[1] <= 0.0;
}

void RigidBody::updateDisplacement(Vector3* forces, float timeStep) {
	displacement = displacement + (timeStep * velocity) + (0.5 * (timeStep * timeStep) * acceleration);
	velocity = velocity + (0.5 * timeStep * acceleration);
	Vector3 totalForce = { 0.0,0.0,0.0 };
	for (Vector3* force = forces; &force; force++) {
		totalForce += *force;
	}
	acceleration = totalForce / mass;
	velocity = velocity + (0.5 * timeStep * acceleration);
	if (checkForCollision()) {
		float cor = 0.5;
		velocity.y = velocity.y * -cor;
	}
}