#include "object3D.h"

#include <vector>
#include <cmath>
#include <math.h>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* object3D::CreateRacetrack(const std::string& name, std::vector<glm::vec3> inPoints, std::vector<glm::vec3> extPoints)
{
    std::vector<VertexFormat> vertices = { };

    for (int i = 0; i < extPoints.size(); i++) {
        vertices.push_back(VertexFormat(extPoints[i]));
        vertices.push_back(VertexFormat(inPoints[i]));
    }
    
    std::vector<unsigned int> indices = { };
    int i = 0;
    for (i; i < vertices.size() - 2; i++) {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }
    indices.push_back(i);
    indices.push_back(i + 1);
    indices.push_back(0);

    indices.push_back(i + 1);
    indices.push_back(0);
    indices.push_back(1);

    Mesh* racetrack = new Mesh(name);

    racetrack->InitFromData(vertices, indices);
    return racetrack;
}