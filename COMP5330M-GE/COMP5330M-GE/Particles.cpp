#include "Particles.h"

Particle::Particle() 
{
	displacement = { 0.0, 0.0, 0.0 };
	velocity = { 0.0, 0.0, 0.0 };
	life = 60;
}

Particle::Particle(Vector3 initialDisplacement, Vector3 initialVelocity, int life) 
{
	displacement = initialDisplacement;
	velocity = initialVelocity;
	this->life = life;
}

ParticleBody::ParticleBody()
{
	Particle *p = new Particle();
	particle = p;
	acceleration = { 0.0,0.0,0.0 };
	mass = 1.0;
}

ParticleBody::ParticleBody(Particle* particle, Vector3 acceleration, float mass)
{
	this->particle = particle;
	this->acceleration = acceleration;
	this->mass = mass;
}

Emitter::Emitter()
{
	position = { 0.0, 0.0, 0.0 };
	radius = 0.0;
	normalSpeedVector = { 0.0, 1.0, 0.0 };
	maxAngleAroundX = maxAngleAroundY = maxAngleAroundZ = 30.0;
	minSpeedRatio = 0.5;
	minLife = 60;
	maxLife = 500;
}

Emitter::Emitter(Vector3 position, float radius, Vector3 normalSpeedVector, float maxAngleAroundX, float maxAngleAroundY, float maxAngleAroundZ, float minSpeedRatio, int minLife, int maxLife)
{
	this->position = position;
	this->radius = radius;
	this->normalSpeedVector = normalSpeedVector;
	this->maxAngleAroundX = maxAngleAroundX;
	this->maxAngleAroundY = maxAngleAroundY;
	this->maxAngleAroundZ = maxAngleAroundZ;
	this->minSpeedRatio = minSpeedRatio;
	this->minLife = minLife;
	this->maxLife = maxLife;
}

