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
    
    // Constructor
    Student(const std::string firstNameInput,
            const std::string lastNameInput,
            const unsigned int idNumberInput,
            const std::string courseInput,
            const unsigned int levelInput);
    
    // Methods
    void add_level_marks(const int[], const int);
    void print_marks();
    void classification();
};
