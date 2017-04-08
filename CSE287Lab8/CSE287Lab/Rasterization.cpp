#include "Rasterization.h"

// ColorBuffer object that is global to all files
extern FrameBuffer frameBuffer;

// Swaps two vertices
void swap(VertexData &v0, VertexData &v1);

// Rasterizes a vertical line
void drawVerticalLine(VertexData v0, VertexData v1);

// Rasterizes a horizontal line
void drawHorizontalLine(VertexData v0, VertexData v1);

// Uses the midpoint algorithm to render a sloped line
void midPointLine(VertexData v0, VertexData v1);

float cheapNonPerspectiveCorrectInterpolationForLines(const glm::vec2 start, const glm::vec2 end, const glm::vec2 & online)
{
	return glm::length(online - start) / glm::length(end-start);
}



inline void swap(VertexData &v0, VertexData &v1)
{
	VertexData temp = v0;
	v0 = v1;
	v1 = temp;

} // end swap


void drawVerticalLine(VertexData v0, VertexData v1)
{
	if (v1.position.y < v0.position.y) {

		swap(v0, v1);
	}

	float zDifference = v1.position.z - v0.position.z;

	for (float y = v0.position.y; y < v1.position.y; y++) {

		// Interpolate vertex attributes
		float weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, glm::vec2(v0.position.x, y));

		Fragment fragment;

		// Interpolate vertex attributes using alpha, beta, and gama weights
		fragment.material = (1.0f - weight) * v0.material + weight * v1.material;
		float z = (1 - weight)  * v0.position.z + weight * v1.position.z;
		fragment.worldNormal = (1.0f - weight) * v0.normal + weight * v1.normal;
		fragment.worldPosition = (1.0f - weight) * v0.worldPosition + weight * v1.worldPosition;
		fragment.windowPosition = glm::vec3(v0.position.x, y, z);

		FragmentOperations::processFragment(fragment);
	}

} // end drawVerticalLine


void drawHorizontalLine(VertexData v0, VertexData v1)
{
	if (v1.position.x < v0.position.x) {

		swap(v0, v1);
	}

	for (float x = v0.position.x; x < v1.position.x; x++) {

		// Interpolate vertex attributes
		float weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, glm::vec2(x, v0.position.y));

		Fragment fragment;

		// Interpolate vertex attributes using alpha, beta, and gama weights
		fragment.material = (1.0f - weight) * v0.material + weight * v1.material;
		float z = (1 - weight)  * v0.position.z + weight * v1.position.z;
		fragment.worldNormal = (1.0f - weight) * v0.normal + weight * v1.normal;
		fragment.worldPosition = (1.0f - weight) * v0.worldPosition + weight * v1.worldPosition;
		fragment.windowPosition = glm::vec3(x, v1.position.y, z);

		FragmentOperations::processFragment(fragment);
	}

} // end drawVerticalLine


