#include "Polygon1.h"


Polygon1::Polygon1(std::vector<glm::vec3> vertices, const color & material)
	: Plane(vertices, material), vertices(vertices)
{
}


Polygon1::~Polygon1()
{
}

HitRecord Polygon1::findClosestIntersection(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection)
{
	HitRecord hitRecord = Plane::findClosestIntersection(rayOrigin, rayDirection);
	
	for (unsigned int i = 0; i < vertices.size()-1; i++) {
		if (glm::length(glm::normalize(glm::cross(vertices[i + 1] - vertices[i], hitRecord.interceptPoint - vertices[i])) - n) > 0.001f) {
			hitRecord.t = FLT_MAX;
			break;
		}
	}
	if (glm::length(glm::normalize(glm::cross(vertices[0] - vertices[vertices.size()-1], hitRecord.interceptPoint - vertices[vertices.size() - 1])) - n) > 0.001f)
		hitRecord.t = FLT_MAX;
		
	return hitRecord;
}
