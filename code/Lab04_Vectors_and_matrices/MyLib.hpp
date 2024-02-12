# pragma once

#include <iostream>
#include <glm/glm.hpp>

class MyLib
{
public:
    static float length(const glm::vec3);
    static float dot(const glm::vec3, const glm::vec3);
    static glm::vec3 normalise(const glm::vec3);
    static glm::vec3 cross(const glm::vec3, const glm::vec3);
    static glm::mat2 transpose(const glm::mat2);
    static glm::mat2 multiplication(const glm::mat2, const glm::mat2);
    static glm::mat2 inverse(const glm::mat2);
};
