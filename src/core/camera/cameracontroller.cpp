#include "core/camera/cameracontroller.h"

void CameraController::PositionCamera(ICamera& camera, glm::vec3 lookAtPosition)
{
    camera.setPosition(glm::vec3(
        lookAtPosition.x + 75.0f,
        100.0f,
        lookAtPosition.z + 75.0f
    ));
    camera.setOrientation(glm::normalize(lookAtPosition - camera.getPosition()));
}