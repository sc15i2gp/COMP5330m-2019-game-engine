#pragma once

#include "Maths.h"

// Simple particle - Moves at a constant velocity
struct Particle {
	Vector3 displacement;
	Vector3 velocity;
	int life;		// In terms of frames

	Particle();
	Particle(Vector3 initialDisplacement, Vector3 initialVelocity, int life);
};

// A particle given rigid body features so it can be acted under forces
struct ParticleBody {
	Particle particle;
	Vector3 acceleration;
	float mass;

	ParticleBody(Particle&, Vector3 acceleration, float mass);
};

// A point or area that emits particles
struct Emitter {
	Vector3 position;
	float radius;
	Vector3 normalSpeedVector;
	float maxAngleAroundX;
	float maxAngleAroundY;
	float maxAngleAroundZ;
	// Has to be between 0.0 and 1.0
	float minSpeedRatio;
	// Life in terms of frames
	int minLife;
	int maxLife;

	Emitter();
	Emitter(Vector3 position, float radius, Vector3 normalSpeedVector, float maxAngleAroundX, float maxAngleAroundY, float maxAngleAroundZ, float minSpeedRatio, int minLife, int maxLife);
};

// Simple particle emission
Particle releaseOneParticle(Emitter&);
// Simple version of the particle release function that uses the emitter's predefined values
Particle simpleReleaseOneParticle(Emitter&);

// Rigid body particle emission
ParticleBody releaseOneRigidParticle(Emitter&, Vector3 acceleration, float mass);
ParticleBody simpleReleaseOneRigidParticle(Emitter&, Vector3 acceleration, float mass);

// Particle release functions
Particle* releaseManyParticlesAtOnce(Emitter&, int numOfParticles);
Particle* releaseManyParticlesInASequence(Emitter&, int numOfParticles, float rate);
Particle* releaseBurstsOfParticlesInASequence(Emitter&, int numOfBursts, int minNumOfParticlesPerGroup, int maxNumOfParticlesPerGroup, float rate);

// Rigid particle release functions
ParticleBody* releaseManyRigidParticlesAtOnce(Emitter&, int numOfParticles, Vector3 acceleration, float mass);
ParticleBody* releaseManyRigidParticlesInASequence(Emitter&, int numOfParticles, float rate, Vector3 acceleration, float mass);
ParticleBody* releaseBurstsOfRigidParticlesInASequence(Emitter&, int numOfBursts, int minNumOfParticlesPerGroup, int maxNumOfParticlesPerGroup, float rate, Vector3 acceleration, float mass);

// Particle movement functions
ParticleBody makeParticleRigidBody(Particle&, Vector3 acceleration, float mass);
Vector3 updateParticlePosition(Particle&, float timeStep);
Vector3 updateParticlePositionUnderForces(ParticleBody&, Vector3* forces, int numOfForces, float timeStep);
