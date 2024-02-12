//  Lab04_Vectors_and_matrices

#include <iostream>
#include <cmath>

// Include the glm library
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/io.hpp>

#include "MyLib.hpp"

int main()
{
    std::cout << "Lab 4 Vectors and Matrices\n--------------------------\n" << std::endl;
    
    // Defining a vector
    glm::vec3 u(2.0f, 2.0f, 1.0f);
    
    std::cout << "Defining a vector\n-----------------" << std::endl;
    std::cout << "u = " << u << std::endl;

    // Vector magnitude
//    float lengthU = sqrt(u[0] * u[0] + u[1] * u[1] + u[2] * u[2]);
    float lengthU = glm::length(u);
    
    std::cout << "\nVector magnitude\n----------------" << std::endl;
    std::cout << "|u| = " << lengthU << std::endl;
    
    // Multiplication by a scalar
    glm::vec3 twoU = 2.0f * u;
    float lengthTwoU = glm::length(twoU);

    std::cout << "\nMultiplication by a scalar\n--------------------------" << std::endl;
    std::cout << "2u = " << twoU << std::endl;
    std::cout << "|2u| = " << lengthTwoU << std::endl;
    
    // Unit vectors
    glm::vec3 uHat = u / lengthU;
    float lengthUHat = glm::length(uHat);

    std::cout << "\nUnit vectors\n------------" << std::endl;
    std::cout << "uHat = " << uHat << std::endl;
    std::cout << "|uHat| = " << lengthUHat << std::endl;
    
    // Vector addition and subtraction
    glm::vec3 v(3.0f, 4.0f, 5.0f);
    glm::vec3 uPlusV = u + v;
    glm::vec3 uMinusV = u - v;

    std::cout << "\nVector addition and subtraction\n-------------------------------" << std::endl;
    std::cout << "v = " << v << std::endl;
    std::cout << "u + v = " << uPlusV << std::endl;
    std::cout << "u - v = " << uMinusV << std::endl;
    
    // The dot product
//    float uDotV = u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
    float uDotV = glm::dot(u, v);

    std::cout << "\nThe dot product\n--------------- " << std::endl;
    std::cout << "u . v = " << uDotV << std::endl;
    
    // The cross product
    glm::vec3 uCrossV = glm::cross(u, v);
    float uDotUCrossV = glm::dot(u, uCrossV);
    float vDotUCrossV = glm::dot(v, uCrossV);

    std::cout << "\nThe cross product\n----------------- " << std::endl;
    std::cout << "u x v = " << uCrossV << std::endl;
    std::cout << "u . (u x v) = " << uDotUCrossV << std::endl;
    std::cout << "v . (u x v) = " << vDotUCrossV << std::endl;
    
    // Defining a matrix
    glm::mat2 A = glm::mat2(1.0f, 2.0f, 3.0f, 4.0f);

    std::cout << "\nDefining a matrix\n-----------------" << std::endl;
//    std::cout << "A =" << A << std::endl;
    std::cout << "A = " << glm::transpose(A) << std::endl;
    
    // Matrix indexing
    std::cout << "\nMatrix indexing\n-----------------" << std::endl;
    std::cout << "A[0][0] = " << A[0][0] << std::endl;
    std::cout << "A[0][1] = " << A[0][1] << std::endl;
    std::cout << "A[1][0] = " << A[1][0] << std::endl;
    std::cout << "A[1][1] = " << A[1][1] << std::endl;
    
    // Matrix multiplication
    glm::mat2 B = glm::mat2(5.0f, 6.0f, 7.0f, 8.0f);
    glm::mat2 AB = B * A;   // note the ordering!
    glm::mat2 BA = A * B;

    std::cout << "\nMatrix multiplication\n---------------------" << std::endl;
    std::cout << "B =" << glm::transpose(B) << std::endl;
    std::cout << "\nAB =" << glm::transpose(AB) << std::endl;
    std::cout << "\nBA =" << glm::transpose(BA) << std::endl;
    
    // Identity matrix
    glm::mat2 I = glm::mat2(1.0f);
    glm::mat2 IA = A * I;

    std::cout << "\nIdentity matrix\n---------------" << std::endl;
    std::cout << "I =" << glm::transpose(I) << std::endl;
    std::cout << "\n IA =" << glm::transpose(IA) << std::endl;
    
    // Inverse matrix
    glm::mat2 invA = glm::inverse(A);
    std::cout << "\nInverse matrix\n--------------" << std::endl;
    std::cout << "invA =" << glm::transpose(invA) << std::endl;
    std::cout << "\ninvA * A" << glm::transpose(invA * A) << std::endl;
    
    glm::mat2 C = glm::mat2(1, 2, 3, 6);
    glm::mat2 invC = glm::inverse(C);
    std::cout << "\nC =" <<glm::transpose(C) << std::endl;
    std::cout << "\ninvC =" <<glm::transpose(invC) << std::endl;
    std::cout << "\ndet(C) = " << glm::determinant(C) << std::endl;
    
    // -------------------------------------------------------------------
    // Exercise 1
    glm::vec3 P = glm::vec3(5.0f, 1.0f, 3.0f);
    glm::vec3 Q = glm::vec3(10.0f, 7.0f, 4.0f);
    glm::vec3 R = glm::vec3(0.0f, 5.0f, -3.0f);
    glm::vec3 p = Q - P;
    glm::vec3 q = R - Q;
    glm::vec3 r = P - R;
    
    std::cout << "\nExercise 1\n----------" << std::endl;
    std::cout << "(a) p = " << p << std::endl;
    std::cout << "(b) p = " << q << std::endl;
    std::cout << "(c) p = " << r << std::endl;
    std::cout << "(d) |p| = " << MyLib::length(p) << std::endl;
    std::cout << "(e) pHat = " << MyLib::normalise(q) << std::endl;
    std::cout << "(f) p . q = " << MyLib::dot(p, q) << std::endl;
    std::cout << "(g) q x r = " << MyLib::cross(q, r) << std::endl;
    
    // Exercise 2
    A = glm::mat2(-1.0, 3.0f, 2.0f, -5.0f);
    B = glm::mat2(0.0f, 2.0f, 7.0f, 1.0f);
    C = glm::mat2(3.0f, 2.0f, -3.0f, -4.0f);
    
    std::cout << "\nExercise 2\n----------" << std::endl;
    std::cout << "(a) AB =" << glm::transpose(B * A) << std::endl;
    std::cout << "\n(b) ABC =" << glm::transpose(C * B * A) << std::endl;
    std::cout << "\n(c) CBA =" << glm::transpose(A * B * C) << std::endl;
    std::cout << "\n(d) A^T B =" << glm::transpose(B * glm::transpose(A)) << std::endl;
    std::cout << "\n(e) inv(A) = " << glm::transpose(MyLib::inverse(A)) << std::endl;
    
    // Exercise 3
    std::cout << "\nExercise 3\n----------" << std::endl;
    std::cout << "1(d) |p| = " << MyLib::length(p) << std::endl;
    std::cout << "1(e) pHat = " << q / MyLib::length(q) << std::endl;
    std::cout << "1(f) p . q = " << MyLib::dot(p, q) << std::endl;
    std::cout << "1(g) q x r = " << MyLib::cross(q, r) << std::endl;
    
    std::cout << "\n2(a) AB =" << MyLib::transpose(MyLib::multiplication(A, B)) << std::endl;
    std::cout << "\n2(b) ABC =" << MyLib::transpose(MyLib::multiplication(A, MyLib::multiplication(B, C))) << std::endl;
    std::cout << "\n2(c) CBA =" << MyLib::transpose(MyLib::multiplication(C, MyLib::multiplication(B, A)))  << std::endl;
    std::cout << "\n2(d) A^T B =" << MyLib::transpose(MyLib::multiplication(MyLib::transpose(A), B)) << std::endl;
    std::cout << "\n2(e) inv(A) = " << MyLib::transpose(MyLib::inverse(A)) << std::endl;
    
    return 0;
}
