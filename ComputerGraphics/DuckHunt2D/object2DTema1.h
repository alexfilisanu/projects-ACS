#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2DTema1
{
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, 
        glm::vec3 color, bool fill = false);   
    Mesh* CreateRectangle(const std::string &name, glm::vec3 leftBottomCorner, float length, 
        float height, glm::vec3 color, bool fill = false);
    Mesh* CreateTriangle(const std::string &name, glm::vec3 firstCorner, glm::vec3 secondCorner, 
        glm::vec3 thirdCorner, glm::vec3 color, bool fill = false);
    Mesh* CreateCircle(const std::string &name, glm::vec3 center, float radius, glm::vec3 color);
    Mesh* CreateHeart(const std::string& name, glm::vec3 center, float radius, glm::vec3 color);
    Mesh* CreateBullet(const std::string& name, glm::vec3 leftBottomCorner, float length,
        float height, glm::vec3 color); 
}
