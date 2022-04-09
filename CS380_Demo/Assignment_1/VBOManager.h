/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: VBOManager.h
Purpose: This file is header file for VBOManager class.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_1
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/
#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "ObjectTypeEnum.h"
#include "Mesh.h"
#include "OBJReader.h"
#include "VAOManager.h"

const int vertex = 0,
normal = 1;


class VBOManager
{
public:
    VBOManager();
    ~VBOManager();

    void CleanUPBuffers();

    void LoadOBJFile();
    void SetUpBuffers();

    void UseVertexBuffers(ObjectTypeEnum objType);
    void UnUseVertexBuffers();

    void UseLineBuffers(LineTypeEnum objType);
    void UnUseLineBuffers();

    void UseFaceVertexBuffers(ObjectTypeEnum objType);
    void UnUseFaceVertexBuffers();

    void UseFaceNormalBuffers(ObjectTypeEnum objType);
    void UnUseFaceNormalBuffers();

    void DrawLine(LineTypeEnum type);
    void DrawMesh(ObjectTypeEnum type);

    void DrawFaceMesh(ObjectTypeEnum type);
    void DrawFaceNormalLine(ObjectTypeEnum type);

    Mesh* FindMesh(ObjectTypeEnum type);
    Mesh* FindMesh(LineTypeEnum type);

private:
    void GenerateVBO();
    void SetVertexBuffer();

    void SetArrayBuffers(GLuint* buffer, int type);
    void SetIndexBuffers(GLuint* buffer, int type);
    void SetLineBuffers(GLuint* buffer, int type);
    void SetFaceVertexBuffers(GLuint* buffer, int type);
    void SetFaceNormalBuffers(GLuint* buffer, int type);

    void CalculateOrbit(float distance);

    VAOManager vaoManager;
    GLuint* programID=nullptr;

    std::vector<GLuint*> vertexBuffer;
    std::vector<GLuint*> lineBuffer;
    std::vector<GLuint*> faceVertexBuffer;
    std::vector<GLuint*> faceNormalBuffer;

    OBJReader objReader;

    GLuint vboSphere[3];
    GLuint vboSphereModified[3];
    GLuint vboSphere_4[3];
    GLuint vboCube[3];
    GLuint vboBunny[3];
    GLuint vboCreateSphere[3];
    GLuint vboCar[3];

    GLuint vboSphereLine[1];
    GLuint vboSphereModifiedLine[1];
    GLuint vboSphere_4Line[1];
    GLuint vboCubeLine[1];
    GLuint vboBunnyLine[1];
    GLuint vboCreateSphereLine[1];
    GLuint vboCarLine[1];
    GLuint vboOrbitLine[1];
    GLuint vboLine[1];

    GLuint vboSphereFace[2];
    GLuint vboSphereModifiedFace[2];
    GLuint vboSphere_4Face[2];
    GLuint vboCubeFace[2];
    GLuint vboBunnyFace[2];
    GLuint vboCreateSphereFace[2];
    GLuint vboCarFace[2];

    GLuint vboSphereFaceNormal[1];
    GLuint vboSphereModifiedFaceNormal[1];
    GLuint vboSphere_4FaceNormal[1];
    GLuint vboCubeFaceNormal[1];
    GLuint vboBunnyFaceNormal[1];
    GLuint vboCreateSphereFaceNormal[1];
    GLuint vboCarFaceNormal[1];
    
    Mesh sphere;
    Mesh sphereModified;
    Mesh sphere_4;
    Mesh cube;
    Mesh bunny;
    Mesh createSphere;
    Mesh car;
    Mesh line;

    std::vector<glm::vec3> orbitVertex;
   

};