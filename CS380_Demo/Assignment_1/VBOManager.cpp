/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: VBOManager.cpp
Purpose: This file is source file for VBOManager class. This file manages for vertex buffers and vaoManager.
In this file, buffers are created, deleted and binded. And It helps drawing objects and lines for objectManager.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_1
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/

#include "VBOManager.h"

const float PI = 4.0f * atan(1.0f);
VBOManager::VBOManager()
{
    SetVertexBuffer();
}

VBOManager::~VBOManager()
{
    
}

void VBOManager::CleanUPBuffers()
{
    for (int i = 0; i < vertexBuffer.size(); ++i)
    {
        glDeleteBuffers(3, vertexBuffer.at(i));
    }

    for (int i = 0; i < lineBuffer.size(); ++i)
    {
        glDeleteBuffers(1, lineBuffer.at(i));
    }

    for (int i = 0; i < faceVertexBuffer.size(); ++i)
    {
        glDeleteBuffers(2, faceVertexBuffer.at(i));
    }

    for (int i = 0; i < faceNormalBuffer.size(); ++i)
    {
        glDeleteBuffers(1, faceNormalBuffer.at(i));
    }

    vaoManager.CleanUPVAO();
}

void VBOManager::UseVertexBuffers(ObjectTypeEnum objType)
{
    int objectIndex = static_cast<int>(objType);

    vaoManager.UseVertexArrayID(objType);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.at(objectIndex)[vertex]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.at(objectIndex)[normal]);

    glEnableVertexAttribArray(vertex);
    glEnableVertexAttribArray(normal);
}

void VBOManager::UnUseVertexBuffers()
{
    glDisableVertexAttribArray(vertex);
    glDisableVertexAttribArray(normal);
    vaoManager.UnUseVertexArrayID();
}

void VBOManager::UseLineBuffers(LineTypeEnum objType)
{
    int objectIndex = static_cast<int>(objType);

    vaoManager.UseVertexArrayID(objType);
    glEnableVertexAttribArray(vertex);
    glBindBuffer(GL_ARRAY_BUFFER, *lineBuffer.at(objectIndex));
}

void VBOManager::UnUseLineBuffers()
{
    glDisableVertexAttribArray(vertex);
    vaoManager.UnUseVertexArrayID();
}

void VBOManager::UseFaceVertexBuffers(ObjectTypeEnum objType)
{
    int objectIndex = static_cast<int>(objType);

    vaoManager.UseFaceVertexArrayID(objType);
    glBindBuffer(GL_ARRAY_BUFFER, faceVertexBuffer.at(objectIndex)[vertex]);
    glBindBuffer(GL_ARRAY_BUFFER, faceVertexBuffer.at(objectIndex)[normal]);

    glEnableVertexAttribArray(vertex);
    glEnableVertexAttribArray(normal);
}

void VBOManager::UnUseFaceVertexBuffers()
{
    glDisableVertexAttribArray(vertex);
    glDisableVertexAttribArray(normal);
    vaoManager.UnUseVertexArrayID();
}

void VBOManager::UseFaceNormalBuffers(ObjectTypeEnum objType)
{
    int objectIndex = static_cast<int>(objType);

    vaoManager.UseFaceNormalVertexArrayID(objType);
    glEnableVertexAttribArray(vertex);
    glBindBuffer(GL_ARRAY_BUFFER, *faceNormalBuffer.at(objectIndex));
}

void VBOManager::UnUseFaceNormalBuffers()
{
    glDisableVertexAttribArray(vertex);
    vaoManager.UnUseVertexArrayID();
}

void VBOManager::DrawLine(LineTypeEnum type)
{
    if (type == LineTypeEnum::eORBIT)
    {
        glDrawArrays(GL_LINES, 0, (GLsizei)orbitVertex.size());
        return;
    }

    Mesh* mesh = FindMesh(type);
    glDrawArrays(GL_LINES, 0, mesh->getVertexNormalCount());
}

void VBOManager::DrawMesh(ObjectTypeEnum type)
{
    Mesh* mesh = FindMesh(type);
    glDrawElements(GL_TRIANGLES, mesh->getIndexBufferSize(), GL_UNSIGNED_INT, (void*)0);
}

void VBOManager::DrawFaceMesh(ObjectTypeEnum type)
{
    Mesh* mesh = FindMesh(type);
    glDrawArrays(GL_TRIANGLES, 0, mesh->getFaceBufferSize());
}

void VBOManager::DrawFaceNormalLine(ObjectTypeEnum type)
{
    Mesh* mesh = FindMesh(type);
    glDrawArrays(GL_LINES, 0, mesh->getFaceNormalCount());
}

