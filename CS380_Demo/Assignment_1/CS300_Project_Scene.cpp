/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: CS300_Project_Scene.cpp
Purpose: This file is source file for creating assignment scene.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_1
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/
#include <iostream>
#include "CS300_Project_Scene.hpp"
#include "shader.hpp"

const glm::vec3 SALMON(1, 0.453f, 0.453f),
                LIGHTPINK(1, 0.68f, 0.906f),
                RED(1.f, 0.f, 0.f), 
                GREEN(0.f, 1.f, 0.f),
                BLUE(0.f,0.f,1.f),
                LIGHTBLUE(0.4824f, 0.6275f, 0.8941f),
                YELLOW(1.f,1.f,0.2f);

const float PI = 4.0f * atan(1.0f);

CS300_Project_Scene::CS300_Project_Scene(int windowWidth, int windowHeight) : Scene(windowWidth, windowHeight)
{
    InitMembers();

    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;

    glm::vec3 eye(0.f,2.f,8.0f);
    glm::vec3 target(0.f, 0.f, -1.f);
    glm::vec3 up(0.f, 1.f, 0.f);
    float fov = 0.5f * PI;
    float aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    float near = 0.1f;
    float far = 10.f;

    cam.CameraSetUP(eye, target, up, fov, aspect, near, far);
    camera = CameraToWorld(cam);
    projection = glm::perspective(glm::radians(45.f), aspect, 0.1f, 100.f);
}

CS300_Project_Scene::~CS300_Project_Scene()
{
    InitMembers();
    delete imGui;
}

int CS300_Project_Scene::Init()
{
    programID = LoadShaders("../Common/shaders/VertexShader.vert",
                            "../Common/shaders/FragmentShader.frag");

    SetupBuffers();
    return Scene::Init();
}

void CS300_Project_Scene::CleanUp()
{
    glDeleteProgram(programID);
    objectManager.CleanUP();
}

void CS300_Project_Scene::ResizeWindow(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
}

int CS300_Project_Scene::Render()
{
    camera = CameraToWorld(cam);
    float aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    projection = glm::perspective(glm::radians(45.f), aspect, 0.1f, 100.f);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glUseProgram(programID);
    glm::mat4 orbitMat = glm::mat4(0.50f);
    objectManager.SetUniforms(projection, camera, orbitMat);
    objectManager.DrawOrbit();
    
    glm::mat4 modelMat = glm::mat4(1.0f);
    float distance = 10.f / PI;
    glm::vec3 scaleVector = glm::vec3(1.f);
    glm::vec3 translate = glm::vec3(0.f);
    glm::vec3 rotateVector = glm::vec3(0.f, 1.0f, 0.f);
    float angleOfRotation = 0.f;
    glm::vec3 lightPosition = glm::vec3(4.f, 2.f, 0.f);

    objectManager.SetLightPosition(lightPosition);

    scaleVector = glm::vec3(1.f);
    translate = glm::vec3(distance, 0.f, 0.f);
    splinePath.DrawSplinePath(&objectManager, projection, camera, BLUE);


    glEnable(GL_DEPTH_TEST);
    glUseProgram(0);

    imGui->ImGuiRender(&objectManager, &splinePath);

    return 0;
}

int CS300_Project_Scene::postRender()
{
    rotationAngle += 0.001f;
    return 0;
}

void CS300_Project_Scene::KeyboardInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        splinePath.MoveCurrentPoints(true, true);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        splinePath.MoveCurrentPoints(false, false);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        splinePath.MoveCurrentPoints(true, false);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        splinePath.MoveCurrentPoints(false, true);
    }
}

void CS300_Project_Scene::InitMembers()
{
    programID = 0;
}

void CS300_Project_Scene::SetupNanoGUI(GLFWwindow* window)
{
    imGui = new ImGuiManager(window);
}

void CS300_Project_Scene::SetupBuffers()
{
    objectManager.Init();
    objectManager.SetShaderProgramID(&programID);

    return;
}


