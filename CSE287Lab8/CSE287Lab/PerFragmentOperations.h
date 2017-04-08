#pragma once

#include "BasicIncludesAndDefines.h"
#include "FrameBuffer.h"
#include "Lights.h"
#include "PerVertexOperations.h"

extern FrameBuffer frameBuffer;

extern std::vector<LightSource*> lights;

enum fogType { NO_FOG, LINEAR_FOG, EXPONENTIAL_FOG, EXPONENTIAL_2_FOG };


struct Fragment {

	glm::vec3 windowPosition;
	color material;
	glm::vec3 worldNormal;
	glm::vec3 worldPosition;

};

class FragmentOperations
{
	public:

		static fogType fogSetting;

		static bool perPixelLightingEnabled;
	
		static void processFragment(Fragment & fragment);

	protected:

		static void applyLighting(Fragment & fragment);

		static void applyFog(Fragment & fragment);


};// end FragmentOperations



