// Lab01 - Introduction to C++

#include <iostream>

#include "Car.hpp"
#include "Student.hpp"

// Global variables
float pi = 3.1415927f;

float power(const float x, const int y)
{
    float xPowerY = 1.0f;
    for (int i = 0; i < y; i++)
    {
        xPowerY *= x;
    }
    return xPowerY;
}

//float radians(float angle)
//{
//    angle *= pi / 180.0f;
//    return angle;
//}

void radians(float &angle)
{
    angle *= pi / 180;
}

/* Car class now defined in separate header and code files
class Car {
public:
    std::string make;
    std::string model;
    int year;
    float speed = 0.0f;

    // Constructor
    Car(const std::string x, const std::string y, const int z);

    // Methods
    void accelerate(const float increment);
};

// Car class constructor
Car::Car(const std::string x, const std::string y, const int z)
{
    make = x;
    model = y;
    year = z;
    std::cout << "\nCar object created" << std::endl;
}

// Car class methods
void Car::accelerate(const float increment)
{
    speed = +increment;
}
*/

int main()
{
    std::cout << "hello world\n" << std::endl;
    
    // Degrees to radians conversion
    float angleInDegrees = 45.0f;
    float angleInRadians;
//    float pi = 3.1415927f;

    angleInRadians = angleInDegrees * pi / 180.0f;

    // Output result
    std::cout << angleInDegrees << " degrees is equal to "
              << angleInRadians << " radians." << std::endl;
    
    // Determine whether it is acute or obtuse
    std::cout << angleInDegrees << " degrees is an ";
    if (angleInDegrees < 90)
    {
        std::cout << "acute angle." << std::endl;
    }
    else
    {
        std::cout << "obtuse angle." << std::endl;
    }
    
    // Output the first 10 powers of 2
    std::cout << "\nPowers of two\n-------------" << std::endl;
    int powerOfTwo = 1;

    for (int i = 0; i < 10; i++)
    {
        powerOfTwo *= 2;
        std::cout << "2^" << i + 1 << " = " << powerOfTwo << std::endl;
    }
    
    // Count the number of steps the Collatz sequence takes for n = 10 to reach 1
    int n, step;
    n = 10;
    step = 0;
    std::cout << "\nCollatz conjecture\n------------------" << std::endl;
    std::cout << n;

    while (n > 1)
    {
        if (n % 2 == 0)
        {
            n /= 2;
        }
        else
        {
            n = 3 * n + 1;
        }

        step++;
        std::cout << " -> " << n;
    }
    
    std::cout << "\n\nThe Collatz sequence took " << step << " steps to reach 1." << std::endl;
    
    // Arrays
    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    
    std::cout << "\nArrays\n------" << std::endl;

    for (int i = 0; i < 9; i++)
    {
        std::cout << "vertices[" << i << "] = " << vertices[i] << std::endl;
    }
    
    std::cout << "\nsize of vertices = " << sizeof(vertices) << std::endl;
    
    std::cout << "\nnumber of elements = "
              << sizeof(vertices) / sizeof(float) << std::endl;
    
    // Call the power function
    float twoPowerTen = power(2.0f, 10);

    std::cout << "\n2^10 = " << twoPowerTen << std::endl;
    
    // Call the radians function
    float angle = angleInDegrees;
//    angle = radians(angle);
    radians(angle);

    std::cout << "\n" << angleInDegrees << " degrees is equal to "
              << angle << " radians." << std::endl;
    
    // Define car object
//    Car delorean;
//    delorean.make = "Delorean";
//    delorean.model = "DMC-12";
//    delorean.year = 1981;
    Car delorean("DeLorean", "DMC-12", 1981);
    
    delorean.print_details();
    
    // Call accelerate method
    delorean.accelerate(88.0f);
    
    // Exercise Solutions

    // Exercise 2
    std::cout << "\nExercise 2\n----------" << std::endl;
    Student ellie("Ellie", "Williams", 12345678, "Computer Science", 5);
    
    // Exercise 4
    std::cout << "\nExercise 4\n----------" << std::endl;
    int level4Marks[] = {55, 60, 72, 64};
    int level5Marks[] = {68, 62, 74, 70};
    ellie.add_level_marks(level4Marks, 4);
    ellie.add_level_marks(level5Marks, 5);
    
    // Exercise 5
    std::cout << "\nExercise 5\n----------" << std::endl;
    ellie.print_marks();
    
    // Exercise 7
    std::cout << "\nExercise 7\n----------" << std::endl;
    int level6Marks[] = {72, 68, 76, 65};
    ellie.add_level_marks(level6Marks, 6);
    ellie.print_marks();
    ellie.classification();
    
    return 0;
}
