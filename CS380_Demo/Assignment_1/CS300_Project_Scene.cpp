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
#include <chrono>

const glm::vec3 SALMON(1, 0.453f, 0.453f),
                LIGHTPINK(1, 0.68f, 0.906f),
                RED(1.f, 0.f, 0.f), 
                GREEN(0.f, 1.f, 0.f),
                BLUE(0.f,0.f,1.f),
                LIGHTBLUE(0.4824f, 0.6275f, 0.8941f),
                YELLOW(1.f,1.f,0.2f);

const float PI = 4.0f * atan(1.0f);
auto t1 = std::chrono::system_clock::now();
auto t2 = std::chrono::system_clock::now();

CS300_Project_Scene::CS300_Project_Scene(int windowWidth, int windowHeight) : Scene(windowWidth, windowHeight)
{
    InitMembers();

    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;

    eye = glm::vec3(0.f, 8.f,12.0f);
    eye_2d = glm::vec3(0.f, 12.f, -1.f);
    target = glm::vec3(0.f, 0.f, 0.f);
    up = glm::vec3(0.f, 1.f, 0.f);
    up_2d = glm::vec3(0.f, 0.f, -1.f);
    fov = 0.5f * PI;
    aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    near_ = 0.1f;
    far_ = 10.f;

    cam.CameraSetUP(eye, target, up, fov, aspect, near_, far_);
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
    line_shader = LoadShaders("../Common/shaders/LineVertexShader.vert",
                               "../Common/shaders/LineFragmentShader.frag");

    t1 = std::chrono::system_clock::now();
    t2 = std::chrono::system_clock::now();

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
    t2 = std::chrono::system_clock::now();
    std::chrono::duration<float> time = t2 - t1;
    t1 = t2;
    float deltaTime = time.count();
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
    //objectManager.DrawOrbit();
    
    glm::mat4 modelMat = glm::mat4(1.0f);
    float distance = 10.f / PI;
    glm::vec3 scaleVector = glm::vec3(1.f);
    glm::vec3 translate = glm::vec3(0.f);
    glm::vec3 rotateVector = glm::vec3(0.f, 1.0f, 0.f);
    float angleOfRotation = 0.f;
    glm::vec3 lightPosition = glm::vec3(4.f, 2.f, 0.f);

    objectManager.SetLightPosition(lightPosition);

    scaleVector = glm::vec3(0.3f,0.3f,0.7f);
    angleOfRotation = PI / 2;
    objectManager.SetTransforms(ObjectTypeEnum::eCAR, translate, scaleVector, angleOfRotation, rotateVector);
    objectManager.DrawObject(ObjectTypeEnum::eCAR, projection, camera, YELLOW);

    scaleVector = glm::vec3(1.f);
    translate = glm::vec3(distance, 0.f, 0.f);
    //splinePath.DrawSplinePoints(&objectManager, projection, camera, BLUE);
    trackLeft.DrawSplinePoints(&objectManager, projection, camera, BLUE);
    //trackRight.DrawSplinePoints(&objectManager, projection, camera, BLUE);

    glUseProgram(line_shader);
    translate = glm::vec3(0, 0, 0);
    angleOfRotation = 0.f;
    modelMat = glm::translate(translate) * glm::rotate(angleOfRotation, rotateVector) * glm::scale(scaleVector);
    splinePath.DrawSplinePath(&line_shader, modelMat, projection, camera, RED);
    trackLeft.DrawTrackPath(&line_shader, modelMat, projection, camera, GREEN);
    trackRight.DrawTrackPath2(&line_shader, modelMat, projection, camera, GREEN);
    splinePath.DrawOptimizedSplinePoints(&line_shader, modelMat, projection, camera, BLUE);
    //splinePath.OptimizationPath();
    //splinePath.DrawSplinePath(&line_shader, modelMat, projection, camera, BLUE, true);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(0);

    imGui->ImGuiRender(&objectManager, &splinePath, &trackLeft, &trackRight);
    //imGui->ImGuiRender(&objectManager, &trackLeft);
    //imGui->ImGuiRender(&objectManager, &trackRight);
    if (imGui->GetCameraChangeButton() == true)
    {
        if (imGui->GetCameraPosition() == true)
        {
            cam.CameraSetUP(eye, target, up, fov, aspect, near_, far_);
            camera = CameraToWorld(cam);
        }
        else
        {
            cam.CameraSetUP(eye_2d, target, up_2d, fov, aspect, near_, far_);
            camera = CameraToWorld(cam);
        }
    }

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
        trackLeft.MoveCurrentPoints(true, true);
        trackRight.MoveCurrentPoints(true, true);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        splinePath.MoveCurrentPoints(false, false);
        trackLeft.MoveCurrentPoints(false, false);
        trackRight.MoveCurrentPoints(false, false);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        splinePath.MoveCurrentPoints(true, false);
        trackLeft.MoveCurrentPoints(true, false);
        trackRight.MoveCurrentPoints(true, false);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        splinePath.MoveCurrentPoints(false, true);
        trackLeft.MoveCurrentPoints(false, true);
        trackRight.MoveCurrentPoints(false, true);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        splinePath.IncreaseIteration();
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



