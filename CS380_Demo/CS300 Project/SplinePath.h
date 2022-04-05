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

    void DrawSplinePath(ObjectManager* m, glm::mat4 proj, glm::mat4 cam, glm::vec3 c);

    void NextSplinePoints(bool to_left);

    void AddSplinePoint();
    void DeleteCurrentSplinePoint();

    int GetCurrentPointID();
    
    void MoveCurrentPoints(bool to_front, bool way);// go front or left, go back or right

private:
    std::vector<glm::vec3> points;
    glm::vec3 scale;

    int current_pts;
};

