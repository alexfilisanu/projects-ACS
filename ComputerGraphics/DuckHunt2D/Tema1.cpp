#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>
#include <math.h>
#include <map>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2DTema1.h"


using namespace std;
using namespace m1;



/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{

    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // Salvez width si height ai scenei
    width = (float)resolution.x;
    height = (float)resolution.y;

    // Initializare translateX si translateY
    translateX = 0;
    translateY = 0;

    // Initializare scaleX si scaleY
    scaleX = 0;
    scaleY = 1;

    // Initializare wingRotationAngle si directionFlag
    wingRotationAngle = 0;
    directionFlag = 1;

    // Initializare scor
    score = 0;
    
    // Crearea tuturor Mesh-urilor pe care le voi folosi in acest joc
    // Rata este centratata in origine, fiind construita in jurul acesteia
    AddMeshToList(object2DTema1::CreateTriangle("triangleBody", glm::vec3(-65, -30, 0), glm::vec3(35, 0, 0),
        glm::vec3(-65, 30, 0), glm::vec3(0.39f, 0.14f, 0.05f), true));
    AddMeshToList(object2DTema1::CreateTriangle("triangleRightWing", glm::vec3(-20, 40, 0), glm::vec3(5, 3, 0),
        glm::vec3(35, 70, 0), glm::vec3(0.32f, 0.16f, 0.08f), true));
    AddMeshToList(object2DTema1::CreateTriangle("triangleLeftWing", glm::vec3(-20, -40, 0), glm::vec3(5, -7, 0),
        glm::vec3(25, -70, 0), glm::vec3(0.32f, 0.16f, 0.08f), true));
    AddMeshToList(object2DTema1::CreateCircle("circleHead", glm::vec3(35, 0, 0), 20, glm::vec3(0.29f, 0.32f, 0.12f)));
    AddMeshToList(object2DTema1::CreateTriangle("triangleBeak", glm::vec3(45, -5, 0), glm::vec3(65, 0, 0),
        glm::vec3(45, 5, 0), glm::vec3(1, 0.51f, 0), true));
    AddMeshToList(object2DTema1::CreateHeart("circleLife1", glm::vec3(1130, 690, 0), 20, glm::vec3(1, 0, 0)));
    AddMeshToList(object2DTema1::CreateHeart("circleLife2", glm::vec3(1180, 690, 0), 20, glm::vec3(1, 0, 0)));
    AddMeshToList(object2DTema1::CreateHeart("circleLife3", glm::vec3(1230, 690, 0), 20, glm::vec3(1, 0, 0)));
    AddMeshToList(object2DTema1::CreateBullet("rectangleBullet1", glm::vec3(1125, 635, 0), 10, 20, glm::vec3(0.83f, 0.68f, 0.21f)));
    AddMeshToList(object2DTema1::CreateBullet("rectangleBullet2", glm::vec3(1175, 635, 0), 10, 20, glm::vec3(0.83f, 0.68f, 0.21f)));
    AddMeshToList(object2DTema1::CreateBullet("rectangleBullet3", glm::vec3(1225, 635, 0), 10, 20, glm::vec3(0.83f, 0.68f, 0.21f)));
    AddMeshToList(object2DTema1::CreateRectangle("rectangleMaxScore", glm::vec3(1110, 610, 0), 150, 20, glm::vec3(1, 1, 1), false));
    AddMeshToList(object2DTema1::CreateRectangle("rectangleScore", glm::vec3(1110, 610, 0), 1, 20, glm::vec3(0, 0, 1), true));
    AddMeshToList(object2DTema1::CreateRectangle("rectangleGround", glm::vec3(0, 0, 0), width, 180, glm::vec3(0, 0.6f, 0.09f), true));

    // Pentru a verifica daca rata este impuscata creez un dreptunghi care sa incadreze perfect rata, acesta miscandu-se
    // in permanenta cu ea. Daca va exista un click in interiorul acestuia consider rata impuscata
    AddMeshToList(object2DTema1::CreateRectangle("rectangleShot", glm::vec3(-65, -70, 0), 130.f, 140.f, glm::vec3(0.45f, 0.84f, 1), false));
    
    // Jumatatile din dimeniunea ratei, atat pe oX, cat si pe oY
    cx = 65;
    cy = 70; 

    // Initializez restul variabilelor necesare pentru logistica jocului 
    allLives = 3;
    speedFactor = 4;
    duck_number = 0;
    round_number = 1;

    // Initializez variabilele din interiorul structurilor ducks cu valori aleatoare
    randDucks();
}


