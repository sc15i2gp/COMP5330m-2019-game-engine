#include "RigidBody.h"

// Default constructor
RigidBody::RigidBody() {
	displacement = { 0.0,0.0,0.0 };
	velocity = { 0.0,0.0,0.0 };
	acceleration = { 0.0,0.0,0.0 };
	mass = 1.0;
	radius = 1.0;
}

// Parameterised constructor
RigidBody::RigidBody(Vector3 initialDisplacement, Vector3 initialVelocity, Vector3 initialAcceleration, float mass, float radius) {
	displacement = initialDisplacement;
	velocity = initialVelocity;
	acceleration = initialAcceleration;
	this->mass = mass;
	this->radius = radius;
}

// Update the displacement, velocity, and acceleration using Velocity Verlet
// We return the difference in displacement made to translate the mesh
Vector3 updateDisplacement(RigidBody& r, Vector3* forces, int numOfForces, float timeStep) {
	Vector3 initialDis = r.displacement;
	r.displacement = r.displacement + (timeStep * r.velocity) + ((0.5 * timeStep * timeStep) * r.acceleration);
	Vector3 finalDis = r.displacement;
	Vector3 disDiff = finalDis - initialDis;
	r.velocity = r.velocity + (0.5 * timeStep * r.acceleration);
	Vector3 totalForce = { 0.0,0.0,0.0 };
	for (int i = 0; i < numOfForces; ++i) {
		totalForce += forces[i];
	}
	r.acceleration = totalForce / r.mass;
	r.velocity = r.velocity + (0.5 * timeStep * r.acceleration);
	return disDiff;
}

bool checkSphereCylinderCollision(RigidBody& r, float x, float z, float miny, float maxy, float radius) {
	// Check for collision with the side of the cylinder
	if (miny <= r.displacement.y <= maxy) {
		Vector3 m = { x, r.displacement.y, z };
		Vector3 distanceBetweenCenters = m - r.displacement;
		/* If the distance between the centres of the shapes is <=
			the sum of their radii, then they are colliding
		*/
		return length(distanceBetweenCenters) <= r.radius + radius;
	}
	// Check for collision with the top of the cylinder
	else if (r.displacement.y > maxy) {
		Vector3 cylinderTopCentre = { x, maxy, z };
		// Suppose there is a line parallel to the y-axis passing the centre of the sphere 
		Vector3 lineCrossesPlane = { r.displacement.x, maxy, r.displacement.z };
		Vector3 distanceBetween = cylinderTopCentre - lineCrossesPlane;
		// If the sphere is passing the top of the cylinder and is in the plane of the cylinder
		return r.displacement.y - r.radius <= maxy && length(distanceBetween) <= radius;
	}
	// Check for collision with the bottom of the cylinder
	else if (r.displacement.y < miny) {
		Vector3 cylinderBottomCentre = { x, miny, z };
		Vector3 lineCrossesPlane = { r.displacement.x, miny, r.displacement.z };
		Vector3 distanceBetween = cylinderBottomCentre - lineCrossesPlane;
		// If the sphere is passing the bottom of the cylinder and is in the plane of the cylinder
		return r.displacement.y + r.radius >= miny && length(distanceBetween) <= radius;
	}
}
