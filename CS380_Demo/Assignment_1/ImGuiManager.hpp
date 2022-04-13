/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ImGuiManager.hpp
Purpose: This file is header file for managing ImGui, there are functions for rendering ImGui window.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_1
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/


#pragma once

//Include ImGui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ObjectManager.h"
#include "../CS300 Project/SplinePath.h"

class ImGuiManager
{
public:
    ImGuiManager();
    ImGuiManager(GLFWwindow* window);
    ~ImGuiManager();

    void ImGuiRender(ObjectManager* objManager, SplinePath* s, SplinePath* s2, SplinePath* s3);

    void ShowPointsInformationWindow(SplinePath* s, SplinePath* s2, SplinePath* s3);
    void ShowCameraWindow();
    void ShowCar();

    bool GetCameraChangeButton();
    bool GetCameraPosition();

    bool GetMovingCar();

private:
    bool camera_position;
    bool change_camera_button = false;
    bool moving_car = true;
};