#pragma once
#include "QuadricSurface.h"
class CylinderAlongX :
	public QuadricSurface
{
public:
	CylinderAlongX(const glm::vec3 & position = glm::vec3(0.0f, 0.0f, -5.0f),
		float radius = 1.0f,
		float length = 1.0f,
		const color & material = color(1.0f, 1.0f, 1.0f, 1.0f));
	~CylinderAlongX();

	virtual HitRecord findClosestIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection);

	float radius;
	float length;

};

