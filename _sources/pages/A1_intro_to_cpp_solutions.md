# Introduction to C++ Exercises Solutions

## Exercise 1

You are tasked with writing a C++ program to help the university store students' details (name, ID number, course, marks etc.). Create a class called `Student` in a header file which has the following attributes:

   - first name - string;
   - last name - string;
   - ID number - unsigned integer;
   - course - string;
   - level - unsigned integer;
   - marks - 12-element unsigned integer array (marks for four units over the three years);

Solution:

```cpp
#pragma once

#include <iostream>

class Student {

public:
    std::string firstName;
    std::string lastName;
    unsigned int idNumber;
    std::string course;
    unsigned int level;
    unsigned int marks[12];
};
```

## Exercise 2

Create a constructor for your class with parameters for creating an object. Use your constructor to create an object with the following attribute values.

  - first name: Ellie
  - last name: Williams
  - ID number: 12345678
  - course: Computer Science
  - level: 5

Header file

```cpp
// Constructor
Student(std::string firstNameInput,
       std::string lastNameInput,
       unsigned int idNumberInput,
       std::string courseInput,
       unsigned int levelInput);
```

Code file

```cpp
Student::Student(std::string firstNameInput,
        std::string lastNameInput,
        unsigned int idNumberInput,
        std::string courseInput,
        unsigned int levelInput)
{
    firstName = firstNameInput;
    lastName = lastNameInput;
    idNumber = idNumberInput;
    course = courseInput;
    level = levelInput;
    
    std::cout << "\nStudent object " << firstName << " " << lastName << " created." << std::endl;
}
```

## Exercise 3

Modify your constructor so that it checks whether the attributes for an object adhere to the following conditions and prints and error message if not.

  - ID number is an 8-digit number;
  - The course is either "Computer Science" or "Computer Games Development";
  - The level is either 4, 5 or 6.

```cpp
Student::Student(std::string firstNameInput,
        std::string lastNameInput,
        unsigned int idNumberInput,
        std::string courseInput,
        unsigned int levelInput)
{
    // Check ID number
    if (idNumberInput < 10000000 || idNumberInput > 99999999)
    {
        std::cout << "Error::Student::ID number needs to be an 8 digit integer." << std::endl;
        return;
    }
    
    // Check the course
    if (courseInput != "Computer Science" && courseInput != "Computer Games Development")
    {
        std::cout << "Error::Student::Course needs to be either 'Computer Science' or 'Computer Games Development'." << std::endl;
        return;
    }
    
    // Check the level
    if (levelInput < 4 || levelInput > 6)
    {
        std::cout << "Error::Student::Level needs to be one of 4, 5 or 6." << std::endl;
        return;
    }
    
    firstName = firstNameInput;
    lastName = lastNameInput;
    idNumber = idNumberInput;
    course = courseInput;
    level = levelInput;
    
    std::cout << "\nStudent object " << firstName << " " << lastName << " created." << std::endl;
}
```