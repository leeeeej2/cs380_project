#include "SplinePath.h"
const float PI = 4.0f * atan(1.0f);
SplinePath::SplinePath()
{
    scale = glm::vec3(1.f);
    float distance = 10.f / PI;
    points.push_back({ distance, 0, 0 });
    points.push_back({ 0,0,-distance });
    points.push_back({ -distance,0,0 });
    points.push_back({ 0,0,distance });
    current_pts = 0;
}

SplinePath::~SplinePath()
{
}

void SplinePath::DrawSplinePath(ObjectManager* m, glm::mat4 proj, glm::mat4 cam, glm::vec3 c)
{
    int size = points.size();

    for (int i = 0; i < size; ++i)
    {
        m->SetTransforms(ObjectTypeEnum::eCREATE_SPHERE, points[i], scale, 0, scale);
        if (current_pts == i)
        {
            m->DrawObject(ObjectTypeEnum::eCREATE_SPHERE, proj, cam, {1,0,0});
        }
        else
        {
            m->DrawObject(ObjectTypeEnum::eCREATE_SPHERE, proj, cam, c);
        }
    }
}

void SplinePath::NextSplinePoints(bool to_left)
{
    if (to_left == true)
    {
        --current_pts;
        if (current_pts < 0)
        {
            current_pts = points.size() - 1;
        }
    }
    else
    {
        ++current_pts;
        if (current_pts >= points.size())
        {
            current_pts = 0;
        }
    }
}

void SplinePath::AddSplinePoint()
{
    points.push_back({0,0,0});
}

void SplinePath::DeleteCurrentSplinePoint()
{
    std::vector<glm::vec3>::iterator it = points.begin() + current_pts;
    points.erase(it);
}

int SplinePath::GetCurrentPointID()
{
    return current_pts;
}

void SplinePath::MoveCurrentPoints(bool to_front, bool way)
{
    float distance = PI / 360.f;
    if (to_front == true)
    {
        if (way == true)//forward
        {
            points[current_pts].z -= distance;
        }
        else //backward
        {
            points[current_pts].z += distance;
        }
    }
    else
    {
        if (way == true) //left
        {
            points[current_pts].x += distance;
        }
        else //right
        {
            points[current_pts].x -= distance;
        }
    }
}
