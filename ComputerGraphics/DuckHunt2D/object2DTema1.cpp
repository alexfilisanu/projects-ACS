#include "object2DTema1.h"

#include <vector>
#include <cmath>
#include <math.h>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2DTema1::CreateSquare(const std::string &name, glm::vec3 leftBottomCorner,
    float length, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* object2DTema1::CreateRectangle(const std::string &name, glm::vec3 leftBottomCorner,
    float length, float height, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}


Mesh* object2DTema1::CreateTriangle(const std::string &name, glm::vec3 firstCorner,
    glm::vec3 secondCorner, glm::vec3 thirdCorner, glm::vec3 color, bool fill)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(firstCorner, color),
        VertexFormat(secondCorner, color),
        VertexFormat(thirdCorner, color),
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(1);
        indices.push_back(0);
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}


Mesh* object2DTema1::CreateCircle(const std::string &name, glm::vec3 center,
    float radius, glm::vec3 color)
{

    std::vector<VertexFormat> vertices = { };

    for (int i = 0; i <= 360; i++) {
        vertices.push_back(VertexFormat(glm::vec3(center[0] + cos(i) * radius, 
            center[1] + sin(i) * radius, 0), color));
    }

    std::vector<unsigned int> indices = { };

    for (int i = 0; i < 360; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    Mesh* circle = new Mesh(name);
    
    circle->SetDrawMode(GL_LINE_LOOP);

    circle->InitFromData(vertices, indices);
    return circle;
}


Mesh* object2DTema1::CreateHeart(const std::string& name, glm::vec3 center,
    float radius, glm::vec3 color)
{
    std::vector<VertexFormat> vertices = { };

    for (float angle = 0; angle <= 2 * M_PI; angle += 0.01f) {
        vertices.push_back(VertexFormat(center, color));
        vertices.push_back(VertexFormat(glm::vec3
        (center.x + 19.0 * sin(angle) * sin(angle) * sin(angle),
            center.y + 16.0 * cos(angle) - 6.0 * cos(2.0 * angle) - 3.0 * cos(3.0 * angle) - cos(4.0 * angle),
            center.z), color));
    }

    std::vector<unsigned int> indices = { };
   
    for (int i = 0; i < vertices.size(); i++) {
        indices.push_back(i);
    }

    Mesh* heart = new Mesh(name);

    heart->SetDrawMode(GL_LINE_LOOP);

    heart->InitFromData(vertices, indices);

    return heart;
}


Mesh* object2DTema1::CreateBullet(const std::string& name, glm::vec3 leftBottomCorner,
    float length, float height, glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color),
        VertexFormat(corner + glm::vec3(length / 2, height + height / 2, 0), color)
    };

    Mesh* bullet = new Mesh(name);
    std::vector<unsigned int> indices = { 2, 4, 3,
                                          1, 2, 0,
                                          0, 2, 3};

    bullet->SetDrawMode(GL_TRIANGLES);

    bullet->InitFromData(vertices, indices);
    return bullet;
}

