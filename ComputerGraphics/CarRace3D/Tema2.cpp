#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/object3D.h"
#include "lab_m1/Tema2/tema2_camera.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    left = -10.f;
    right = 10.f;
    bottom = -10.f;
    top = 10.f;
    zNear = 0.1f;
    zFar = 200.f;
    factor1 = 0.f;  // factor de viteza pentru primul obstacol dinamic
    factor2 = 0.f; // factor de viteza pentru al 2lea obstacol dinamic
    rotate = 0.f;  // unghiul de rotatie al camerei pentru a roti masina in mod corespunzator
    
    // camera principala third person -- orientata in spatele masinii
    camera = new implemented::CameraTema2();
    camera->Set(glm::vec3(21.66f, 0.f, 5.82f), glm::vec3(21.66f, 0.f, 4.82f), glm::vec3(0, 1, 0));
    camera->RotateThirdPerson_OX(-glm::radians(30.f));
    
    // camera pentru viewport -- orientata deasupra masinii
    cameraViewport = new implemented::CameraTema2();
    cameraViewport->Set(glm::vec3(camera->GetTargetPosition().x, camera->GetTargetPosition().y + 10.f, 
        camera->GetTargetPosition().z), camera->GetTargetPosition(), glm::vec3(0, 0, 1));

    // puncte definite cu Geogebra
    points = {
        glm::vec3(23.11, 0, 1.22),
        glm::vec3(19.66, 0, 7.82),
        glm::vec3(14.66, 0, 10.49),
        glm::vec3(9.06, 0, 7.38),
        glm::vec3(4.75, 0, 4.45),
        glm::vec3(-1.84, 0, 6.39),
        glm::vec3(-7.87, 0, 5.7),
        glm::vec3(-12.14, 0, 7.6),
        glm::vec3(-18.17, 0, 5.01),
        glm::vec3(-20.76, 0, -0.85),
        glm::vec3(-18.52, 0, -7.01),
        glm::vec3(-10.2, 0, -8.26),
        glm::vec3(-3.13, 0, -6.92),
        glm::vec3(4, 0, -10),
        glm::vec3(8.93, 0, -6.92),
        glm::vec3(15.61, 0, -4.9),
        glm::vec3(20.87, 0, -8.26),
        glm::vec3(24.7, 0, -3.52)
    };
    points.push_back(points[0]);

    // generez 100 de puncte intermediare intre 2 punte consecutive pentru a avea o densitate mare de triunghiuri
    allPoints = { };
    for (int i = 0; i < points.size() - 1; i++) {
        for (float j = 0; j < 1; j += 0.01) {
            allPoints.push_back(glm::vec3(points[i].x + (points[i + 1].x - points[i].x) * j, 0,
                points[i].z + (points[i + 1].z - points[i].z) * j));
        }
    }
    allPoints.push_back(points[18]);

    // generez punctele interioare si exterioare punctelor generate anterior cu ajutorul formulelor prezentate pe ocw
    int i = 0;
    for (i; i < allPoints.size() - 1; i++) {
        glm::vec3 D = glm::normalize(allPoints[i + 1] - allPoints[i]);
        glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
        glm::vec3 R = allPoints[i] + 1.5f * P;
        glm::vec3 A = allPoints[i] - 0.9f * P;
        inPoints.push_back(R);
        extPoints.push_back(A);
    }
    glm::vec3 D = glm::normalize(allPoints[0] - allPoints[i]);
    glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
    glm::vec3 R = allPoints[i] + 1.5f * P;
    glm::vec3 A = allPoints[i] - 0.9f * P;
    inPoints.push_back(R);
    extPoints.push_back(A);

    // folosesc mesh ul "plane50", deja existent, pentru plan
    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // folosesc mesh ul "box", deja existent, pentru masini, si copaci(scalari diferite)
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // adaug pista calculata in object3D in lista de mesh uri
    AddMeshToList(object3D::CreateRacetrack("racetrack", inPoints, extPoints));

    // folosesc shader ul din lab5(pentru culoare), la care adaug si efectul de curbura
    {
        Shader* shader = new Shader("Tema2Shader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
   
    // setez rezolutia viewport ului(lab4)
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(resolution.x * 4 / 5 - 50, 50, resolution.x / 5.f, resolution.y / 5.f);
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.45f, 0.84f, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderScene(float deltaTimeSeconds) 
{
    // randare plan
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2.f));
        RenderSimpleMesh(meshes["plane"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.2f, 0.8f, 0.2f), camera->GetTargetPosition());
    }

    // randare pista -- o translatez pe OY cu 0.01 pentru a fi deasupra planului
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.01f, 0.f));
        RenderSimpleMesh(meshes["racetrack"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.16f, 0.16f, 0.13f), camera->GetTargetPosition());
    }

    // randare copacei decorativi(un box scalat pentru trunchi si unul pentru coroana)
    {
        // randare copacei in interiorul pistei
        for (int i = 0; i < inPoints.size(); i += 99) {
            // calculul pozitiei pentru a nu fi pe pista
            glm::vec3 position = 2.f * inPoints[i] - allPoints[i];
            
            // randare trunchi
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, position);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.75f, 0.25f));
            RenderSimpleMesh(meshes["box"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.3f, 0.1f, 0.1f), camera->GetTargetPosition());
            
            // randare coroana
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(position.x, position.y + 0.75f, position.z));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75f));
            RenderSimpleMesh(meshes["box"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.f, 0.5f, 0.2f), camera->GetTargetPosition());
        }

        // randare copacei in exteriorul pistei
        for (int i = 0; i < extPoints.size(); i += 99) {
            // calculul pozitiei pentru a nu fi pe pista
            glm::vec3 position = allPoints[i] - (allPoints[i] - extPoints[i]) * 1.5f;

            // randare trunchi
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, position);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.75f, 0.25f));
            RenderSimpleMesh(meshes["box"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.3f, 0.1f, 0.1f), camera->GetTargetPosition());

            // randare coroana
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(position.x, position.y + 0.75f, position.z));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75f));
            RenderSimpleMesh(meshes["box"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.f, 0.5f, 0.2f), camera->GetTargetPosition());
        }
    }

    // randare obstacol dinamic1
    {
        // daca factor1 ajunge egal cu numarul de puncte din allPoints inseamna ca pista a ajuns la final si trebuie sa inceapa
        // din nou de la inceput(inceputul si finalul sunt puncte consecutive) 
        if (factor1 >= allPoints.size() - 1) {
            factor1 = 0.f;
        }

        // generez un traseu prestabilit pentru masina1(jumatate dintre allPoints si extPoints)
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, (allPoints[factor1] + extPoints[factor1]) * 0.5f);
        
        // verific daca pozitia masinii mele(GetTargetPosition()) se intersecteaza cu cea a obstacolului dinamic
        checkCollision1 = Intersect(camera->GetTargetPosition(), (allPoints[factor1] + extPoints[factor1]) * 0.5f, 0.2f);
       
        // calculez unghiul cu care trebuie rotatita masina1 pentru ca aceasta sa se deplaseze mereu cu fata
        float degree = GetAngle(factor1);

        // rotesc masina1 pe OY cu unghiul calculat anterior
        modelMatrix = glm::rotate(modelMatrix, degree, glm::vec3(0.f, 1.f, 0.f));

        // scalare si translatare pentru a fi deasupra pistei
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.3f, 0.75f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.3f, 0));

        RenderSimpleMesh(meshes["box"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.56f, 0, 1), camera->GetTargetPosition());
        
        // incrementez factor1 cu 70 * deltaTimeSeconds si verific la inceput daca acesta poate fi incrementat 
        // cu aceasta valoare, in caz contrar il fac 0
        factor1 += 70 * deltaTimeSeconds;
    }

    // randare obstacol dinamic2
    {
        // daca factor2 ajunge egal cu numarul de puncte din allPoints inseamna ca pista a ajuns la final si trebuie sa inceapa
        // din nou de la inceput(inceputul si finalul sunt puncte consecutive)
        if (factor2 >= allPoints.size() - 1) {
            factor2 = 0.f;
        }

        // generez un traseu prestabilit pentru masina2(jumatate dintre allPoints si inPoints)
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, (allPoints[factor2] + inPoints[factor2]) * 0.5f);

        // verific daca pozitia masinii mele(GetTargetPosition()) se intersecteaza cu cea a obstacolului dinamic
        checkCollision2 = Intersect(camera->GetTargetPosition(), (allPoints[factor2] + inPoints[factor2]) * 0.5f, 0.2f);
        
        // calculez unghiul cu care trebuie rotatita masina1 pentru ca aceasta sa se deplaseze mereu cu fata
        float degree = GetAngle(factor2);

        // rotesc masina2 pe OY cu unghiul calculat anterior
        modelMatrix = glm::rotate(modelMatrix, degree, glm::vec3(0.f, 1.f, 0.f));

        // scalare si translatare pentru a fi deasupra pistei
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.3f, 0.75f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.3f, 0.f));

        RenderSimpleMesh(meshes["box"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.8f, 0.53f, 0.6f), camera->GetTargetPosition());

        // incrementez factor2 cu 60 * deltaTimeSeconds si verific la inceput daca acesta poate fi incrementat
        // cu aceasta valoare, in caz contrar il fac 0
        factor2 += 60 * deltaTimeSeconds;
    }

    // randare masina mea
    {
        // translatez masina mea in pozitia GetTargetPosition
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        
        // o rotesc pe OY cu rotate(unghi calculat pentru ca atunci cand camera se roteste 
        // masina sa se roteasca in tandem)
        modelMatrix = glm::rotate(modelMatrix, rotate, glm::vec3(0.f, 1.f, 0.f));

        // scalare si translatare pentru a fi deasupra pistei
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.3f, 0.75f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.3f, 0));

        RenderSimpleMesh(meshes["box"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0, 1, 1), camera->GetTargetPosition());
    }
}

