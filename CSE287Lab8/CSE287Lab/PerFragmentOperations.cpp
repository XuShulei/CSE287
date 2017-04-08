#include "PerFragmentOperations.h"

bool FragmentOperations::perPixelLightingEnabled = false;

fogType FragmentOperations::fogSetting = NO_FOG;


void FragmentOperations::applyLighting(Fragment & fragment)
{

	// TODO

} // end applyLighting



void FragmentOperations::applyFog(Fragment & fragment)
{
	// TODO

} // end applyFog


void FragmentOperations::processFragment(Fragment & fragment)
{
	// Set the color for the pixel 
	frameBuffer.setPixel((int)fragment.windowPosition.x, (int)fragment.windowPosition.y, fragment.material);

} // end processFragment