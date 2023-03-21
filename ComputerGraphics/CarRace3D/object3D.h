#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object3D
{
    Mesh* CreateRacetrack(const std::string& name, std::vector<glm::vec3> inPoints, std::vector<glm::vec3> extPoints);
}