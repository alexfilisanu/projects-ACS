#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/tema2_camera.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
         struct ViewportArea
         {
             ViewportArea() : x(0), y(0), width(1), height(1) {}
             ViewportArea(int x, int y, int width, int height)
                 : x(x), y(y), width(width), height(height) {}
             int x;
             int y;
             int width;
             int height;
         };

        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, const glm::vec3& carPosition);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderScene(float deltaTimeSeconds);

        float TriangleArea(glm::vec3 A, glm::vec3 B, glm::vec3 C);
        bool InsideTriangle(glm::vec3 P, glm::vec3 A, glm::vec3 B, glm::vec3 C);
        bool InsideRacetrack(glm::vec3 P);
        bool Intersect(glm::vec3 sphere, glm::vec3 other, float radius);
        float GetAngle(float factor);
        
     protected:
        implemented::CameraTema2 *camera, *cameraViewport;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        std::vector<glm::vec3> points, allPoints, inPoints, extPoints;
        float translateX, translateZ;
        int factor1, factor2;
        float rotate;

        bool projectionType;
        float left, right, bottom, top, zNear, zFar;
        bool checkCollision1, checkCollision2;
        ViewportArea miniViewportArea;
    };
}   // namespace m1