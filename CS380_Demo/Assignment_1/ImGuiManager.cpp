/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ImGuiManager.cpp
Purpose: This file is source file for managing ImGui, there are functions for rendering ImGui window.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_1
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/

#include "ImGuiManager.hpp"

ImGuiManager::ImGuiManager()
{
    camera_position = true;
}

ImGuiManager::ImGuiManager(GLFWwindow* window)
{
    const char* version = "#version 410";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(version);
}

ImGuiManager::~ImGuiManager()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::ImGuiRender(ObjectManager* objManager, SplinePath* s, SplinePath* s2, SplinePath* s3)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static int vectorType = 2;
    static int drawType = 0;

    ShowPointsInformationWindow(s, s2, s3);
    ShowCameraWindow();
    ShowCar();

    //ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::ShowPointsInformationWindow(SplinePath* s, SplinePath* s2, SplinePath* s3)
{
    if (!ImGui::CollapsingHeader("Spline Points"))
    {
        return;
    }

    ImGui::Text("Current point id: %d", s->GetCurrentPointID());
    
    if (ImGui::Button("Add Point"))
    {
        s->AddSplinePoint();
        s2->AddSplinePoint();
        s3->AddSplinePoint();
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete Current Point"))
    {
        s->DeleteCurrentSplinePoint();
        s2->DeleteCurrentSplinePoint();
        s3->DeleteCurrentSplinePoint();
    }

    ImGui::Text("Change point");
    ImGui::SameLine();
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton("##left", ImGuiDir_Left))
    {
        s->NextSplinePoints(true);
        s2->NextSplinePoints(true);
        s3->NextSplinePoints(true);
    }
    ImGui::SameLine(0.f, ImGui::GetStyle().ItemInnerSpacing.x);
    if (ImGui::ArrowButton("##right", ImGuiDir_Right))
    {
        s->NextSplinePoints(false);
        s2->NextSplinePoints(false);
        s3->NextSplinePoints(false);
    }
    ImGui::PopButtonRepeat();

}

void ImGuiManager::ShowCameraWindow()
{
    if (!ImGui::CollapsingHeader("Camera"))
    {
        return;
    }
    if (ImGui::Button("Change Camera Position"))
    {
        if (camera_position == true)
        {
            camera_position = false;
        }
        else
        {
            camera_position = true;
        }
        change_camera_button = true;
    }
}

void ImGuiManager::ShowCar()
{
    if (!ImGui::CollapsingHeader("Moving Object"))
    {
        return;
    }

    if (ImGui::Button("Stop Moving Object"))
    {
        if (moving_car == false)
        {
            moving_car = true;
        }
        else
        {
            moving_car = false;
        }
    }

}

bool ImGuiManager::GetCameraChangeButton()
{
    if (change_camera_button == true)
    {
        change_camera_button = false;
        return true;
    }

    return false;
}

bool ImGuiManager::GetCameraPosition()
{
    return camera_position;
}

bool ImGuiManager::GetMovingCar()
{
    return moving_car;
}