void midPointLine(VertexData v0, VertexData v1)
{
	// Check if v0 is left of v1
	if (v1.position.x < v0.position.x) {

		// Make v0 left of v1
		swap(v0, v1);
	}

	// Calculate slope of the line
	float m = (v1.position.y - v0.position.y) / (v1.position.x - v0.position.x);

	if (m > 0 && m < 1.0f) { // For slope in (0,1] More "run" than "rise"

		float y = v0.position.y;

		for (float x = v0.position.x; x < v1.position.x; x += 1.0f) {

			// Interpolate vertex attributes
			float weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, glm::vec2(x, y));

			Fragment fragment;

			// Interpolate vertex attributes using alpha, beta, and gama weights
			fragment.material = (1.0f - weight) * v0.material + weight * v1.material;
			float z = (1 - weight)  * v0.position.z + weight * v1.position.z;
			fragment.worldNormal = (1.0f - weight) * v0.normal + weight * v1.normal;
			fragment.worldPosition = (1.0f - weight) * v0.worldPosition + weight * v1.worldPosition;
			fragment.windowPosition = glm::vec3(x, y, z);

			FragmentOperations::processFragment(fragment);

			// Evaluate the implicit equation for the line to determine if
			// the line will be above the midpoint between the pixel centers.
			float fXY = (v0.position.y - v1.position.y) * (x + 1.0f) +
				(v1.position.x - v0.position.x) * (y + 0.5f) +
				v0.position.x * v1.position.y - v1.position.x * v0.position.y;

			if (fXY < 0) {
				y += 1.0f;
			}
		}
	}
	else if (m > 1) { // For slope in (1,infinity] More "run" than "rise"

		float x = v0.position.x;

		for (float y = v0.position.y; y < v1.position.y; y += 1.0f) {

			// Interpolate vertex attributes
			float weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, glm::vec2(x, y));

			Fragment fragment;

			// Interpolate vertex attributes using alpha, beta, and gama weights
			fragment.material = (1.0f - weight) * v0.material + weight * v1.material;
			float z = (1 - weight)  * v0.position.z + weight * v1.position.z;
			fragment.worldNormal = (1.0f - weight) * v0.normal + weight * v1.normal;
			fragment.worldPosition = (1.0f - weight) * v0.worldPosition + weight * v1.worldPosition;
			fragment.windowPosition = glm::vec3(x, y, z);

			FragmentOperations::processFragment(fragment);
			
			// Evaluate the implicit equation for the line to determine if
			// the line will be left or right the midpoint between the pixel centers.
			float fXY = (v0.position.y - v1.position.y) * (x + 0.5f) +
				(v1.position.x - v0.position.x) * (y + 1.0f) +
				v0.position.x * v1.position.y - v1.position.x * v0.position.y;

			if (fXY > 0) {
				x += 1.0f;
			}
		}
	}
	else if (m >= -1.0f && m < 0) { // For slope in [-1,0) More "run" than "rise"

		float y = v0.position.y;
		float x = v0.position.x;

		for (float x = v0.position.x; x < v1.position.x; x += 1.0f) {

			// Interpolate vertex attributes
			float weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, glm::vec2(x, y));

			Fragment fragment;

			// Interpolate vertex attributes using alpha, beta, and gama weights
			fragment.material = (1.0f - weight) * v0.material + weight * v1.material;
			float z = (1 - weight)  * v0.position.z + weight * v1.position.z;
			fragment.worldNormal = (1.0f - weight) * v0.normal + weight * v1.normal;
			fragment.worldPosition = (1.0f - weight) * v0.worldPosition + weight * v1.worldPosition;
			fragment.windowPosition = glm::vec3(x, y, z);

			FragmentOperations::processFragment(fragment);

			// Evaluate the implicit equation for the line to determine if
			// the line will be below the midpoint between the pixel centers.
			float fXY = (v0.position.y - v1.position.y) * (x + 1.0f) +
				(v1.position.x - v0.position.x) * (y - 0.5f) +
				v0.position.x * v1.position.y - v1.position.x * v0.position.y;

			if (fXY > 0) {

				y -= 1.0f;
			}
		}
	}
	else if (m < -1) { // For slope in [-infinity,-1) More "run" than "rise"

		float x = v0.position.x;

		for (float y = v0.position.y; y > v1.position.y; y -= 1.0f) {

			// Interpolate vertex attributes
			float weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, glm::vec2(x, y));

			Fragment fragment;

			// Interpolate vertex attributes using alpha, beta, and gama weights
			fragment.material = (1.0f - weight) * v0.material + weight * v1.material;
			float z = (1 - weight)  * v0.position.z + weight * v1.position.z;
			fragment.worldNormal = (1.0f - weight) * v0.normal + weight * v1.normal;
			fragment.worldPosition = (1.0f - weight) * v0.worldPosition + weight * v1.worldPosition;
			fragment.windowPosition = glm::vec3(x, y, z);

			FragmentOperations::processFragment(fragment);

			// Evaluate the implicit equation for the line to determine if
			// the line will be left or right the midpoint between the pixel centers.
			float fXY = (v0.position.y - v1.position.y) * (x + 0.5f) +
				(v1.position.x - v0.position.x) * (y - 1.0f) +
				v0.position.x * v1.position.y - v1.position.x * v0.position.y;

			if (fXY < 0) {
				x += 1.0f;
			}
		}
	}

} // end midPointLine


void drawLine(VertexData v0, VertexData v1)
{
	if (v0.position.x == v1.position.x) {
		drawVerticalLine(v0, v1);
	}
	else if (v0.position.y == v1.position.y) {
		drawHorizontalLine(v0, v1);
	}
	else {
		midPointLine(v0, v1);
	}

} // end drawLine


void drawManyLines(const std::vector<VertexData> & vertices)
{
	for (unsigned int i = 0; (i + 1) < vertices.size(); i += 2) {

		drawLine(vertices[i], vertices[i + 1]);
	}

} // end drawManyLines


