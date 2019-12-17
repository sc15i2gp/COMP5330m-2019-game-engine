#include "RigidBody.h"

RigidBody::RigidBody() {
	displacement = { 0.0,0.0,0.0 };
	velocity = { 0.0,0.0,0.0 };
}

RigidBody::RigidBody(Vector3 initialDisplacement, Vector3 initialVelocity) {
	displacement = initialDisplacement;
	velocity = initialVelocity;
}

void RigidBody::updateDisplacement(Vector3* forces, float timeStep) {
	for (int i = 0; i < 3; ++i) {
		float accumulate = 0.0;
		float acceleration = 0.0;
		for (Vector3* force = forces; &force; force++) {
			// FIX THIS
			//accumulate = force[i];
		}
	}
}