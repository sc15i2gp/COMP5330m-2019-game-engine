#include "RigidBody.h"

// Default constructor
RigidBody::RigidBody() 
{
	displacement = { 0.0,0.0,0.0 };
	velocity = { 0.0,0.0,0.0 };
	acceleration = { 0.0,0.0,0.0 };
	mass = 1.0;
	radius = 1.0;
}

// Parameterised constructor
RigidBody::RigidBody(Vector3 initialDisplacement, Vector3 initialVelocity, Vector3 initialAcceleration, float mass, float radius) 
{
	displacement = initialDisplacement;
	velocity = initialVelocity;
	acceleration = initialAcceleration;
	this->mass = mass;
	this->radius = radius;
}

// Update the displacement, velocity, and acceleration using Velocity Verlet
// We return the difference in displacement made to translate the mesh
void updateDisplacement(RigidBody& r, Vector3* forces, int numOfForces, float timeStep) 
{
	float half = 0.5;
	r.displacement = r.displacement + (timeStep * r.velocity) + ((half * timeStep * timeStep) * r.acceleration);
	r.velocity = r.velocity + (half * timeStep * r.acceleration);
	Vector3 totalForce = { 0.0,0.0,0.0 };
	for (int i = 0; i < numOfForces; ++i) {
		totalForce += forces[i];
	}
	r.acceleration = totalForce / r.mass;
	r.velocity = r.velocity + (half * timeStep * r.acceleration);
}

bool checkSphereCylinderCollision(RigidBody& r, float x, float z, float miny, float maxy, float radius) 
{
	// Check for collision with the side of the cylinder
	if (miny <= r.displacement.y || r.displacement.y <= maxy) {
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
		Vector3 lineCrossesSphere = { r.displacement.x, maxy, r.displacement.z };
		Vector3 distanceBetween = cylinderTopCentre - lineCrossesSphere;
		// If the sphere is passing the top of the cylinder and is in the plane of the cylinder
		return r.displacement.y - r.radius <= maxy && length(distanceBetween) <= radius;
	}
	// Check for collision with the bottom of the cylinder
	else if (r.displacement.y < miny) {
		Vector3 cylinderBottomCentre = { x, miny, z };
		Vector3 lineCrossesSphere = { r.displacement.x, miny, r.displacement.z };
		Vector3 distanceBetween = cylinderBottomCentre - lineCrossesSphere;
		// If the sphere is passing the bottom of the cylinder and is in the plane of the cylinder
		return r.displacement.y + r.radius >= miny && length(distanceBetween) <= radius;
	}
}

bool checkSphereCylinderCollisionInPath(RigidBody& r, Vector3 pathTaken, float x, float z, float miny, float maxy, float radius)
{
	int numOfTimeSteps = length(pathTaken);
	for (float i = 0.0; i <= 1.0; i += 1.0f / numOfTimeSteps) {
		Vector3 position = r.displacement + (i * pathTaken);
		if (checkSphereCylinderCollision(r, x, z, miny, maxy, radius)) {
			return true;
		}
	}
	return false;
}

bool checkSphereTriangleCollision(RigidBody& r, Vector3 v1, Vector3 v2, Vector3 v3)
{
	// First, check if vectors are inside sphere
	Vector3 c1 = v1 - r.displacement;
	Vector3 c2 = v2 - r.displacement;
	Vector3 c3 = v3 - r.displacement;
	if (length(c1) <= r.radius) return true;
	else if (length(c2) <= r.radius) return true;
	else if (length(c3) <= r.radius) return true;
	// Then check the distance from sphere to plane of triangle
	Vector3 edge1 = v2 - v1;
	Vector3 edge2 = v3 - v1;
	Vector3 normal = cross(edge1, edge2);
	// Equation of a plane: Ax + By + Cz + D = 0
	float d = -dot(normal, v1);
	// Distance from point to plane = abs(Ax0 + By0 + Cz0 + D) / sqrt(A^2 + B^2 + C^2)
	float num = dot(normal, r.displacement) + d;
	if (num < 0) num *= -1.0;
	float den = length(normal);
	return (num / den) <= r.radius;
}

float checkSphereTriangleCollisionInPath(RigidBody& r, Vector3 pathTaken, Vector3 v1, Vector3 v2, Vector3 v3)
{
	int numOfTimeSteps = length(pathTaken);
	for (float i = 0.0; i <= 1.0; i += 1.0f / numOfTimeSteps) {
		Vector3 position = r.displacement + (i * pathTaken);
		if (checkSphereTriangleCollision(r, v1, v2, v3)) {
			return true;
		}
	}
	return false;
}
