
#ifndef _CAMERA_CONTROLLER_H_
#define _CAMERA_CONTROLLE_H_

#include "core/camera/icamera.h"
#include "core/camera/icameracontroller.h"

class CameraController : public ICameraController
{
public:
	void PositionCamera(ICamera& camera, glm::vec3 lookAtPosition) override;
};

#endif
