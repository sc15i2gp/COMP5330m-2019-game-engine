#pragma once

#include "Maths.h"

struct Particle {
	Vector3 displacement;
	Vector3 velocity;
	int life;		// In terms of frames

	Particle();
	Particle(Vector3 initialDisplacement, Vector3 initialVelocity, int life);
};

struct ParticleBody {
	Particle particle;
	Vector3 acceleration;
	float mass;

	ParticleBody(Particle&, Vector3 acceleration, float mass);
};

struct Emitter {
	Vector3 position;
	float radius;
	Vector3 normalSpeedVector;
	float rateOfParticleRelease;
	float maxAngleAroundX;
	float maxAngleAroundY;
	float maxAngleAroundZ;
	// Has to be between 0.0 and 1.0
	float minSpeedRatio;
	int minLife;
	int maxLife;

	Emitter();
	Emitter(Vector3 position, float radius, Vector3 normalSpeedVector, float rate, float maxAngleAroundX, float maxAngleAroundY, float maxAngleAroundZ, float minSpeedRatio, int minLife, int maxLife);
};

Particle releaseOneParticle(Emitter&);

Particle simpleReleaseOneParticle(Emitter&);

ParticleBody makeParticleRigidBody(Particle&, Vector3 acceleration, float mass);

Vector3 updateParticlePosition(Particle&, float timeStep);

Vector3 updateParticlePositionUnderForces(ParticleBody&, Vector3* forces, int numOfForces, float timeStep);
