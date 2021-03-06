#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "ObjectManager.h"

#define TIME 0.005f

class SplinePath
{
public:
    SplinePath();
    ~SplinePath();

    void DrawSplinePoints(ObjectManager* m, glm::mat4 proj, glm::mat4 cam, glm::vec3 c);
    void DrawSplinePath(GLuint* shader, glm::mat4 transform, glm::mat4 proj, glm::mat4 cam, glm::vec3 color);
    void DrawTrackPath(GLuint* shader, glm::mat4 transform, glm::mat4 proj, glm::mat4 cam, glm::vec3 color);
    void DrawTrackPath2(GLuint* shader, glm::mat4 transform, glm::mat4 proj, glm::mat4 cam, glm::vec3 color);
    void DrawOptimizedSplinePoints(GLuint* shader, glm::mat4 transform, glm::mat4 proj, glm::mat4 cam, glm::vec3 color);

    void NextSplinePoints(bool to_left);

    void AddSplinePoint();
    void DeleteCurrentSplinePoint();

    int GetCurrentPointID();
    
    void MoveCurrentPoints(bool to_front, bool way);// go front or left, go back or right

    glm::vec3 GetSplinePoint(float t, bool IsOptimized = false);
    glm::vec3 GetSplineGradient(float t, bool IsOptimized = false);

    void CalculationSplinePoint();
    void CalculationTrackPoint();
    void CalculationOptimizationPath();

    void UpdateBuffers();
    void UpdateBuffersTrack();
    void UpdateBuffersTrack2();
    void UpdateOptimizedBuffers();

    void UpdateCar(float delta);
    void MovingCar();

    void IncreaseIteration();
    void resetInteration();

    glm::vec3 GetSplinePositionForCar();
    float GetCarRotationAngle();

    int iterationNum = 0;
    float car = 0.f;
    glm::vec3 coordinate;
    float rotation = 0.f;

private:
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> spline_pts;
    std::vector<glm::vec3> tracks;
    std::vector<glm::vec3> tracks2;
    glm::vec3 scale;
    
    std::vector<glm::vec3> optimizedPoints;
    std::vector<float> displacement;
    std::vector<glm::vec3> optimizedSpline;
    bool first = true;

    GLuint optimizedVao;
    GLuint optimizedVbo;

    GLuint vao;
    GLuint vao2;
    GLuint vao3;

    GLuint vbo;
    GLuint vbo2;
    GLuint vbo3;

    int current_pts;
    float car_position_time;
    float car_rotation = 0.f;
    glm::vec3 pre_pos;
    glm::vec3 curr_pos;
    glm::vec3 pro_pos;

};