void drawWireFrameTriangle(const VertexData & v0, const VertexData & v1, const VertexData & v2)
{
	drawLine(v0, v1);
	drawLine(v1, v2);
	drawLine(v2, v0);

} // end drawWireFrameTriangle


void drawManyWireFrameTriangles(const std::vector<VertexData> & vertices)
{
	for (unsigned int i = 0; (i + 2) < vertices.size(); i += 3) {

		drawWireFrameTriangle(vertices[i], vertices[i + 1], vertices[i + 2]);
	}

} // end drawManyWireFrameTriangles


  // Implicit equation for the line between v0 and v1
inline float f01(const VertexData & v0, const VertexData & v1, const VertexData & v2, float x, float y)
{
	return (v0.position.y - v1.position.y) * x + (v1.position.x - v0.position.x) * y + 
		   (v0.position.x * v1.position.y) - (v1.position.x * v0.position.y);

} // end f01


  // Implicit equation for the line between v1 and v2
inline float f12(const VertexData & v0, const VertexData & v1, const VertexData & v2, float x, float y)
{
	return (v1.position.y - v2.position.y) * x + (v2.position.x - v1.position.x) * y + 
		   (v1.position.x * v2.position.y) - (v2.position.x * v1.position.y);

} // end f12


  // Implicit equation for the line between v2 and v0
inline float f20(const VertexData & v0, const VertexData & v1, const VertexData & v2, float x, float y)
{
	return (v2.position.y - v0.position.y) * x + (v0.position.x - v2.position.x) * y + 
		   (v2.position.x * v0.position.y) - (v0.position.x * v2.position.y);

} // end f20


void drawFilledTriangle(const VertexData & v0, const VertexData & v1, const VertexData & v2)
{
	// Find minimimum and maximum x and y limits for the triangle
	float xMin = glm::floor(glm::min(glm::min(v0.position.x, v1.position.x), v2.position.x));
	float xMax = glm::ceil(glm::max(glm::max(v0.position.x, v1.position.x), v2.position.x));
	float yMin = glm::floor(glm::min(glm::min(v0.position.y, v1.position.y), v2.position.y));
	float yMax = glm::ceil(glm::max(glm::max(v0.position.y, v1.position.y), v2.position.y));

	float fAlpha = f12(v0, v1, v2, v0.position.x, v0.position.y);
	float fBeta = f20(v0, v1, v2, v1.position.x, v1.position.y);
	float fGama = f01(v0, v1, v2, v2.position.x, v2.position.y);

	// Check all the pixels in the rows between the minimum and maximum y
	for (float y = yMin; y <= yMax; y++) {

		// Check all the pixels in the columns between the minimum and maximum x
		for (float x = xMin; x <= xMax; x++) {

			// Calculate the weights for Gouraud inperpolation
			// If any weight is negative, the fragment is not in the triangle
			float alpha = f12(v0, v1, v2, x, y) / fAlpha;
			float beta = f20(v0, v1, v2, x, y) / fBeta;
			float gama = f01(v0, v1, v2, x, y) / fGama;

			// Determine if the pixel position is inside the triangle
			if (alpha >= 0 && beta >= 0 && gama >= 0) {

				if ((alpha > 0 || fAlpha * f12(v0, v1, v2, -1, -1) > 0) &&
					(beta > 0 || fBeta * f20(v0, v1, v2, -1, -1) > 0) &&
					(gama > 0 || fGama * f01(v0, v1, v2, -1, -1) > 0)) {

					Fragment fragment;

					// Interpolate vertex attributes using alpha, beta, and gama weights
					fragment.material = alpha * v0.material + beta * v1.material + gama * v2.material;

					float z = alpha * v0.position.z + beta * v1.position.z + gama * v2.position.z;
					fragment.windowPosition = glm::vec3(x, y, z);
					fragment.worldNormal = alpha * v0.normal+ beta * v1.normal + gama * v2.normal;
					fragment.worldPosition = alpha * v0.worldPosition + beta * v1.worldPosition + gama * v2.worldPosition;

					FragmentOperations::processFragment(fragment);
				}
			}
		}
	}
} // end drawFilledTriangle


void drawManyFilledTriangles(const std::vector<VertexData> & vertices)
{
	for (unsigned int i = 0; (i + 2) < vertices.size(); i += 3) {

		drawFilledTriangle(vertices[i], vertices[i + 1], vertices[i + 2]);
	}

} // end drawManyFilledTriangles