void VBOManager::GenerateVBO()
{
    vaoManager.GenerateVAO();

    for (int i = 0; i < vertexBuffer.size(); ++i)
    {
        glGenBuffers(3, vertexBuffer.at(i));
    }

    for (int i = 0; i < lineBuffer.size(); ++i)
    {
        glGenBuffers(1, lineBuffer.at(i));
    }

    for (int i = 0; i < faceVertexBuffer.size(); ++i)
    {
        glGenBuffers(2, faceVertexBuffer.at(i));
    }

    for (int i = 0; i < faceNormalBuffer.size(); ++i)
    {
        glGenBuffers(1, faceNormalBuffer.at(i));
    }

}

void VBOManager::SetVertexBuffer()
{
    vertexBuffer.push_back(vboSphere);
    vertexBuffer.push_back(vboSphereModified);
    vertexBuffer.push_back(vboSphere_4);
    vertexBuffer.push_back(vboCube);
    vertexBuffer.push_back(vboBunny);
    vertexBuffer.push_back(vboCreateSphere);

    lineBuffer.push_back(vboSphereLine);
    lineBuffer.push_back(vboSphereModifiedLine);
    lineBuffer.push_back(vboSphere_4Line);
    lineBuffer.push_back(vboCubeLine);
    lineBuffer.push_back(vboBunnyLine);
    lineBuffer.push_back(vboOrbitLine);
    lineBuffer.push_back(vboCreateSphereLine);

    faceVertexBuffer.push_back(vboSphereFace);
    faceVertexBuffer.push_back(vboSphereModifiedFace);
    faceVertexBuffer.push_back(vboSphere_4Face);
    faceVertexBuffer.push_back(vboCubeFace);
    faceVertexBuffer.push_back(vboBunnyFace);
    faceVertexBuffer.push_back(vboCreateSphereFace);

    faceNormalBuffer.push_back(vboSphereFaceNormal);
    faceNormalBuffer.push_back(vboSphereModifiedFaceNormal);
    faceNormalBuffer.push_back(vboSphere_4FaceNormal);
    faceNormalBuffer.push_back(vboCubeFaceNormal);
    faceNormalBuffer.push_back(vboBunnyFaceNormal);
    faceNormalBuffer.push_back(vboCreateSphereFaceNormal);

}

void VBOManager::LoadOBJFile()
{
    objReader.ReadOBJFile("../Common/models/sphere.obj", &sphere);
    objReader.ReadOBJFile("../Common/models/sphere_modified.obj", &sphereModified);
    objReader.ReadOBJFile("../Common/models/4Sphere.obj", &sphere_4);
    objReader.ReadOBJFile("../Common/models/cube.obj", &cube, OBJReader::ReadMethod::LINE_BY_LINE, true);
    objReader.ReadOBJFile("../Common/models/bunny.obj", &bunny);
    createSphere.CreateSphere(0.1f, 30);

    CalculateOrbit(10.f / PI);
    SetUpBuffers();
}

void VBOManager::SetUpBuffers()
{
    GenerateVBO();
    int count = static_cast<int>(ObjectTypeEnum::COUNT);

    for (int i = 0; i < count; ++i)
    {
        SetArrayBuffers(vertexBuffer.at(i), i);
        SetIndexBuffers(vertexBuffer.at(i), i);

        SetFaceVertexBuffers(faceVertexBuffer.at(i), i);
        SetFaceNormalBuffers(faceNormalBuffer.at(i), i);
    }

    count = static_cast<int>(LineTypeEnum::COUNT);
    for (int i = 0; i < count; ++i)
    {
        SetLineBuffers(lineBuffer.at(i), i);
    }
}

void VBOManager::SetArrayBuffers(GLuint* buffer, int type)
{
    ObjectTypeEnum objectType = static_cast<ObjectTypeEnum>(type);
    Mesh* mesh = FindMesh(objectType);

    vaoManager.UseVertexArrayID(objectType);
    //vbo for vertex
    glBindBuffer(GL_ARRAY_BUFFER, buffer[vertex]);
    glBufferData(GL_ARRAY_BUFFER, mesh->getVertexBufferSize() * sizeof(glm::vec3), mesh->getVertexBuffer(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vertex);

    //vbo for normal vector
    glBindBuffer(GL_ARRAY_BUFFER, buffer[normal]);
    glBufferData(GL_ARRAY_BUFFER, mesh->getVertexBufferSize() * sizeof(glm::vec3), mesh->getVertexNormals(), GL_STATIC_DRAW);
    glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(normal);
}

void VBOManager::SetIndexBuffers(GLuint* buffer, int type)
{
    ObjectTypeEnum objectType = static_cast<ObjectTypeEnum>(type);
    Mesh* mesh = FindMesh(objectType);

    vaoManager.UseVertexArrayID(objectType);

    //vbo for index
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBufferSize() * sizeof(GLint), mesh->getIndexBuffer(), GL_STATIC_DRAW);

}