// Functia ce realizeaza aceasta initializare aleatoare
void Tema1::randDucks()
{
    float LO, HI, res;
    for (int i = 0; i < 5; i++) {
        ducks[i].clock = 0; 
        ducks[i].bullets = 3;
        LO = 300;
        HI = width - 300;
        res = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
        ducks[i].startX = res;
        ducks[i].startY = cy;

        // Unghiul de plecare va fi intre 15 si 75 sau intre 105 si 165 pentru a evita 
        // ca rata sa mearga pe directii apropiate de verticala / orizontala
        LO = M_PI / 12;
        HI = 11 * M_PI / 12;
        res = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
        while (res >= 5 * M_PI / 12 && res <= 7 * M_PI / 12) {
            res = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
        }
        ducks[i].orientationAngle = res;

        ducks[i].alive = true;
        ducks[i].shoted = false;
        ducks[i].escaped = false;
    }
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.45f, 0.84f, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    // Prima data randez iarba, care este implementata ca o zona in care rata nu poate
    // fi vazuta sau impuscata
    RenderMesh2D(meshes["rectangleGround"], shaders["VertexColor"], glm::mat3(1));

    RenderMesh2D(meshes["rectangleMaxScore"], shaders["VertexColor"], glm::mat3(1));

    // Daca scorul jucatorului depaseste valoarea maxima a dreptunghiului ce retine 
    // scorul, atunci scorul o sa creasca doar in valoare, nu si in afisaj
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(1110, 610);
    if (score <= 150) {
        scaleX = score;
    }
    modelMatrix *= transform2D::Scale(scaleX, scaleY);
    modelMatrix *= transform2D::Translate(-1110, -610);
    RenderMesh2D(meshes["rectangleScore"], shaders["VertexColor"], modelMatrix);   
            
    // Afisez numarul de vieti ramase
    if (allLives >= 1) {
        RenderMesh2D(meshes["circleLife1"], shaders["VertexColor"], glm::mat3(1));
    }
    if (allLives >= 2) {
        RenderMesh2D(meshes["circleLife2"], shaders["VertexColor"], glm::mat3(1));
    }
    if (allLives >= 3) {
        RenderMesh2D(meshes["circleLife3"], shaders["VertexColor"], glm::mat3(1));
    }

    // Afisez numarul de gloante ramase
    if (ducks[duck_number].bullets >= 1) {
        RenderMesh2D(meshes["rectangleBullet1"], shaders["VertexColor"], glm::mat3(1));
    }
    if (ducks[duck_number].bullets >= 2) {
        RenderMesh2D(meshes["rectangleBullet2"], shaders["VertexColor"], glm::mat3(1));
    }
    if (ducks[duck_number].bullets >= 3) {
        RenderMesh2D(meshes["rectangleBullet3"], shaders["VertexColor"], glm::mat3(1));
    }

    // In cazul in care jucatorul ramane fara vieti jocul se termina
    if (allLives == 0) {
        cout << "#### SCORE: " << score << " ####";
        exit(0);
    }
    else {
        // Daca numarul ratelor a ajuns la 5, initializez iar ratele cu valori aleatoare
        // si incepe o noua runda in care ratele sunt mai rapide
        if (duck_number == 5) {
            randDucks();
            duck_number = 0;
            round_number++;
            speedFactor += 2;
        }
        else {
            // Daca rata e in modul normal de zbor
            if (ducks[duck_number].alive) {
                // Daca rata a zburat mai putin de 5 secunde si nu au fost conumate cele 3 gloante
                // verific daca se loveste de o margine a scenei, caz in care ricoseaza
                if (ducks[duck_number].clock <= 5 && ducks[duck_number].bullets != 0) {
                    // Marginea de SUS
                    if (translateY + ducks[duck_number].startY > height) {
                        ducks[duck_number].orientationAngle = 2 * M_PI - ducks[duck_number].orientationAngle;
                        translateY -= 5;
                    }
                    // Marginea de JOS
                    if (translateY + ducks[duck_number].startY < 0) {
                        ducks[duck_number].orientationAngle = 2 * M_PI - ducks[duck_number].orientationAngle;
                        translateY += 5;
                    }
                    // Marginea din DREAPTA
                    if (translateX + ducks[duck_number].startX > width) {
                        if (ducks[duck_number].orientationAngle >= M_PI) {
                            ducks[duck_number].orientationAngle = 3 * M_PI - ducks[duck_number].orientationAngle;
                        }
                        else {
                            ducks[duck_number].orientationAngle = M_PI - ducks[duck_number].orientationAngle;
                        }
                        translateX -= 5;
                    }
                    // Marginea din STANGA
                    if (translateX + ducks[duck_number].startX < 0) {
                        if (ducks[duck_number].orientationAngle >= M_PI) {
                            ducks[duck_number].orientationAngle = 3 * M_PI - ducks[duck_number].orientationAngle;
                        }
                        else {
                            ducks[duck_number].orientationAngle = M_PI - ducks[duck_number].orientationAngle;
                        }
                        translateX += 5;
                    }
                    // Scad 2Pi(perioada) din unghi in cazul in care acesta este mai mare
                    if (ducks[duck_number].orientationAngle > 2 * M_PI) {
                        ducks[duck_number].orientationAngle -= 2 * M_PI;
                    }

                    // Ii calculez ratei noua sa pozitie 
                    Tema1::updateDuckPosition(ducks[duck_number].startX, ducks[duck_number].startY, deltaTimeSeconds,
                        ducks[duck_number].orientationAngle);
                    ducks[duck_number].clock += deltaTimeSeconds;
                }
                // Daca au trecut mai mult de 5 secunde sau daca s-au consumat toate cele 3 gloante,
                // rata este considerata evadata
                else {
                    ducks[duck_number].alive = false;
                    ducks[duck_number].escaped = true;
                }
            }
            else {
                // Daca rata a evadat aceasta are o noua animatie, zburand pe plan vertical in sus, 
                // pana cand paraseste scena. Cand scena a fost parasia, se scade o viata jucatorului
                // si se trece la urmatoarea rata
                if (ducks[duck_number].escaped) {
                    if (ducks[duck_number].startY + translateY - cx > height) {
                        translateX = 0;
                        translateY = 0;
                        allLives--;
                        duck_number++;
                    }
                    else {
                        Tema1::updateDuckPosition(ducks[duck_number].startX, ducks[duck_number].startY, deltaTimeSeconds, M_PI_2);
                    }
                }
                // Daca rata a fost impuscata aceasta are o noua animatie, zburand pe plan verical in jos
                // pana cand paraseste scena. Cand scena a fost parasita, se incrementeaza scorul si se 
                // trece la urmatoarea rata 
                if (ducks[duck_number].shoted) {
                    if (ducks[duck_number].startY + translateY + cx < 0) {
                        translateX = 0;
                        translateY = 0;
                        duck_number++;
                        score += round_number;
               
                    }
                    else {
                        Tema1::updateDuckDeath(ducks[duck_number].startX, ducks[duck_number].startY, deltaTimeSeconds);
                    }
                }
            }
        }
    }
}


