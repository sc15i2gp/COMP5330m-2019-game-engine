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
	speedXDifference = speedYDifference = speedZDifference = 1.0;
	minLife = 60;
	maxLife = 500;
}

Emitter::Emitter(Vector3 position, float radius, Vector3 normalSpeedVector, float rate, float speedXDifference, float speedYDifference, float speedZDifference, int minLife, int maxLife) {
	this->position = position;
	this->radius = radius;
	this->normalSpeedVector = normalSpeedVector;
	rateOfParticleRelease = rate;
	this->speedXDifference = speedXDifference;
	this->speedYDifference = speedYDifference;
	this->speedZDifference = speedZDifference;
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
	float v1 = (((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2) - 1) * e.speedXDifference;
	float v2 = (((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2) - 1) * e.speedYDifference;
	float v3 = (((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2) - 1) * e.speedZDifference;
	Vector3 vel = { e.normalSpeedVector.x + v1, e.normalSpeedVector.y + v2, e.normalSpeedVector.z + v3 };
	// Generate a lifespan
	float life = e.minLife + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (e.maxLife - e.minLife)));
	return Particle(pos,vel,life);
}
