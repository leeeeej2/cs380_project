#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "ObjectManager.h"

class SplinePath
{
public:
    SplinePath();
    ~SplinePath();

    void DrawSplinePoints(ObjectManager* m, glm::mat4 proj, glm::mat4 cam, glm::vec3 c);
    void DrawSplinePath(GLuint* shader, glm::mat4 transform, glm::mat4 proj, glm::mat4 cam, glm::vec3 color);
    void DrawTrackPath(GLuint* shader, glm::mat4 transform, glm::mat4 proj, glm::mat4 cam, glm::vec3 color);
    void DrawTrackPath2(GLuint* shader, glm::mat4 transform, glm::mat4 proj, glm::mat4 cam, glm::vec3 color);

    void NextSplinePoints(bool to_left);

    void AddSplinePoint();
    void DeleteCurrentSplinePoint();

    int GetCurrentPointID();
    
    void MoveCurrentPoints(bool to_front, bool way);// go front or left, go back or right

    glm::vec3 GetSplinePoint(float t);
    glm::vec3 GetSplineGradient(float t);

    void CalculationSplinePoint();
    void CalculationTrackPoint();
    void UpdateBuffers();
    void UpdateBuffersTrack();
    void UpdateBuffersTrack2();

private:
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> spline_pts;
    std::vector<glm::vec3> tracks;
    std::vector<glm::vec3> tracks2;
    glm::vec3 scale;
    
    GLuint vao;
    GLuint vao2;
    GLuint vao3;

    GLuint vbo;
    GLuint vbo2;
    GLuint vbo3;

    int current_pts;
};

