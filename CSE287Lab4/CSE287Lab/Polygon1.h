#pragma once
#include "Plane.h"
class Polygon1 : public Plane
{
public:
	Polygon1(std::vector<glm::vec3> vertices, const color & material);

	~Polygon1();

	std::vector<glm::vec3> vertices;

	virtual HitRecord findClosestIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection);
};

