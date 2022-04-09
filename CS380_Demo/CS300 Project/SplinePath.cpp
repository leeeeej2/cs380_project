#include "SplinePath.h"
const float PI = 4.0f * atan(1.0f);
SplinePath::SplinePath()
{
    scale = glm::vec3(1.f);
    float distance = 10.f / PI;
    glm::vec3 rotate_vector = glm::vec3(0, 1, 0);
    float rotation = 0.f;
    glm::vec3 translate_vector = glm::vec3(distance, 0, 0);
    glm::vec4 transform;

    for (rotation = 0.f; rotation < 2 * PI; rotation += PI / 5)
    {
        transform = glm::rotate(rotation, rotate_vector)* glm::vec4(translate_vector,1.f);
        points.push_back({ transform.x, transform.y, transform.z });
    }
    
    /*points.push_back({ distance, 0, 0 });
    points.push_back({ 0,0,-distance });
    points.push_back({ -distance,0,0 });
    points.push_back({ 0,0,distance });*/
    current_pts = 0;

    CalculationSplinePoint();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * spline_pts.size(), &spline_pts[0].x, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);//spline position 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

SplinePath::~SplinePath()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void SplinePath::DrawSplinePoints(ObjectManager* m, glm::mat4 proj, glm::mat4 cam, glm::vec3 c)
{
    int size = points.size();

    for (int i = 0; i < size; ++i)
    {
        m->SetTransforms(ObjectTypeEnum::eCREATE_SPHERE, points[i], scale, 0, scale);
        if (current_pts == i)
        {
            m->DrawObject(ObjectTypeEnum::eCREATE_SPHERE, proj, cam, { 1,0,0 });
        }
        else
        {
            m->DrawObject(ObjectTypeEnum::eCREATE_SPHERE, proj, cam, c);
        }
    }
}

void SplinePath::DrawSplinePath(GLuint* shader, glm::mat4 transform, glm::mat4 proj, glm::mat4 cam, glm::vec3 color)
{
    GLint uniform_transform = glGetUniformLocation(*shader, "transform");
    GLint uniform_camera = glGetUniformLocation(*shader, "cameraMatrix");
    GLint uniform_ndc = glGetUniformLocation(*shader, "ndcMatrix");
    GLint uniform_color = glGetUniformLocation(*shader, "color");

    //set uniform
    glUniformMatrix4fv(uniform_transform, 1, GL_FALSE, &transform[0][0]);
    glUniformMatrix4fv(uniform_camera, 1, GL_FALSE, &cam[0][0]);
    glUniformMatrix4fv(uniform_ndc, 1, GL_FALSE, &proj[0][0]);
    glUniform3f(uniform_color, color.x, color.y, color.z);

    //update vertex
    UpdateBuffers();

    //set vertex buffer
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glDrawArrays(GL_LINES, 0, (GLsizei)spline_pts.size());
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
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
    points.push_back({ 0,0,0 });
    CalculationSplinePoint();
    UpdateBuffers();
}

void SplinePath::DeleteCurrentSplinePoint()
{
    std::vector<glm::vec3>::iterator it = points.begin() + current_pts;
    --current_pts;
    points.erase(it);
    CalculationSplinePoint();
    UpdateBuffers();
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
    CalculationSplinePoint();
    UpdateBuffers();
}

glm::vec3 SplinePath::GetSplinePoint(float t)
{
    int p0, p1, p2, p3;
    int size = points.size();

    p1 = ((int)t) % size;
    p2 = (p1 + 1) % size;
    p3 = (p1 + 2) % size;

    if (p1 >= 1)
    {
        p0 = p1 - 1;
    }
    else
    {
        p0 = size - 1;
    }

    t = t - (int)t;

    float tt = t * t;
    float ttt = tt * t;

    float q1 = -ttt + 2.f * tt - t;
    float q2 = 3.f * ttt - 5.f * tt + 2.f;
    float q3 = -3.f * ttt + 4.f * tt + t;
    float q4 = ttt - tt;

    float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
    float tz = 0.5f * (points[p0].z * q1 + points[p1].z * q2 + points[p2].z * q3 + points[p3].z * q4);

    return { tx, 0, tz };
}

glm::vec3 SplinePath::GetSplineGradient(float t)
{
    int p0, p1, p2, p3;
    int size = points.size();

    p1 = ((int)t) % size;
    p2 = (p1 + 1) % size;
    p3 = (p1 + 2) % size;

    if (p1 >= 1)
    {
        p0 = p1 - 1;
    }
    else
    {
        p0 = size - 1;
    }

    t = t - (int)t;

    float tt = t * t;
    float ttt = tt * t;

    float q1 = -3.0f * tt + 4.0f * t - 1.0f; 			// -3t^2 + 4t - 1
    float q2 = 9.0f * tt - 10.0f * t;				// 9t^2 - 10t 
    float q3 = -9.0f * tt + 8.0f * t + 1.0f;		// -9t^2 + 8t + 1
    float q4 = 3.0f * tt - 2.0f * t;				// 3t^2 - 2t

    float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
    float tz = 0.5f * (points[p0].z * q1 + points[p1].z * q2 + points[p2].z * q3 + points[p3].z * q4);

    return { tx, 0, tz };
}

void SplinePath::CalculationSplinePoint()
{
    spline_pts.clear();
    float size = (float)points.size();
    float step = points.size() * 5;
    float alpha = 1 / step;

    glm::vec3 pos;
    glm::vec3 previous_pos;

    previous_pos = GetSplinePoint(0);

    for (float t = 0.05f; t < size; t += 0.05f)
    {
        spline_pts.push_back(previous_pos);

        pos = GetSplinePoint(t);
        previous_pos = pos;

        spline_pts.push_back(pos);
    }

}

void SplinePath::UpdateBuffers()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * spline_pts.size(), &spline_pts[0]);
}
