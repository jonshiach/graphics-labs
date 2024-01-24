#include "Student.hpp"
#include <iostream>

Student::Student(
                 const std::string firstNameInput,
                 const std::string lastNameInput,
                 const unsigned int idNumberInput,
                 const std::string courseInput,
                 const unsigned int levelInput)
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

void Student::add_level_marks(const int marksInput[], const int levelInput)
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

void Student::print_marks()
{
    std::cout << "\n" << firstName << " " << lastName << " (" << idNumber << ") marks\n" << std::endl;
    
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

void Student::classification()
{
    float weightedAverge, level5Average, level6Average;
    level5Average = (marks[4] + marks[5] + marks[6] + marks[7]) / 4;
    level6Average = (marks[8] + marks[9] + marks[10] + marks[11]) / 4;
    weightedAverge = 0.25f * level5Average + 0.75f * level6Average;
    
    std::cout << "\nClassification: ";
    if (weightedAverge >= 70.0f)
    {
        std::cout << "First-class" << std::endl;
    }
    else if (weightedAverge >= 60.0f)
    {
        std::cout << "Upper second-class" << std::endl;
    }
    else if (weightedAverge >= 50.0f)
    {
        std::cout << "Lower second-class" << std::endl;
    }
    else if (weightedAverge >= 40.0f)
    {
        std::cout << "Third-lcass" << std::endl;
    }
    else
    {
        std::cout << "Fail" << std::endl;
    }
}