void VBOManager::SetLineBuffers(GLuint* buffer, int type)
{
    LineTypeEnum objectType = static_cast<LineTypeEnum>(type);
    vaoManager.UseVertexArrayID(objectType);

    if (objectType == LineTypeEnum::eORBIT)
    {
        glBindBuffer(GL_ARRAY_BUFFER, *buffer);
        glBufferData(GL_ARRAY_BUFFER, orbitVertex.size() * sizeof(glm::vec3), orbitVertex.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(vertex);
        return;
    }
    Mesh* mesh = FindMesh(objectType);

    glBindBuffer(GL_ARRAY_BUFFER, *buffer);
    glBufferData(GL_ARRAY_BUFFER, mesh->getVertexNormalCount() * sizeof(glm::vec3), mesh->getVertexNormalsForDisplay(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vertex);
}

void VBOManager::SetFaceVertexBuffers(GLuint* buffer, int type)
{
    ObjectTypeEnum objectType = static_cast<ObjectTypeEnum>(type);
    Mesh* mesh = FindMesh(objectType);

    vaoManager.UseFaceVertexArrayID(objectType);
    //vbo for vertex
    glBindBuffer(GL_ARRAY_BUFFER, buffer[vertex]);
    glBufferData(GL_ARRAY_BUFFER, mesh->getFaceBufferSize() * sizeof(glm::vec3), mesh->getFaceBuffer(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vertex);

    //vbo for normal vector
    glBindBuffer(GL_ARRAY_BUFFER, buffer[normal]);
    glBufferData(GL_ARRAY_BUFFER, mesh->getFaceBufferSize() * sizeof(glm::vec3), mesh->getFaceNormals(), GL_STATIC_DRAW);
    glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(normal);
}

void VBOManager::SetFaceNormalBuffers(GLuint* buffer, int type)
{
    ObjectTypeEnum objectType = static_cast<ObjectTypeEnum>(type);
    vaoManager.UseFaceNormalVertexArrayID(objectType);
    Mesh* mesh = FindMesh(objectType);

    glBindBuffer(GL_ARRAY_BUFFER, *buffer);
    glBufferData(GL_ARRAY_BUFFER, mesh->getFaceNormalCount() * sizeof(glm::vec3), mesh->getFaceNormalsForDisplay(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vertex);
}

void VBOManager::CalculateOrbit(float distance)
{
    int countOfVertex = 30;
    float value = 2 * PI / countOfVertex;
    float degree = 0.f;
    float x = distance * cosf(degree);
    float z = distance * sinf(degree);
    orbitVertex.push_back(glm::vec3(x, 0.f, z));

    for (degree = value; degree < 2 * PI; degree += value)
    {
        float x = distance * cosf(degree);
        float z = distance * sinf(degree);
        orbitVertex.push_back(glm::vec3(x, 0.f, z));
        orbitVertex.push_back(glm::vec3(x, 0.f, z));
    }
}

Mesh* VBOManager::FindMesh(ObjectTypeEnum type)
{
    Mesh* mesh = nullptr;

    switch (type)
    {
    case ObjectTypeEnum::eSPHERE:
        mesh = &sphere;
        break;
    case ObjectTypeEnum::eSPHERE_MODIFIED:
        mesh = &sphereModified;
        break;
    case ObjectTypeEnum::e4SPHERE:
        mesh = &sphere_4;
        break;
    case ObjectTypeEnum::eCUBE:
        mesh = &cube;
        break;
    case ObjectTypeEnum::eBUNNY:
        mesh = &bunny;
        break;
    case ObjectTypeEnum::eCREATE_SPHERE:
        mesh = &createSphere;
        break;
    default:
        break;
    }

    return mesh;
}

Mesh* VBOManager::FindMesh(LineTypeEnum type)
{
    Mesh* mesh = nullptr;

    switch (type)
    {
    case LineTypeEnum::eSPHERE:
        mesh = &sphere;
        break;
    case LineTypeEnum::eSPHERE_MODIFIED:
        mesh = &sphereModified;
        break;
    case LineTypeEnum::e4SPHERE:
        mesh = &sphere_4;
        break;
    case LineTypeEnum::eCUBE:
        mesh = &cube;
        break;
    case LineTypeEnum::eBUNNY:
        mesh = &bunny;
        break;
    case LineTypeEnum::eCREATE_SPHERE:
        mesh = &createSphere;
        break;
    default:
        break;
    }

    return mesh;
}