// Calculez matricea de roatie a ratei pentru un unghi dat (pentru ca aceasta este construita 
// centrat nu este nevoie de translatii inainte si dupa operatia de rotire)
glm::mat3 Tema1::rotateDuck(float rotateAngle)
{
    glm::mat3 rotateDuckMat = glm::mat3(1);
    rotateDuckMat *= transform2D::Rotate(rotateAngle);
    
    return rotateDuckMat;
}


// Functie pentru zborul normal al ratei -> apeleaza functia anterioara pentru matricea de rotatie
//      -> deplasarea se realizeaza pe directia oX cu cos(unghi) si pe directia oY cu sin(unghi) 
void Tema1::updateDuckPosition(float startX, float startY, float deltaTimeSeconds, float rotateAngle)
{
    glm::mat3 rotateDuckMat = Tema1::rotateDuck(rotateAngle);

    // Miscarea capului ratei
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(startX, startY);
    translateX += deltaTimeSeconds * cos(rotateAngle) * speedFactor * 10;
    translateY += deltaTimeSeconds * sin(rotateAngle) * speedFactor * 10;
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= rotateDuckMat;
    RenderMesh2D(meshes["circleHead"], shaders["VertexColor"], modelMatrix);

    // Miscarea corpului ratei 
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(startX, startY);
    translateX += deltaTimeSeconds * cos(rotateAngle) * speedFactor * 10;
    translateY += deltaTimeSeconds * sin(rotateAngle) * speedFactor * 10;
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= rotateDuckMat;
    RenderMesh2D(meshes["triangleBody"], shaders["VertexColor"], modelMatrix);

    // Miscarea aripii drepte
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(startX, startY);
    translateX += deltaTimeSeconds * cos(rotateAngle) * speedFactor * 10;
    translateY += deltaTimeSeconds * sin(rotateAngle) * speedFactor * 10;
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= rotateDuckMat;
    // Pentru realizarea miscarii de rotatie a unei aripi o translatez in punctul in 
    // jurul caruia vreau sa se roteasca si ii impun un prag minim si unul maxim intre
    // care sa oscileze - viteza aripilor e direct proportionala cu viteza ratei
    modelMatrix *= transform2D::Translate(5, -3);
    if (wingRotationAngle >= 0.7f) {
        directionFlag = -1;
    }
    else if (wingRotationAngle <= 0.0f) {
        directionFlag = 1;
    }
    wingRotationAngle += deltaTimeSeconds * speedFactor * 0.1f * directionFlag;
    modelMatrix *= transform2D::Rotate(wingRotationAngle);
    modelMatrix *= transform2D::Translate(-5, 3);
    RenderMesh2D(meshes["triangleRightWing"], shaders["VertexColor"], modelMatrix);

    // Miscarea aripii stangi - similar cu miscarea celei drepte, doar ca unghiurile
    // sunt opuse
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(startX, startY);
    translateX += deltaTimeSeconds * cos(rotateAngle) * speedFactor * 10;
    translateY += deltaTimeSeconds * sin(rotateAngle) * speedFactor * 10;
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= rotateDuckMat;
    modelMatrix *= transform2D::Translate(5, -7);
    if (wingRotationAngle >= 0.7f) {
        directionFlag = -1;
    }
    else if (wingRotationAngle <= 0.0f) {
        directionFlag = 1;
    }
    wingRotationAngle += deltaTimeSeconds * speedFactor * 0.1f * directionFlag;
    modelMatrix *= transform2D::Rotate(-wingRotationAngle);
    modelMatrix *= transform2D::Translate(-5, 7);
    RenderMesh2D(meshes["triangleLeftWing"], shaders["VertexColor"], modelMatrix);

    // Miscarea ciocului
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(startX, startY);
    translateX += deltaTimeSeconds * cos(rotateAngle) * speedFactor * 10;
    translateY += deltaTimeSeconds * sin(rotateAngle) * speedFactor * 10;
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= rotateDuckMat;
    RenderMesh2D(meshes["triangleBeak"], shaders["VertexColor"], modelMatrix);

    // Miscarea dreptunghiului pentru verificarea impuscaturilor si calculul coordonatelor
    // colturilor acestuia
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(startX, startY);
    translateX += deltaTimeSeconds * cos(rotateAngle) * speedFactor * 10;
    translateY += deltaTimeSeconds * sin(rotateAngle) * speedFactor * 10;
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= rotateDuckMat;
    rectangleVertex1 = { startX + translateX - cx, startY + translateY - cy, 0 };
    rectangleVertex2 = { startX + translateX + cx, startY + translateY - cy, 0 };
    rectangleVertex3 = { startX + translateX - cx, startY + translateY + cy, 0 };
    rectangleVertex4 = { startX + translateX + cx, startY + translateY + cy, 0 };
    RenderMesh2D(meshes["rectangleShot"], shaders["VertexColor"], modelMatrix);
}


