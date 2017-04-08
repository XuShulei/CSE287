#pragma once



#include <iostream>
#include <vector>
#include <time.h>
#include <memory>

#define GLM_SWIZZLE

#include <GL/freeglut.h>

#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/constants.hpp>

#include <glm/gtx/rotate_vector.hpp>
//#include <glm/gtx/euler_angles.hpp>
//#include <glm/gtx/quaternion.hpp>

/*
* Preprocessor statement for text substitution
*/

// Attenuation factors
#define CONSTANT_ATTEN 1.0f
#define LINEAR_ATTEN 0.01f
#define QUADRATIC_ATTEN 0.001f

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 500

#define color glm::vec4

#define EPSILON 1.0E-4f

#define PI glm::pi<float>()

color getRandomColor();


struct VertexData {

	glm::vec4 position;
	color material;
	glm::vec3 normal;
	glm::vec3 worldPosition;

	VertexData( const glm::vec4 & pos = glm::vec4(0.0, 0.0, 0.0, 0.0),
				const color & col = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f),
				const glm::vec3 & norm = glm::vec3(0.0, 0.0, 1.0) )
	{
		position = pos;
		material = col;
		normal = glm::normalize(norm);

	}

};

struct Plane {

	glm::vec3 a; // point on the plane
	glm::vec3 n; // vector that is normal to the front face of a plane

	Plane(){};

	Plane(glm::vec3 point, glm::vec3 normal)
	{
		a = point;
		n = glm::normalize(normal);
	}

	// Returns true is a point is to the "left" of a plane.
	bool insidePlane(const VertexData & point)
	{
		// If dot product is positive the point is on the "positive" side of the plane
		if (glm::dot((point.position.xyz - a), n) > 0.0) {
			return true;
		}
		else {
			return false;
		}

	} // end insidePlane

	// Finds the point of intersection for a line segment and a plane
	VertexData findIntersection(const VertexData &  p1, const VertexData & p2)
	{
		// Find the distance of each point from the plane
		float d1 = glm::dot(p1.position.xyz - a, n);
		float d2 = glm::dot(p2.position.xyz - a, n);

		// Find the paramter of the intercept with the plane
		float t = d1 / (d1 - d2);

		// Generate the vertex data for the point of intersection
		VertexData I;
		// Find point the coordinates of the intersection with the plane
		I.position = p1.position + t * (p2.position - p1.position);

		// Find interpolated values for the vertex data
		I.material = p1.material + t * (p2.material - p1.material);
		I.normal = p1.normal + t * (p2.normal - p1.normal);

		return I;

	} // end findIntersection
};

// Structure for holding the material properties of a object
struct Material
{
	color ambientMat;
	color diffuseMat;
	color specularMat;
	float specularExp;
	color emmisiveMat;
};

void print(const glm::vec2 & v0);

void print(const glm::vec3 & v0);

void print(const glm::vec4 & v0);

void print(const glm::mat2 & m);

void print(const glm::mat3 & m);

void print(const glm::mat4 & m);

// Give three vertices on the face of a polygon in counter clockwise 
// order calculates a normal vector for the polygon.
glm::vec3 findUnitNormal(glm::vec3 pZero, glm::vec3 pOne, glm::vec3 pTwo);

