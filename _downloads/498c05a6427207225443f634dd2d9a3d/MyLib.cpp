#include <glm/glm.hpp>
#include <cmath>
#include "MyLib.hpp"

float MyLib::norm(const glm::vec3 vector)
{
    return sqrt(vector[0] + vector[1] + vector[2]);
}

glm::vec3 MyLib::normalize(const glm::vec3 vector)
{
    return vector / MyLib::norm(vector);
}

glm::mat4 MyLib::translate(const glm::mat4 A, const glm::vec3 t)
{
    glm::mat4 translate = glm::mat4(1.0f);
    translate[3][0] = t[0], translate[3][1] = t[1], translate[3][2] = t[2];
    return translate;
}

glm::mat4 MyLib::scale(const glm::mat4 A, const glm::vec3 s)
{
    glm::mat4 scale = glm::mat4(1.0f);
    scale[0][0] = s[0], scale[1][1] = s[1], scale[2][2] = s[2];
    return scale;
}

glm::mat4 MyLib::rotate(const glm::mat4 A, const float angle, const glm::vec3 vector)
{
    glm::vec3 v = MyLib::normalize(vector);
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);
    glm::mat4 rotate = glm::mat4(1.0f);
    rotate[0][0] = v[0] * v[0] * (1 - cosTheta) + cosTheta;
    rotate[0][1] = v[0] * v[1] * (1 - cosTheta) + v[2] * sinTheta;
    rotate[0][2] = v[0] * v[2] * (1 - cosTheta) - v[1] * sinTheta;
    rotate[1][0] = v[1] * v[0] * (1 - cosTheta) - v[2] * sinTheta;
    rotate[1][1] = v[1] * v[1] * (1 - cosTheta) + cosTheta;
    rotate[1][2] = v[1] * v[2] * (1 - cosTheta) + v[0] * sinTheta;
    rotate[2][0] = v[2] * v[0] * (1 - cosTheta) + v[1] * sinTheta;
    rotate[2][1] = v[2] * v[1] * (1 - cosTheta) - v[0] * sinTheta;
    rotate[2][2] = v[2] * v[2] * (1 - cosTheta) + cosTheta;
    return rotate;
}