Particle releaseOneParticle(Emitter& e) 
{
	// Generate a position
	float p1 = (((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2) - 1) * e.radius;
	float p2 = (((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2) - 1) * e.radius;
	float p3 = (((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2) - 1) * e.radius;
	Vector3 pos = { e.position.x + p1, e.position.y + p2, e.position.z + p3 };
	// Generate the velocity in which it is released
	float xAngle = (((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2) - 1) * e.maxAngleAroundX;
	float yAngle = (((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2) - 1) * e.maxAngleAroundY;
	float zAngle = (((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2) - 1) * e.maxAngleAroundZ;
	// Rotate around the x-axis
	Vector3 vel = { e.normalSpeedVector.x, 
		(e.normalSpeedVector.y * static_cast <float> (cos_deg(xAngle))) - (e.normalSpeedVector.z * static_cast <float> (sin_deg(xAngle))), 
		(e.normalSpeedVector.y * static_cast <float> (sin_deg(xAngle))) + (e.normalSpeedVector.z * static_cast <float> (cos_deg(xAngle))) };
	// Rotate around the y-axis
	vel = { (vel.x * static_cast <float> (cos_deg(yAngle))) + (vel.z * static_cast <float> (sin_deg(yAngle))),
		vel.y,
		(-vel.x * static_cast <float> (sin_deg(yAngle))) + (vel.z * static_cast <float> (cos_deg(yAngle))) };
	// Rotate around the z-axis
	vel = { (vel.x * static_cast <float> (cos_deg(zAngle))) - (vel.y * static_cast <float> (sin_deg(zAngle))),
		(vel.x * static_cast <float> (sin_deg(zAngle))) + (vel.y * static_cast <float> (cos_deg(zAngle))),
		vel.z };
	// Multiply the speed by a random value between minSpeedRatio and 1.0
	float ratio = e.minSpeedRatio + (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - e.minSpeedRatio))));
	vel = ratio * vel;
	// Generate a lifespan
	int life = e.minLife + (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (e.maxLife - e.minLife))));
	return Particle(pos,vel,life);
}

Particle simpleReleaseOneParticle(Emitter& e)
{
	return Particle(e.position, e.normalSpeedVector, e.maxLife);
}

ParticleBody releaseOneRigidParticle(Emitter& e, Vector3 acceleration, float mass)
{
	Particle p = releaseOneParticle(e);
	return ParticleBody(&p, acceleration, mass);
}

ParticleBody simpleReleaseOneRigidParticle(Emitter& e, Vector3 acceleration, float mass)
{
	Particle p = simpleReleaseOneParticle(e);
	return ParticleBody(&p, acceleration, mass);
}

Particle* releaseManyParticlesAtOnce(Emitter& e, int numOfParticles)
{
	Particle* particles = new Particle[numOfParticles];
	for (int i = 0; i < numOfParticles; i++) {
		particles[i] = releaseOneParticle(e);
	}
	return particles;
}

// Should be run in a thread so Sleep() does not affect the whole program
Particle* releaseManyParticlesInASequence(Emitter& e, int numOfParticles, float rate)
{
	Particle* particles = new Particle[numOfParticles];
	float time = 1000.0 / rate;
	for (int i = 0; i < numOfParticles; i++) {
		particles[i] = releaseOneParticle(e);
		Sleep(time);
	}
	return particles;
}

Particle* releaseBurstsOfParticlesInASequence(Emitter& e, int numOfBursts, int numOfParticlesPerGroup, float rate)
{
	Particle* particles = new Particle[numOfBursts * numOfParticlesPerGroup];
	float time = 1000.0 / rate;
	for (int i = 0; i < numOfBursts; i++) {
		particles = releaseManyParticlesAtOnce(e, numOfParticlesPerGroup);
		particles += numOfParticlesPerGroup;
		Sleep(time);
	}
	return particles;
}

ParticleBody* releaseManyRigidParticlesAtOnce(Emitter& e, int numOfParticles, Vector3 acceleration, float mass)
{
	ParticleBody* particles = new ParticleBody[numOfParticles];
	for (int i = 0; i < numOfParticles; i++) {
		particles[i] = releaseOneRigidParticle(e, acceleration, mass);
	}
	return particles;
}

ParticleBody* releaseManyRigidParticlesInASequence(Emitter& e, int numOfParticles, float rate, Vector3 acceleration, float mass)
{
	ParticleBody* particles = new ParticleBody[numOfParticles];
	float time = 1000.0 / rate;
	for (int i = 0; i < numOfParticles; i++) {
		particles[i] = releaseOneRigidParticle(e, acceleration, mass);
		Sleep(time);
	}
	return particles;
}

ParticleBody* releaseBurstsOfRigidParticlesInASequence(Emitter& e, int numOfBursts, int numOfParticlesPerGroup, float rate, Vector3 acceleration, float mass)
{
	ParticleBody* particles = new ParticleBody[numOfBursts * numOfParticlesPerGroup];
	float time = 1000.0 / rate;
	for (int i = 0; i < numOfBursts; i++) {
		particles = releaseManyRigidParticlesAtOnce(e, numOfParticlesPerGroup, acceleration, mass);
		particles += numOfParticlesPerGroup;
		Sleep(time);
	}
	return particles;
}

ParticleBody makeParticleRigidBody(Particle* p, Vector3 acceleration, float mass)
{
	return ParticleBody(p, acceleration, mass);
}

Vector3 updateParticlePosition(Particle& p, float timeStep)
{
	Vector3 initialDis = p.displacement;
	p.displacement += timeStep * p.velocity;
	Vector3 finalDis = p.displacement;
	Vector3 diffDis = finalDis - initialDis;
	p.life -= 1;
	return diffDis;
}

Vector3 updateParticlePositionUnderForces(ParticleBody& p, Vector3* forces, int numOfForces, float timeStep)
{
	Vector3 initialDis = p.particle->displacement;
	p.particle->displacement = p.particle->displacement + (timeStep * p.particle->velocity) + ((0.5 * timeStep * timeStep) * p.acceleration);
	Vector3 finalDis = p.particle->displacement;
	Vector3 disDiff = finalDis - initialDis;
	p.particle->velocity = p.particle->velocity + (0.5 * timeStep * p.acceleration);
	Vector3 totalForce = { 0.0,0.0,0.0 };
	for (int i = 0; i < numOfForces; ++i) {
		totalForce += forces[i];
	}
	p.acceleration = totalForce / p.mass;
	p.particle->velocity = p.particle->velocity + (0.5 * timeStep * p.acceleration);
	p.particle->life -= 1;
	return disDiff;
}
