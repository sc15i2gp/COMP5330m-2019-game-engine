#include "RigidBody.h"

RigidBody::RigidBody() {
	displacement = { 0.0,0.0,0.0 };
	velocity = { 0.0,0.0,0.0 };
	acceleration = { 0.0,0.0,0.0 };
	mass = 1.0;
	minx = -1.0;
	maxx = 1.0;
	miny = -1.0;
	maxy = 1.0;
	minz = -1.0;
	maxz = 1.0;
}

RigidBody::RigidBody(Mesh mesh, Vector3 initialVelocity, Vector3 initialAcceleration, float mass) {
	this->mesh = mesh;
	Vector3 verticesSum = { 0.0,0.0,0.0 };
	minx = maxy = miny = maxx = minz = maxz = 0.0;
	for (int i = 0; i < mesh.number_of_vertices; ++i) {
		verticesSum += mesh.vertices[i].position;
		if (mesh.vertices[i].position.x > maxx) maxx = mesh.vertices[i].position.x;
		if (mesh.vertices[i].position.y > maxy) maxy = mesh.vertices[i].position.y;
		if (mesh.vertices[i].position.z > maxz) maxz = mesh.vertices[i].position.z;
		if (mesh.vertices[i].position.x < minx) minx = mesh.vertices[i].position.x;
		if (mesh.vertices[i].position.y < miny) miny = mesh.vertices[i].position.y;
		if (mesh.vertices[i].position.z < minz) minz = mesh.vertices[i].position.z;
	}
	verticesSum /= mesh.number_of_vertices;
	displacement = verticesSum;
	velocity = initialVelocity;
	acceleration = initialAcceleration;
	this->mass = mass;
}

bool cylinderCollision(RigidBody& r, Mesh mesh) {
	for (int i = 0; i < mesh.number_of_vertices; ++i) {
		float x1 = mesh.vertices[i].position.x - r.maxx;
		float y1 = mesh.vertices[i].position.y - r.maxy;
		float z1 = mesh.vertices[i].position.z - r.maxz;
		float x2 = r.minx - mesh.vertices[i].position.x;
		float y2 = r.miny - mesh.vertices[i].position.y;
		float z2 = r.minz - mesh.vertices[i].position.z;
		if (x1 <= 0.0 || y1 <= 0.0 || z1 <= 0.0 || x2 <= 0.0 || y2 <= 0.0 || z2 <= 0.0) {
			return true;
		}
	}
	return false;
}

void updateVectorPositions(RigidBody& r, Vector3 difference)
{
	for (int i = 0; i < r.mesh.number_of_vertices; ++i) {
		r.mesh.vertices[i].position += difference;
	}
}

void updateDisplacement(RigidBody& r, Vector3* forces, int numOfForces, float timeStep) {
	Vector3 initialDis = r.displacement;
	r.displacement = r.displacement + (timeStep * r.velocity) + ((0.5 * timeStep * timeStep) * r.acceleration);
	Vector3 finalDis = r.displacement;
	Vector3 disDiff = finalDis - initialDis;
	updateVectorPositions(r, disDiff);
	r.velocity = r.velocity + (0.5 * timeStep * r.acceleration);
	Vector3 totalForce = { 0.0,0.0,0.0 };
	for (int i = 0; i < numOfForces; ++i) {
		totalForce += forces[i];
	}
	r.acceleration = totalForce / r.mass;
	r.velocity = r.velocity + (0.5 * timeStep * r.acceleration);
}
