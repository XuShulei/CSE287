#pragma once
#include "QuadricSurface.h"
class CylinderX : public QuadricSurface
{
public:
	CylinderX(const glm::vec3 & position, const color & mat);
	~CylinderX();
};
