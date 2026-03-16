#ifndef _I_CAMERA_CONTROLLER_H_
#define _I_CAMERA_CONTROLLER_H_

#include "core/camera/icamera.h"

#include <glm/glm.hpp>

class ICameraController
{
public:
	virtual ~ICameraController() = default;
	virtual void PositionCamera(ICamera& camera, glm::vec3 lookAtPosition) = 0;
};

#endif