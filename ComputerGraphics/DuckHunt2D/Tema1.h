#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;


    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;  
        
        glm::mat3 Tema1::rotateDuck(float rotateAngle);
        void updateDuckPosition(float startX, float startY, float deltaTimeSeconds, float rotateAngle);
        void updateDuckDeath(float startX, float startY, float deltaTimeSeconds);
        void randDucks();
        
    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float wingRotationAngle;
        float directionFlag;
        float width, height;
        glm::vec3 rectangleVertex1, rectangleVertex2, rectangleVertex3, rectangleVertex4;
        float score;
        int allLives;
        int speedFactor;
        int duck_number;
        int round_number;
        
        struct duck_struct {
            float clock;
            int bullets;
            float startX;
            float startY;
            float orientationAngle;
            bool alive;
            bool shoted;
            bool escaped;
        } ducks[5];
    };
}   // namespace m1