void Tema2::Update(float deltaTimeSeconds)
{   
    // daca projection type == true, atunci randez scena pentru camera din spatele masinii 
    projectionType = true;
    RenderScene(deltaTimeSeconds);

    // daca projection type == true, atunci randez scena pentru camera de deasupra masinii 
    projectionType = false;

    glClear(GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    glViewport(resolution.x * 4 / 5 - 50, 50, resolution.x / 5.f, resolution.y / 5.f);

    cameraViewport->Set(glm::vec3(camera->GetTargetPosition().x, camera->GetTargetPosition().y + 10.f,
        camera->GetTargetPosition().z), camera->GetTargetPosition(), glm::vec3(0, 0, 1));
    RenderScene(deltaTimeSeconds);
}


void Tema2::FrameEnd()
{
}

// RenderSimpleMesh din lab5, la care am adaugat pozitia masinii(in plus, viewMatrix si projectionMatrix sunt 
// calculate diferit in functie de tipul camerei pentru care calculez(cea din spatele masinii sau cea de deasupra))
void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, const glm::vec3& carPosition)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    GLint modelObjectColor = glGetUniformLocation(shader->GetProgramID(), "object_color");
    glUniform3fv(modelObjectColor, 1, glm::value_ptr(color));

    GLint modelCarPosition = glGetUniformLocation(shader->GetProgramID(), "car_position");
    glUniform3fv(modelCarPosition, 1, glm::value_ptr(carPosition));

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix;
    if (projectionType) {
        viewMatrix = camera->GetViewMatrix();
    } 
    else {
        viewMatrix = cameraViewport->GetViewMatrix();
    }
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    if (projectionType) {
        glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
        int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
        glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    } 
    else {
        glm::mat4 projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
        int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
        glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // daca exista coliziuni intre masina mea si cele 2 obstacole dinamice
    // nu pot sa misc deloc masina cu ajutorul tastelor W-A-S-D
    if (!checkCollision1 && !checkCollision2) {
        float cameraSpeed = 2.0f;
        // verific daca masina este in interiorul pistei folosind metoda ariilor descrisa pe ocw
        bool checkInside = InsideRacetrack(camera->GetTargetPosition());
        
        // daca se afla in interiorul pistei pot sa merg fata-spate 
        if (checkInside) {
            // miscare in fata 
            if (window->KeyHold(GLFW_KEY_W)) {
                camera->MoveForward(deltaTime * cameraSpeed);
            }
            // miscare in spate
            if (window->KeyHold(GLFW_KEY_S)) {
                camera->MoveForward(-deltaTime * cameraSpeed);
            }
        }
        // altfel verific daca pozitiile fata-spate sunt accesibile, caz in care se produc
        else {
            float speed = deltaTime * cameraSpeed;
            if (window->KeyHold(GLFW_KEY_W)) {
                if (InsideRacetrack(camera->position + glm::normalize(camera->forward) * speed + camera->forward * camera->distanceToTarget)) {
                    camera->MoveForward(speed);
                }
            }
            if (window->KeyHold(GLFW_KEY_S)) {
                if (InsideRacetrack(camera->position - glm::normalize(camera->forward) * speed + camera->forward * camera->distanceToTarget)) {
                    camera->MoveForward(-speed);
                }
            }
        }

        // pot sa rotesc masina stanga-dreapta cand se blocheaza la iesirea din pista pentru a reveni pe pista
        // rotire dreapta
        if (window->KeyHold(GLFW_KEY_D)) {
            // salvez rotirea camerei in rotate pentru a roti si masina odata cu camera
            float rotateCamera = deltaTime * cameraSpeed;
            camera->RotateThirdPerson_OY(-rotateCamera);
            rotate -= rotateCamera;
        }
        // rotire stanga
        if (window->KeyHold(GLFW_KEY_A)) {
            // salvez rotirea camerei in rotate pentru a roti si masina odata cu camera
            float rotateCamera = deltaTime * cameraSpeed;
            camera->RotateThirdPerson_OY(rotateCamera);
            rotate += rotateCamera;
        }
    }
}
        


