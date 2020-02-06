#include "Particles.h"

Particle::Particle() {
	displacement = { 0.0, 0.0, 0.0 };
	velocity = { 0.0, 0.0, 0.0 };
	life = 60;
}

Particle::Particle(Vector3 initialDisplacement, Vector3 initialVelocity, int life) {
	displacement = initialDisplacement;
	velocity = initialVelocity;
	this->life = life;
}

Emitter::Emitter() {
	position = { 0.0, 0.0, 0.0 };
	radius = 0.0;
	normalSpeedVector = { 0.0, 1.0, 0.0 };
	rateOfParticleRelease = 10;
	maxAngleAroundX = maxAngleAroundY = maxAngleAroundZ = 1.0;
	minLife = 60;
	maxLife = 500;
}

Emitter::Emitter(Vector3 position, float radius, Vector3 normalSpeedVector, float rate, float maxAngleAroundX, float maxAngleAroundY, float maxAngleAroundZ, int minLife, int maxLife) {
	this->position = position;
	this->radius = radius;
	this->normalSpeedVector = normalSpeedVector;
	rateOfParticleRelease = rate;
	this->maxAngleAroundX = maxAngleAroundX;
	this->maxAngleAroundY = maxAngleAroundY;
	this->maxAngleAroundZ = maxAngleAroundZ;
	this->minLife = minLife;
	this->maxLife = maxLife;
}

Particle releaseOneParticle(Emitter& e) {
	// Generate a position
	float p1 = (((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2) - 1) * e.radius;
	float p2 = (((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2) - 1) * e.radius;
	float p3 = (((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2) - 1) * e.radius;
	Vector3 pos = { e.position.x + p1, e.position.y + p2, e.position.z + p3 };
	// Generate the velocity in which it is released
	float xAngle = (((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2) - 1) * e.maxAngleAroundX;
	float yAngle = (((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2) - 1) * e.maxAngleAroundY;
	float zAngle = (((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2) - 1) * e.maxAngleAroundZ;
	Vector3 vel = { 0.0, 0.0, 0.0 };
	// Generate a lifespan
	float life = e.minLife + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (e.maxLife - e.minLife)));
	return Particle(pos,vel,life);
}
