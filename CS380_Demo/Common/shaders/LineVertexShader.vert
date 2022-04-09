#version 410 core

in layout(location = 0) vec3 position;

uniform mat4 transform;
uniform mat4 cameraMatrix;
uniform mat4 ndcMatrix;

void main()
{
    gl_Position = ndcMatrix*inverse(cameraMatrix) * transform * vec4(position, 1.0);
}