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
	normalReleaseVector = { 0.0, 1.0, 0.0 };
	rateOfParticleRelease = 10;
	maxAngle = 90;
	minLife = 60;
	maxLife = 500;
}

Emitter::Emitter(Vector3 position, float radius, Vector3 normalReleaseVector, float rate, float maxAngle, int minLife, int maxLife) {
	this->position = position;
	this->radius = radius;
	this->normalReleaseVector = normalReleaseVector;
	rateOfParticleRelease = rate;
	this->maxAngle = maxAngle;
	this->minLife = minLife;
	this->maxLife = maxLife;
}

Particle releaseOneParticle(Emitter& e) {

}