void Tema2::OnKeyPress(int key, int mods)
{
}


void Tema2::OnKeyRelease(int key, int mods)
{
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}

// functie ce calculeaza aria unui triunghi pentru 3 puncte date
float Tema2::TriangleArea(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
    glm::vec3 AB = glm::vec3(B.x - A.x, 0.f, B.z - A.z);
    glm::vec3 AC = glm::vec3(C.x - A.x, 0.f, C.z - A.z);
    float cross_product = AB.x * AC.z - AB.z * AC.x;
    return abs(cross_product) / 2;
}

// functie ce verifica daca un punct este in interiorul triunghiului
bool Tema2::InsideTriangle(glm::vec3 P, glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
    float triangle_area = TriangleArea(A, B, C);
    float area_sum = 0;
    area_sum += TriangleArea(A, B, P);
    area_sum += TriangleArea(A, C, P);
    area_sum += TriangleArea(B, C, P);

    return abs(triangle_area - area_sum) <= 0.001f;
}

// functie ce verifica daca un punct este in interiorul pistei
bool Tema2::InsideRacetrack(glm::vec3 P) {
    std::vector<glm::vec3> vertices = { };

    for (int i = 0; i < allPoints.size() - 1; i++) {
        vertices.push_back(extPoints[i]);
        vertices.push_back(inPoints[i]);
    }
    vertices.push_back(extPoints[0]);
    vertices.push_back(inPoints[0]);

    int i = 0;
    for (i; i < vertices.size() - 2; i++) {
        if (InsideTriangle(P, vertices[i], vertices[i + 1], vertices[i + 2])) {
            return true;
        }
    }
    if (InsideTriangle(P, vertices[i], vertices[i + 1], vertices[0])) {
        return true;
    }
    if (InsideTriangle(P, vertices[i], vertices[0], vertices[1])) {
        return true;
    }

    return false;
}

