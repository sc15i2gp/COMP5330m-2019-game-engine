#pragma once

#include "Maths.h"

struct Particle {
	Vector3 displacement;
	Vector3 velocity;
	int life;		// In terms of frames

	Particle();
	Particle(Vector3 initialDisplacement, Vector3 initialVelocity, int life);
};

struct Emitter {
	Vector3 position;
	float radius;
	Vector3 normalReleaseVector;
	float rateOfParticleRelease;
	float maxAngle;

	Emitter();
	Emitter(Vector3 position, float radius, Vector3 normalReleaseVector, float rate, float maxAngle);
};