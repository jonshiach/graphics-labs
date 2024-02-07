# pragma once

#include <iostream>
#include <glm/glm.hpp>

class MyLib
{
public:
    static float radians(const float);
    static float norm(const glm::vec3);
    static glm::vec3 normalize(const glm::vec3);
    static glm::mat4 translate(const glm::mat4, const glm::vec3);
    static glm::mat4 scale(const glm::mat4, const glm::vec3);
    static glm::mat4 rotate(const glm::mat4, const float, const glm::vec3);
};