// Functie pentru zborul ratei dupa ce a murit - similiar cu cel al ratei care zboara normal, 
// doar ca dirctia de deplasare este verticala in jos, iar miscarea aripilor nu se mai realizeaza.
// Nu mai este nevoie nici de rectangleShot, nci de translatie pe Ox, nici de valori trigonometrice.
// Viteza de cadere este constanta indiferent de viteza ratei
void Tema1::updateDuckDeath(float startX, float startY, float deltaTimeSeconds)
{
    glm::mat3 rotateDuckMat = Tema1::rotateDuck(3 * M_PI_2);

    // Miscarea capului
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(startX, startY);
    translateY -= deltaTimeSeconds * 100;
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= rotateDuckMat;
    RenderMesh2D(meshes["circleHead"], shaders["VertexColor"], modelMatrix);

    // Miscarea corpului
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(startX, startY);
    translateY -= deltaTimeSeconds * 100;
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= rotateDuckMat;
    RenderMesh2D(meshes["triangleBody"], shaders["VertexColor"], modelMatrix);

    // Miscarea aripii drepte
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(startX, startY);
    translateY -= deltaTimeSeconds * 100;
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= rotateDuckMat;
    RenderMesh2D(meshes["triangleRightWing"], shaders["VertexColor"], modelMatrix);

    // Miscarea aripii stangi
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(startX, startY);
    translateY -= deltaTimeSeconds * 100;
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= rotateDuckMat;
    RenderMesh2D(meshes["triangleLeftWing"], shaders["VertexColor"], modelMatrix);

    // Miscarea ciocului
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(startX, startY);
    translateY -= deltaTimeSeconds * 100;
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= rotateDuckMat;
    RenderMesh2D(meshes["triangleBeak"], shaders["VertexColor"], modelMatrix);
}


