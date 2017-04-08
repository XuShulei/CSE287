#pragma once

#include "BasicIncludesAndDefines.h"

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
	LightSource(const color & colorOfLight) 
	: lightColor(colorOfLight), enabled(true)
	{}

	virtual color illuminate(const glm::vec3 & eyePosition, const glm::vec3 & worldPosition, glm::vec3 worldNormal, color material)
	{
		if (enabled) {

			return lightColor * material;
		}
		else {

			return glm::vec4(0.0f);
		}
	}

	/* 
	* Color and intensity of the light.
	*/
	color lightColor; 

	bool enabled;
};

/**
* Struct for simulating light sources that have an explicit position and 
* shine equally in all directions. Instantiations of the struct will have
* a position property and a color and intensity of the light given off
* by the light.
*/
struct PositionalLight : public LightSource
{
	PositionalLight(glm::vec3 position, const color & colorOfLight)
	: LightSource(colorOfLight), position(position)
	{}

	virtual color illuminate(const glm::vec3 & eyePosition, const glm::vec3 & worldPosition, glm::vec3 worldNormal, color material)
	{

		return LightSource::illuminate(eyePosition, worldPosition, worldNormal, material);
	}

	/**
	* x, y, z position of the light source. 
	*/
	glm::vec3 position; 
};

/**
* Struct for simulating light sources that do not have an explicit position.
* Such light sources have only a direction against which they are shinning.
* Instantiations of the struct will have this direction properties along with
* a color and intensity of the light given off by the light source.
*/
struct DirectionalLight : public LightSource
{
	DirectionalLight(glm::vec3 direction, const color & colorOfLight)
	: LightSource(colorOfLight), direction(glm::normalize(direction))
	{}

	virtual color illuminate(const glm::vec3 & eyePosition, const glm::vec3 & worldPosition, glm::vec3 worldNormal, color material)
	{

		return LightSource::illuminate(eyePosition, worldPosition, worldNormal, material);

	}

	/**
	* Unit vector that points in the direction that is opposite 
	* the direction in which the light is shining.
	*/
	glm::vec3 direction; 
};


/**
* Struct for simulating light sources that have an explicit position and
* shine in a specified direction.Width of the associated beam of light is
* controlled using a spot cutoff cosine. Instantiations of the struct will 
* have position and direction properties along with
* a color and intensity of the light given off by the light source.
*/
struct SpotLight : public PositionalLight
{
	SpotLight(glm::vec3 position, glm::vec3 direction, float cutOffCosineRadians, const color & colorOfLight)
	: PositionalLight(position, colorOfLight), direction(glm::normalize(direction)), cutOffCosineRadians(cutOffCosineRadians)
	{}

	virtual color illuminate(const glm::vec3 & eyePosition, const glm::vec3 & worldPosition, glm::vec3 worldNormal, color material)
	{
		return LightSource::illuminate(eyePosition, worldPosition, worldNormal, material);
	}

	/**
	* Unit vector that points in the direction in which the light is shining.
	*/
	glm::vec3 direction;

	/**
	* Angle in radians of half the spot light beam
	*/
	float cutOffCosineRadians;

};