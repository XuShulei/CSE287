#include "RayTracer.h"


RayTracer::RayTracer(FrameBuffer & cBuffer, color defaultColor )
:colorBuffer(cBuffer), defaultColor(defaultColor), recursionDepth(2)
{
	
}


RayTracer::~RayTracer(void)
{
}

void RayTracer::setCameraFrame(const glm::vec3 & viewPosition, const glm::vec3 & viewingDirection, glm::vec3 up)
{
	// Calculate and set the the w, u, and vdata members and 
	// set the eye data member to the viewing position.
	// TODO
	eye = viewPosition;
	w = -glm::normalize(viewingDirection);
	u = glm::normalize(glm::cross(up, w));
	v = glm::normalize(glm::cross(w, u));

} // end setCameraFrame


void RayTracer::calculatePerspectiveViewingParameters(const float verticalFieldOfViewDegrees)
{
	// Set the set the topLimit, bottomLimit, rightLimit, 
	// leftLimit, distToPlane, nx, and ny data members
	// TODO
	distToPlane = 1.0f / glm::tan(verticalFieldOfViewDegrees / 360 * M_PI);
	topLimit = 1;
	
	nx = (float)colorBuffer.getWindowWidth();
	ny = (float)colorBuffer.getWindowHeight();
	rightLimit = topLimit * nx / ny;

	leftLimit = -rightLimit;
	bottomLimit = -topLimit;

} // end calculatePerspectiveViewingParameters


void RayTracer::calculateOrthographicViewingParameters(const float viewPlaneHeight)
{
	topLimit = fabs(viewPlaneHeight) / 2.0f;

	rightLimit = topLimit * ((float)colorBuffer.getWindowWidth()/colorBuffer.getWindowHeight()); // Set r based on aspect ratio and height of plane

	// Make view plane symetrical about the viewing direction
	leftLimit = -rightLimit; 
	bottomLimit = -topLimit;

	// Calculate the distance between pixels in the horizontal and vertical directions
	nx = (float)colorBuffer.getWindowWidth();
	ny = (float)colorBuffer.getWindowHeight();

	distToPlane = 0.0; // Rays start on the view plane
	
} // end calculateOrthographicViewingParameters


glm::vec2 RayTracer::getProjectionPlaneCoordinates(const int x, const int y)
{
	// Page 75
	// Calculate and return the u and v data members as the x an y coordinates
	// of the 
	glm::vec2 uvScalarValues;

	// TODO
	float u = leftLimit + (rightLimit - leftLimit) * (x + 0.5) / nx;
	float v = bottomLimit + (topLimit - bottomLimit) * (y + 0.5) / ny;
	uvScalarValues.x = u;
	uvScalarValues.y = v;

	return uvScalarValues;
}


void RayTracer::setOrthoRayOriginAndDirection(const int x, const int y)
{
	glm::vec2 uv = getProjectionPlaneCoordinates(x, y);

	// Page 75
	rayDirection = glm::normalize(-w);
	rayOrigin = eye + uv.x * u + uv.y * v;

} // end setOrthoOriginAndDirection


void RayTracer::setPerspectiveRayOriginAndDirection(const int x, const int y)
{

	// TODO
	glm::vec2 uv = getProjectionPlaneCoordinates(x, y);
	rayOrigin = eye;
	rayDirection = glm::normalize(-distToPlane*w + uv.x * u + uv.y * v);
} // end setPerspectiveOriginAndDirection





void RayTracer::raytraceScene(const std::vector<std::shared_ptr<Surface>> & surfaces, const std::vector<std::shared_ptr<LightSource>> & lights)
{
	this->surfacesInScene = surfaces;
	this->lightsInScene = lights;

	// Iterate through each and every pixel in the rendering window
	// TODO
	for (unsigned int i = 0; i < colorBuffer.getWindowWidth(); i++) {
		for (unsigned int j = 0; j < colorBuffer.getWindowHeight(); j++) {
			setPerspectiveRayOriginAndDirection(i, j);
			color cl = traceIndividualRay(rayOrigin, rayDirection,recursionDepth);
			colorBuffer.setPixel(i, j, cl);
		}
	}

} // end raytraceScene



color RayTracer::traceIndividualRay(const glm::vec3 &e, const glm::vec3 &d, int recursionLevel)
{
	// Find surface intersection that is closest to 'e' in the direction 'd.'
	// TODO
	HitRecord hit = findIntersection(e, d, surfacesInScene);

	color localLight = color(0.0f, 0.0f, 0.0f, 1.0f);
	
	if (recursionLevel > 0) {
		if (hit.t != FLT_MAX) {
			for (unsigned int i = 0; i < lightsInScene.size(); i++) {
				localLight += lightsInScene[i]->illuminate(-d, hit, surfacesInScene);
			}

			glm::vec3 rd = glm::reflect(d, hit.surfaceNormal);// glm::normalize(d - 2 * glm::dot(d, hit.surfaceNormal) * hit.surfaceNormal);
			HitRecord hit1 = findIntersection(hit.interceptPoint, rd, surfacesInScene);
			return 0.7f * localLight + 0.3f * traceIndividualRay(hit.interceptPoint + EPSILON * hit.surfaceNormal, rd, recursionLevel - 1);
		}
		return localLight;
	}
	else
		return localLight;
} // end traceRay


