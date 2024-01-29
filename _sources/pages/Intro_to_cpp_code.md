# Introduction to C++ code

## `main.cpp`

```cpp
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

/* radions() function has been redefined below
float radians(const float angle)
{
    return angle * pi / 180.0f;
}
*/

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
    std::cout << "\nIf statements\n-------------" << std::endl;
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
    std::cout << "\nFor loop\n--------" << std::endl;
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
    std::cout << "\nWhile loop\n----------\n" << n;

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
    
    std::cout << "\nnumber of elements = " << sizeof(vertices) / sizeof(float) << std::endl;
    
    // Call the power function
    float twoPowerTen = power(2.0f, 10);

    std::cout << "\nFunctions\n---------" << std::endl;
    std::cout << "2^10 = " << twoPowerTen << std::endl;
    
    // Call the radians function
    float angle = angleInDegrees;
//    angle = radians(angle);
    radians(angle);

    std::cout << "\n" << angleInDegrees << " degrees is equal to "<< angle << " radians." << std::endl;
    
    // Define car object
//    Car delorean;
//    delorean.make = "Delorean";
//    delorean.model = "DMC-12";
//    delorean.year = 1981;
    
    std::cout << "\nClasses\n-------" << std::endl;
    Car delorean("DeLorean", "DMC-12", 1981);
    delorean.print_details();
    
    // Call accelerate method
    delorean.accelerate(88.0f);
    
    // Convert speed from mph to kph
    std::cout << "\n" << 88 << " mph is equivalent to " << Car::mph2kph(88.0f) << " kph." << std::endl;
    
    // Exercise Solutions
    std::cout << "\n\nExercises\n---------" << std::endl;
    
    // Exercise 1 - see Student.hpp
    
    // Exercise 2
    std::cout << "\nExercise 2\n----------" << std::endl;
    Student ellie("Ellie", "Williams", 12345678, "Computer Science", 5);
    
    // Exercise 3
    int level4Marks[] = {55, 60, 72, 64};
    int level5Marks[] = {68, 62, 74, 70};
    ellie.addLevelMarks(level4Marks, 4);
    ellie.addLevelMarks(level5Marks, 5);
    
    std::cout << "\nExercise 3\n----------" << std::endl;
    for (int i = 0; i < 12; i++)
    {
        std::cout << ellie.marks[i] << ", ";
    }
    std::cout << std::endl;
    
    // Exercise 4
    std::cout << "\nExercise 4\n----------" << std::endl;
    ellie.outputMarks();
    
    // Exercise 5
    std::cout << "\nExercise 5\n----------" << std::endl;
    std::cout << Student::levelAverage(ellie.marks, 4) << std::endl;
    
    // Exercise 6
    std::cout << "\nExercise 6\n----------" << std::endl;
    ellie.classification();
    
    // Exercise 7
    std::cout << "\nExercise 7\n----------" << std::endl;
    int level6Marks[] = {72, 68, 76, 65};
    ellie.addLevelMarks(level6Marks, 6);
    ellie.outputMarks();
    ellie.classification();
    
    return 0;
}

```

## `Car` class

### `Car.hpp`

```cpp
#pragma once

#include <iostream>

class Car {

public:
    std::string make;
    std::string model;
    int year;
    float speed = 0.0f;

    // Constructor
    Car(const std::string, const std::string, const int);

    // Methods
    void print_details();
    void accelerate(const float);
    static float mph2kph(const float);
};
```

### `Car.cpp`

```cpp
#include <iostream>
#include "Car.hpp"

Car::Car(const std::string makeInput, const std::string modelInput, const int yearInput)
{
    make = makeInput;
    model = modelInput;
    year = yearInput;
    std::cout << "\nCar object created" << std::endl;
}

void Car::print_details()
{
    std::cout << "\nMake: " << make
              << "\nModel: " << model
              << "\nYear: " << year << std::endl;
}

void Car::accelerate(const float increment)
{
    speed = +increment;
    std::cout << "\nThe car has accelerated to " << speed << " mph." << std::endl;
}

float Car::mph2kph(const float speed)
{
    return speed * 1.60934f;
}
```


## Student class

### `Student.hpp`

```cpp
#pragma once

#include <iostream>

class Student {

public:
    std::string firstName;
    std::string lastName;
    int idNumber;
    std::string course;
    int level;
    int marks[12];
    
    // Constructor
    Student(const std::string firstNameInput,
            const std::string lastNameInput,
            const int idNumberInput,
            const std::string courseInput,
            const int levelInput);
    
    // Methods
    void addLevelMarks(const int[], const int);
    void outputMarks();
    void classification();
    static float levelAverage(const int[], const int);
};
```


### `Student.cpp`

```cpp
#include "Student.hpp"
#include <iostream>

Student::Student(
                 const std::string firstNameInput,
                 const std::string lastNameInput,
                 const int idNumberInput,
                 const std::string courseInput,
                 const int levelInput)
{
    firstName = firstNameInput;
    lastName = lastNameInput;
    idNumber = idNumberInput;
    course = courseInput;
    level = levelInput;
    
    std::cout << "\nStudent object " << firstName << " " << lastName << " created." << std::endl;
}

void Student::addLevelMarks(const int marksInput[], const int levelInput)
{
    for (int i = 0; i < 4; i++)
    {
        marks[4 * (levelInput - 4) + i] = marksInput[i];
    }
    
    // Update level if less than levelInput
    if (level < levelInput)
    {
        level = levelInput;
    }
}

void Student::outputMarks()
{
    std::cout << "\n" << firstName << " " << lastName << " (" << idNumber << ")\n" << std::endl;
    
    for (int i = 0; i < level - 3; i++)
    {
        std::cout << "Level " << i + 4 << ": ";
        for (int j = 0; j < 3; j++)
        {
            std::cout << marks[4 * i + j] << ", ";
        }
        std::cout << marks[4 * i + 3] << std::endl;
    }
}

float Student::levelAverage(const int marks[], const int level)
{
    int i = 4 * (level - 4);
    return (marks[i] + marks[i+1] + marks[i+2] + marks[i+3]) / 4.0f;
}

void Student::classification()
{
    int weightedAverage;
    weightedAverage = 0.25 * levelAverage(marks, 5) + 0.75 * levelAverage(marks, 6);
    
    std::cout << "\nClassification: ";
    if (weightedAverage >= 70.0f)
    {
        std::cout << "First-class";
    }
    else if (weightedAverage >= 60.0f)
    {
        std::cout << "Upper second-class";
    }
    else if (weightedAverage >= 50.0f)
    {
        std::cout << "Lower second-class";
    }
    else if (weightedAverage >= 40.0f)
    {
        std::cout << "Third-lcass";
    }
    else
    {
        std::cout << "Fail";
    }
    std::cout << " (weighted average = " << weightedAverage << ")." << std::endl;
}
```