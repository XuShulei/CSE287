#pragma once

#include "BasicIncludesAndDefines.h"
#include "Lights.h"
#include "Rasterization.h"

extern std::vector<LightSource*> lights;

enum Render_Mode { FILL, LINE };

class PerVertex {

public:

	// Pipeline transformation matrices
	static glm::mat4 modelingTransformation;
	static  glm::mat4 viewingTransformation;
	static glm::mat4 projectionTransformation;
	static glm::mat4 viewportTransformation;
	static  glm::vec3 eyePositionInWorldCoords;

	// View port limits
	static float xViewportMin, yViewportMin, xViewportMax, yViewportMax;

	// Normalized device coordinate horizontal and vertical limits
	static const int xNdcMin, yNdcMin, xNdcMax, yNdcMax;

	// Planes describing the normalized device coordinates view volume
	static std::vector<Plane> ndcPlanes;

	static bool perVertexLightingEnabled;

	static Render_Mode polygonRenderMode;

	static void processTriangleVertices(const std::vector<VertexData> & objectCoords);

	static void processLineSegments(const std::vector<VertexData> & objectCoords);

protected:

	static std::vector<VertexData> clipAgainstPlane(std::vector<VertexData> & verts, Plane & plane);

	static std::vector<VertexData> triangulate(const std::vector<VertexData> & poly);

	static std::vector<VertexData> clipPolygon(const std::vector<VertexData> & clipCoords);

	static std::vector<VertexData> clipLineSegments(const std::vector<VertexData> & clipCoords);

	static std::vector<VertexData> removeBackwardFacingTriangles(const std::vector<VertexData> & triangleVerts);

	static std::vector<VertexData> transformVerticesToWorldCoordinates(const glm::mat4 & modelMatrix, const std::vector<VertexData> & vertices);

	static void applyLighting(std::vector<VertexData> & worldCoords);

	static std::vector<VertexData> transformVertices(const glm::mat4 & transMatrix, const std::vector<VertexData> & vertices);

}; // end PerVertex class



