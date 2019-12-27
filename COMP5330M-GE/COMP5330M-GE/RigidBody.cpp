#include "RigidBody.h"

RigidBody::RigidBody() {
	displacement = { 0.0,0.0,0.0 };
	velocity = { 0.0,0.0,0.0 };
	acceleration = { 0.0,0.0,0.0 };
	mass = 1.0;
}

RigidBody::RigidBody(Mesh mesh, Vector3 initialVelocity, Vector3 initialAcceleration, float mass) {
	Vector3 verticesSum = { 0.0,0.0,0.0 };
	for (int i = 0; i < mesh.number_of_vertices; ++i) {
		verticesSum += mesh.vertices[i].position;
	}
	verticesSum /= mesh.number_of_vertices;
	displacement = verticesSum;
	velocity = initialVelocity;
	acceleration = initialAcceleration;
	this->mass = mass;
}

bool checkForCollision(RigidBody& r) {
	return true;
}

Mesh_vertex cylinderCollision(RigidBody& r, Mesh mesh) {
	float rminx = 0.0; float rminy = 0.0; float rminz = 0.0;
	float rmaxx = 0.0; float rmaxy = 0.0; float rmaxz = 0.0;
	for (int i = 0; i < r.mesh.number_of_vertices; ++i) {
		if (r.mesh.vertices[i].position.x > rmaxx) rmaxx = r.mesh.vertices[i].position.x;
		if (r.mesh.vertices[i].position.y > rmaxy) rmaxy = r.mesh.vertices[i].position.y;
		if (r.mesh.vertices[i].position.z > rmaxz) rmaxz = r.mesh.vertices[i].position.z;
		if (r.mesh.vertices[i].position.x < rminx) rminx = r.mesh.vertices[i].position.x;
		if (r.mesh.vertices[i].position.y < rminy) rminy = r.mesh.vertices[i].position.y;
		if (r.mesh.vertices[i].position.z < rminz) rminz = r.mesh.vertices[i].position.z;
	}
	for (int j = 0; j < mesh.number_of_vertices; ++j) {

	}
}

void updateDisplacement(RigidBody& r, Vector3* forces, int numOfForces, float timeStep) {
	r.displacement = r.displacement + (timeStep * r.velocity) + (0.5 * (timeStep * timeStep) * r.acceleration);
	r.velocity = r.velocity + (0.5 * timeStep * r.acceleration);
	Vector3 totalForce = { 0.0,0.0,0.0 };
	for (int i = 0; i < numOfForces; ++i) {
		totalForce += forces[i];
	}
	r.acceleration = totalForce / r.mass;
	r.velocity = r.velocity + (0.5 * timeStep * r.acceleration);
	if (checkForCollision(r)) {
		float cor = 0.5;
		Vector3 v = { 0.0,1.0,0.0 };
		normalise(v);
		r.velocity.x = r.velocity.x * v.x * cor;
		r.velocity.y = r.velocity.y * v.y * cor;
		r.velocity.z = r.velocity.z * v.z * cor;
	}
}