// functie ce verifica daca 2 sfere se intersecteaza
bool Tema2::Intersect(glm::vec3 sphere, glm::vec3 other, float radius) {
    float distance = sqrt(
        (sphere.x - other.x) * (sphere.x - other.x) +
        (sphere.y - other.y) * (sphere.y - other.y) +
        (sphere.z - other.z) * (sphere.z - other.z));
    return distance < radius * 2;
}

// functie ce calculeaza unghiul de rotatie pentru obstacolele dinamice in functie de un factor
float Tema2::GetAngle(float factor) {
    float deltaX = (allPoints[factor + 1].x + inPoints[factor + 1].x - allPoints[factor].x - inPoints[factor].x) * 0.5f;
    float deltaZ = (allPoints[factor + 1].z + inPoints[factor + 1].z - allPoints[factor].z - inPoints[factor].z) * 0.5f;
    float degree;
    float arctg = atan(deltaZ / deltaX);
    if (deltaX < 0) {
        if (deltaZ > deltaX) {
            degree = M_PI_2 - arctg;
        }
        else {
            degree = 3 * M_PI_2 - arctg;
        }
    }
    else {
        if (deltaZ > deltaX) {
            degree = 3 * M_PI_2 - arctg;
        }
        else {
            degree = M_PI_2 - arctg;
        }
    }

    return degree;
}