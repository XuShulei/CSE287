#pragma once

#include "BasicIncludesAndDefines.h"
#include "HitRecord.h"
#include "Surface.h"

HitRecord findIntersection(const glm::vec3 &e, const glm::vec3 &d, const std::vector<std::shared_ptr<Surface>> & surfaces);


/**
* Base struct for all types of lights. Supports only specification of the 
* color and intensity of light sources. Parent of sub-structs that simulate
* positional, directional, and spot lights.
*
* Instances of this struct an be used to simulate only ambient light. Ambient
* light is described as bounced light that has been scattered so much that it
* is impossible to tell the direction to its source. If using a LightSource 
* instantiation to simulate ambient light, the overall intensity of the light
* should be low in order to avoid washing out shadows as well as diffuse and 
* specular effects that can be achieved when using children of this struct.
*/
struct LightSource
{
	LightSource(const color & lightColor) 
	: lightColor(lightColor)
	{}

	virtual color illuminate(const glm::vec3 & eyeVector, HitRecord & closestHit, const std::vector<std::shared_ptr<Surface>> & surfaces)
	{
		return lightColor * closestHit.material.ambientColor;
	}

	/* 
	* Color and intensity of the light.
	*/
	color lightColor; 
};

/**
* Struct for simulating light sources that have an explicit position and 
* shine equally in all directions. Instantiations of the struct will have
* a position property and a color and intensity of the light given off
* by the light.
*/
struct PositionalLight : public LightSource
{
	PositionalLight(glm::vec3 position, const color & lightColor)
	: LightSource(lightColor), lightPosition(position)
	{}

	virtual color illuminate(const glm::vec3 & eyeVector, HitRecord & closestHit, const std::vector<std::shared_ptr<Surface>> & surfaces)
	{
		HitRecord h = findIntersection(closestHit.interceptPoint + EPSILON*closestHit.surfaceNormal, lightPosition - closestHit.interceptPoint, surfaces);
		if (h.t < FLT_MAX) {
			return color(0.0f, 0.0f, 0.0f, 1.0f);
		}
		glm::vec3 l = glm::normalize(lightPosition - closestHit.interceptPoint);
		color diffuse = glm::max(0.0f, glm::dot(closestHit.surfaceNormal, l)) * lightColor * closestHit.material.diffuseColor;
		
		glm::vec3 v = glm::normalize(eyeVector - closestHit.interceptPoint);
		glm::vec3 r = glm::normalize(-l - 2 * glm::dot(-l, closestHit.surfaceNormal)*closestHit.surfaceNormal);
		color specular = glm::pow(glm::max(0.0f, glm::dot(r, v)), closestHit.material.shininess) * lightColor * closestHit.material.specularColor;
		return diffuse + specular;
	}

	/**
	* x, y, z position of the light source. 
	*/
	glm::vec3 lightPosition; 
};

/**
* Struct for simulating light sources that do not have an explicit position.
* Such light sources have only a direction against which they are shinning.
* Instantiations of the struct will have this direction properties along with
* a color and intensity of the light given off by the light source.
*/
struct DirectionalLight : public LightSource
{
	DirectionalLight(glm::vec3 direction, const color & lightColor)
	: LightSource(lightColor), lightDirection(glm::normalize(direction))
	{}

	virtual color illuminate(const glm::vec3 & eyeVector, HitRecord & closestHit, const std::vector<std::shared_ptr<Surface>> & surfaces)
	{
		HitRecord h = findIntersection(closestHit.interceptPoint + EPSILON*closestHit.surfaceNormal, lightDirection, surfaces);
		if (h.t < FLT_MAX) {
			return color(0.0f, 0.0f, 0.0f, 1.0f);
		}
		glm::vec3 v = glm::normalize(eyeVector - closestHit.interceptPoint);
		glm::vec3 r = glm::normalize(lightDirection - 2 * glm::dot(lightDirection, closestHit.surfaceNormal)*closestHit.surfaceNormal);

		color diffuse = glm::max(0.0f, glm::dot(closestHit.surfaceNormal, lightDirection)) * lightColor * closestHit.material.diffuseColor;		
		color specular = glm::pow(glm::max(0.0f, glm::dot(r, v)), closestHit.material.shininess) * lightColor * closestHit.material.specularColor;
		return diffuse + specular;
	}

	/**
	* Unit vector that points in the direction that is opposite 
	* the direction in which the light is shining.
	*/
	glm::vec3 lightDirection; 
};


