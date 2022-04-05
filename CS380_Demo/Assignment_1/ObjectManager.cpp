/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ObjectManager.cpp
Purpose: This file is source file for ObjectManager class. This file manages for objects and vbomanager.
There are functions for drawing objects and setting uniforms values and matrix for shader.
And also in this file, vboManager can load obj files, set up buffers and clean up buffers.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_1
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/

#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::Init()
{
    vboManager.LoadOBJFile();
    vboManager.SetUpBuffers();

    sphere = vboManager.FindMesh(ObjectTypeEnum::eSPHERE);
    bunny = vboManager.FindMesh(ObjectTypeEnum::eBUNNY);
    cloud = vboManager.FindMesh(ObjectTypeEnum::e4SPHERE);
    sphere_modified = vboManager.FindMesh(ObjectTypeEnum::eSPHERE_MODIFIED);
    cube = vboManager.FindMesh(ObjectTypeEnum::eCUBE);
}

void ObjectManager::CleanUP()
{
    vboManager.CleanUPBuffers();
}

void ObjectManager::AddObject(std::string name, glm::mat4* transform)
{
    objects.insert(std::make_pair(name, transform));
}

void ObjectManager::DrawObject(ObjectTypeEnum type, glm::mat4 projection, glm::mat4 camera, glm::vec3 color)
{
    SetUniforms(projection, camera);
    SetColor(color);

    if (isDrawVertexNormalBased == true)
    {
        vboManager.UseVertexBuffers(type);
        vboManager.DrawMesh(type);
        vboManager.UnUseVertexBuffers();
    }
    else
    {
        vboManager.UseFaceVertexBuffers(type);
        vboManager.DrawFaceMesh(type);
        vboManager.UnUseFaceVertexBuffers();
    }

    int index = static_cast<int>(type);

    if (drawVertexNormalVector)
    {
        LineTypeEnum lineType = static_cast<LineTypeEnum>(type);
        DrawVertexNormalVector(lineType);
    }

    if (drawFaceNormalVector)
    {
        DrawFaceNormalVector(type);
    }
}

void ObjectManager::SetVersionOfNormalBased(bool vertexVersion)
{
    isDrawVertexNormalBased = vertexVersion;
}

bool ObjectManager::GetVersionOfNormalBased()
{
    return isDrawVertexNormalBased;
}

void ObjectManager::DrawVertexNormalVector(LineTypeEnum type)
{
    vboManager.UseLineBuffers(type);
    vboManager.DrawLine(type);
    vboManager.UnUseLineBuffers();
}

void ObjectManager::SetDrawVertexNormalVector(bool draw)
{
    drawVertexNormalVector = draw;
}

bool ObjectManager::GetDrawVertexNormalVector()
{
    return drawVertexNormalVector;
}

void ObjectManager::DrawFaceNormalVector(ObjectTypeEnum type)
{
    vboManager.UseFaceNormalBuffers(type);
    vboManager.DrawFaceNormalLine(type);
    vboManager.UnUseFaceNormalBuffers();
}

void ObjectManager::SetDrawFaceNormalVector(bool draw)
{
    drawFaceNormalVector = draw;
}

bool ObjectManager::GetDrawFaceNormalVector()
{
    return drawFaceNormalVector;
}

void ObjectManager::SetUniforms(glm::mat4 ndc, glm::mat4 camera)
{
    SetUniformMatrix("ndcMatrix", ndc);
    SetUniformMatrix("cameraMatrix", camera);
}

void ObjectManager::SetUniforms(glm::mat4 ndc, glm::mat4 camera, glm::mat4 tramsform)
{
    SetUniformMatrix("ndcMatrix", ndc);
    SetUniformMatrix("cameraMatrix", camera);
    SetUniformMatrix("transform", tramsform);
}

glm::mat4 ObjectManager::SetTransforms(ObjectTypeEnum type, glm::vec3 translate, glm::vec3 scale, float rotation, glm::vec3 rotationVector)
{
    glm::mat4 modelMat = glm::translate(translate) * glm::rotate(rotation, rotationVector) * glm::scale(scale);
    SetUniformMatrix("transform", modelMat);
    return modelMat;
}

glm::mat4 ObjectManager::SetTransformsForRotation(ObjectTypeEnum type, glm::vec3 translate, glm::vec3 scale, float rotation, glm::vec3 rotationVector)
{
    glm::mat4 modelMat = glm::rotate(rotation, rotationVector) * glm::translate(translate) * glm::scale(scale);
    SetUniformMatrix("transform", modelMat);
    return modelMat;
}

void ObjectManager::SetColor(glm::vec3 color)
{
    GLint uniformLocation = glGetUniformLocation(*programID, "color");
    glUniform3f(uniformLocation, color.x, color.y, color.z);
}

void ObjectManager::SetLightPosition(glm::vec3 lightPos)
{
    SetUniformVector("lightPosition", lightPos);
}

void ObjectManager::SetUniformMatrix(const GLchar* location, glm::mat4 matrix)
{
    GLint uniformLocation = glGetUniformLocation(*programID, location);
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &matrix[0][0]);
}

void ObjectManager::SetUniformVector(const GLchar* location, glm::vec3 vec)
{
    GLint uniformLocation = glGetUniformLocation(*programID, location);
    glUniform3f(uniformLocation, vec.x, vec.y, vec.z);
}

void ObjectManager::SetShaderProgramID(GLuint* id)
{
    programID = id;
}

void ObjectManager::DrawOrbit()
{
    vboManager.UseLineBuffers(LineTypeEnum::eORBIT);
    vboManager.DrawLine(LineTypeEnum::eORBIT);
    vboManager.UnUseLineBuffers();
}


