/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: CS300_Project_Scene.hpp
Purpose: This file is header file for creating assignment scene.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_1
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/
#pragma once

#include "../Common/Scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Mesh.h"
#include "ImGuiManager.hpp"
#include "VBOManager.h"
#include "Camera.h"
#include "Projection.h"
#include "ObjectManager.h"
#include "../CS300 Project/SplinePath.h"

const int SPHERE_OBJECT_NUM = 8;

class CS300_Project_Scene :public Scene
{
public:
    CS300_Project_Scene() = default;
    CS300_Project_Scene(int windowWidth, int windowHeight);
    virtual ~CS300_Project_Scene();

    int Init() override;
    void CleanUp() override;

    void ResizeWindow(int width, int height) override;

    int Render() override;
    int postRender() override;

    void KeyboardInput(GLFWwindow* window) override;

private:
    void InitMembers();
    void SetupNanoGUI(GLFWwindow* window) override;
    void SetupBuffers();


    GLuint programID = 0 ;
    GLuint line_shader = 0;

    ImGuiManager* imGui = nullptr;

    int windowWidth = 0, windowHeight = 0;

    Camera cam;
    glm::mat4 camera;
    glm::mat4 projection;
    
    ObjectManager objectManager;
    SplinePath splinePath;

    float rotationAngle = 0.f;

    glm::mat4 bunny = glm::mat4(1.0f);
    glm::mat4 createdSphere[SPHERE_OBJECT_NUM] = { glm::mat4(1.0f) };
    glm::mat4 sky = glm::mat4(1.0f);
    glm::mat4 sphereModified = glm::mat4(1.0f);
    glm::mat4 sphere = glm::mat4(1.0f);
    glm::mat4 cube = glm::mat4(1.0f);
};