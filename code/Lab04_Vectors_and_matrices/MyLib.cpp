#include <glm/glm.hpp>
#include <cmath>
#include "MyLib.hpp"

float MyLib::length(const glm::vec3 v)
{
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

glm::vec3 MyLib::normalise(const glm::vec3 v)
{
    return v / length(v);
}

float MyLib::dot(const glm::vec3 a, glm::vec3 b)
{
    float aDotb = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    return aDotb;
}

glm::vec3 MyLib::cross(const glm::vec3 a, const glm::vec3 b)
{
    glm::vec3 aCrossb;
    aCrossb[0] = a[1] * b[2] - a[2] * b[1];
    aCrossb[1] = a[2] * b[0] - a[0] * b[2];
    aCrossb[2] = a[0] * b[1] - a[1] * b[0];
    return aCrossb;
}

glm::mat2 MyLib::transpose(const glm::mat2 A)
{
    glm::mat2 AT = glm::mat2(A[0][0], A[1][0], A[0][1], A[1][1]);
    return AT;
}

glm::mat2 MyLib::multiplication(const glm::mat2 A, const glm::mat2 B)
{
    glm::mat2 AB;
    AB[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0];
    AB[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1];
    AB[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0];
    AB[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1];
    return AB;
}

glm::mat2 MyLib::inverse(const glm::mat2 A)
{
    glm::mat2 invA;
    float denominator = A[0][0] * A[1][1] - A[0][1] * A[1][0];
    invA[0][0] = A[1][1] / denominator;
    invA[0][1] = -A[1][0] / denominator;
    invA[1][0] = -A[0][1] / denominator;
    invA[1][1] = A[0][0] / denominator;
    return invA;
}
