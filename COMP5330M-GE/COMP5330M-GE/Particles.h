#pragma once

#include "Maths.h"

// Simple particle - Moves at a constant velocity
struct Particle {
	Vector3 displacement;
	Vector3 velocity;
	int life;		// In terms of frames
	float size;		// The quads are drawn (x-s,y-s,z) (x-s,y+s,z) (x+s,y+s,z) (x+s,y-s,z)

	Particle();
	Particle(Vector3 initialDisplacement, Vector3 initialVelocity, int life, float size);
};

// A particle given rigid body features so it can be acted under forces
struct ParticleBody {
	Particle* particle;
	Vector3 acceleration;
	float mass;

	ParticleBody();
	ParticleBody(Particle* particle, Vector3 acceleration, float mass);
};

struct ParticlePoolNode {
	Particle particle;
	bool nodeActive;
};

struct ParticlePool {
	ParticlePoolNode* nodes;
	int numOfParticles;
};

struct RigidParticlePoolNode {
	ParticleBody particleBody;
	bool nodeActive;
};

struct RigidParticlePool {
	RigidParticlePoolNode* nodes;
	int numOfParticles;
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
	// Particle size
	float minSize;
	float maxSize;

	Emitter();
	Emitter(Vector3 position, float radius, Vector3 normalSpeedVector, float maxAngleAroundX, float maxAngleAroundY, float maxAngleAroundZ, float minSpeedRatio, int minLife, int maxLife, float minSize, float maxSize);
};

// Simple particle emission
Particle releaseOneParticle(Emitter&);
// Simple version of the particle release function that uses the emitter's predefined values
Particle simpleReleaseOneParticle(Emitter&);

// Rigid body particle emission
ParticleBody releaseOneRigidParticle(Emitter&, Vector3 acceleration, float mass);
ParticleBody simpleReleaseOneRigidParticle(Emitter&, Vector3 acceleration, float mass);

// Particle pool initialisation
void initialisePool(ParticlePool&, int numOfParticles);
void initialiseRigidPool(RigidParticlePool&, int numOfParticles);

// Particle release functions
// Rate is particles per second
void releaseManyParticlesAtOnce(Emitter&, ParticlePool pool, int numOfParticles);
void releaseManyParticlesInASequence(Emitter&, ParticlePool pool, int numOfParticles, float rate);
void releaseManyParticlesInASequenceForever(Emitter&, ParticlePool pool, float rate);
void releaseBurstsOfParticlesInASequence(Emitter&, ParticlePool pool, int numOfBursts, int numOfParticlesPerGroup, float rate);
void releaseBurstsOfParticlesInASequenceForever(Emitter&, ParticlePool pool, int numOfParticlesPerGroup, float rate);

// Rigid particle release functions
void releaseManyRigidParticlesAtOnce(Emitter&, RigidParticlePool pool, int numOfParticles, Vector3 acceleration, float mass);
void releaseManyRigidParticlesInASequence(Emitter&, RigidParticlePool pool, int numOfParticles, float rate, Vector3 acceleration, float mass);
void releaseManyRigidParticlesInASequenceForever(Emitter&, RigidParticlePool pool, float rate, Vector3 acceleration, float mass);
void releaseBurstsOfRigidParticlesInASequence(Emitter&, RigidParticlePool pool, int numOfBursts, int numOfParticlesPerGroup, float rate, Vector3 acceleration, float mass);
void releaseBurstsOfRigidParticlesInASequenceForever(Emitter&, RigidParticlePool pool, int numOfParticlesPerGroup, float rate, Vector3 acceleration, float mass);

// Particle movement functions
ParticleBody makeParticleRigidBody(Particle* p, Vector3 acceleration, float mass);
void updateParticlePosition(Particle&, float timeStep);
void updateParticlePositionUnderForces(ParticleBody&, Vector3* forces, int numOfForces, float timeStep);

// Particle Pool Functions
int updatePool(ParticlePool&, long int mspf);
int updateRigidPool(RigidParticlePool&);
void deleteParticleInPool(ParticlePool&, int current, int inactive);
void deleteRigidParticleInPool(RigidParticlePool&, int current, int inactive);
