#include "Plane.h"

/**
* Constructor for the Plane.
*/
Plane::Plane(const glm::vec3 & point, const glm::vec3 & normal, const color & material)
	: Surface(material), a(point), n(normalize(normal))
{
}

Plane::Plane(std::vector<glm::vec3> vertices, const color & material)
	: Surface(material)
{
	a = vertices[0];

	n = glm::normalize(glm::cross(vertices[2] - vertices[1], vertices[0] - vertices[1]));
}


Plane::~Plane(void)
{
}

/*
* Checks a ray for intersection with the surface. Finds the closest point of intersection
* if one exits. Returns a HitRecord with the t parmeter set to FLT_MAX if there is no
* intersection.
*/
HitRecord Plane::findClosestIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection)
{
	HitRecord hitRecord;

	// TODO
	if (dot(n, rayDirection) > 0)
		n = -n;
	float discriminant = glm::dot(rayDirection, n);
	if (discriminant == 0) {
		hitRecord.t = FLT_MAX;
	}
	else {
		hitRecord.t = glm::dot((a - rayOrigin), n) / discriminant;

		if (hitRecord.t < 0)
			hitRecord.t = FLT_MAX;

		hitRecord.interceptPoint = rayOrigin + hitRecord.t * rayDirection;
		hitRecord.material = material;
		
		hitRecord.surfaceNormal = n;
	}

	return hitRecord;

} // end checkIntercept