void Tema1::FrameEnd()
{
}


/*
*  These are callback functions. To find more about callbacks and
*  how they behave, see `input_controller.h`.
*/


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    // Calculez noile dimensiuni pe oX si oY ale scenei si realizez o scalare
    // pentru ca indiferent de dimensiunea scenei, calculele urmatoare sa fie corecte
    // (in acest mod, si in full screen jocul se comporta normal)
    glm::ivec2 resolution = window->GetResolution();
    mouseX *= width / resolution.x;
    mouseY *= height / resolution.y;
    mouseY = height - mouseY;

    // Dac rata nu a fost impuscata si nici nu a evadat iau in calcul posibilitatea
    // de a o putea impusca. In celelalte cazuri nu are sens impuscare
    if (!ducks[duck_number].shoted && !ducks[duck_number].escaped) {
        // Daca mai am gloante si daca rata nu se afla in zona sigura(loc in care
        // nu poate fi vazuta sau impuscata) verific daca rata a fost nimerita.
        // Pentru a face acest lucru calculez ariile triunghiurilor formate din cate
        // 2 colturi ale dreptunghiului si punctul unde a fost facut click ul. Daca 
        // suma acestora e mai mare decat aria dreptunghiului, atunci rata nu a fost
        // impuscata
        if (ducks[duck_number].bullets > 0 && mouseY > 180) {
            float areaRectangle = 140.f * 130.f;
            float area1 = abs(rectangleVertex1.x * (rectangleVertex2.y - mouseY) + rectangleVertex2.x * (mouseY - rectangleVertex1.y) +
                mouseX * (rectangleVertex1.y - rectangleVertex2.y)) / 2;
            float area2 = abs(rectangleVertex2.x * (rectangleVertex3.y - mouseY) + rectangleVertex3.x * (mouseY - rectangleVertex2.y) +
                mouseX * (rectangleVertex2.y - rectangleVertex3.y)) / 2;
            float area3 = abs(rectangleVertex3.x * (rectangleVertex4.y - mouseY) + rectangleVertex4.x * (mouseY - rectangleVertex3.y) +
                mouseX * (rectangleVertex3.y - rectangleVertex4.y)) / 2;
            float area4 = abs(rectangleVertex4.x * (rectangleVertex1.y - mouseY) + rectangleVertex1.x * (mouseY - rectangleVertex4.y) +
                mouseX * (rectangleVertex4.y - rectangleVertex1.y)) / 2;

            if (areaRectangle >= area1 + area2 + area3 + area4) {
                ducks[duck_number].alive = false;
                ducks[duck_number].shoted = true;
            }
        }
        ducks[duck_number].bullets--;
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
