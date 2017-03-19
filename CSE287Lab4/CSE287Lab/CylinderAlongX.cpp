#include "CylinderAlongX.h"



CylinderAlongX::CylinderAlongX(const glm::vec3 & position, float radius, float length, const color & material)
	: QuadricSurface(position, material), radius(radius), length(length)
{
	A = 0;
	B = 1 / std::pow(radius,2);
	C = 1 / std::pow(radius, 2);
	D = 0;
	E = 0;
	F = 0;
	G = 0;
	H = 0;
	I = 0;
	J = -1;
}


CylinderAlongX::~CylinderAlongX()
{
}

HitRecord CylinderAlongX::findClosestIntersection(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection)
{
	HitRecord hit = QuadricSurface::findClosestIntersection(rayOrigin, rayDirection);
	if (hit.t == FLT_MAX)
		return hit;

	float d = glm::length(hit.interceptPoint - center);
	if (glm::pow(d, 2) - glm::pow(radius, 2) > glm::pow(length / 2, 2)) {
		hit = QuadricSurface::findClosestIntersection(hit.interceptPoint + 0.001f * rayDirection, rayDirection);
		hit.t = glm::length(hit.interceptPoint - rayOrigin);
		d = glm::length(hit.interceptPoint - center);
		if (glm::pow(d, 2) - glm::pow(radius, 2) > glm::pow(length / 2, 2))
			hit.t = FLT_MAX;
	}
	
	return hit;
}
