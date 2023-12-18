# Introduction to C++

In this first lab we will be introducing/revisiting the fundamentals of the C++ language.

## IDEs

A C++ program is simply a set of plain text files that we instruct a compiler to convert to machine code and produce an executable. As long as you have installed the compiler and you have a text editor program you get program away to your hearts content. However, as your programs start to become increasingly sophisticated and you experience syntax errors and bugs, you will start to yearn for a better way. Fortunately there is and we can use an Integrated Development Environment (IDE).

An IDE provides a way of writing and organising your code files as well as a number of useful tools for debugging and organising your code projects. There are a number of IDEs available for working with C++ but we will look at the most common ones for Windows and macOS which are Visual Studio and Xcode respectively (I haven't covered Linux here but I figure if you prefer to work with Linux you are able to find a suitable IDE yourself).

---

### Visual Studio (Windows)

The latest version of Visual Studio can be installed via the App Store and this has been installed on PCs in the Dalton Building.

1. Open **Visual Studio** and click on 'Create a new project' (or File > New > New Project if you are already in Visual Studio). Select **Empty Project** and click on **Next**.

2. Give your project a suitable name like **lab00\_intro\_to\_cpp**, select a directory in your OneDrive area and click on **Create**.
   This will create a folder with the name of your project within which there will be a file with the extension **.sln** (along with some other files). This is your project file and double clicking on this will open your project in Visual Studio.

3. We need to add a file which will contain code to our project. Right-click on **Source Files** in the Solution Explorer and **Add > New Item...** Make sure **C++ File (.cpp)** is highlighted, give the file a suitable name like **main.cpp** and click **Add**.

### Xcode (macOS)

The latest version of Xcode can be installed via the Mac App Store.

1. Open Xcode and click on **Create a new Xcode project** (or if you are already in Xcode click File > New > Project or press ⇧⌘N), choose **Command Line Tool** and click **Next**.

2. Give your project a suitable name like **lab00\_intro\_to\_cpp** and click **Next**, select a directory in your OneDrive area and click on **Create**.

    This will create a folder with the name of your project within which there will be a file with the extension **.xcodeproj**. This is your project file and double clicking on this will open your project in Xcode.

3. Xcode has already given you a **main.cpp** file with a basic hello world example program.

---

## C++ basics

### hello world

We will start with the classic hello world example. In your **.cpp** source file, enter the following code.

```cpp
// Lab01 - Introduction to C++

#include <iostream>

int main() 
{
    std::cout << "hello world\n" << std::endl;
    return 0;
}
```

Compile and run the code by pressing the F5 (Visual Studio) key or ⌘R in Xcode. If you do not have any syntax errors a command window should appear with the following output.

```text
hello world
```

Lets run through the code and see what each line does

| <div style="width:250px">Code</div> | Explanation |
|:--|:--|
|`// Lab01 - Introduction to C++` | Any text between `//` and the end of the current line is a comment and is ignored by the compiler. For multiline comments, any text between ``/*`` and `*/` is considered a comment. |
| `#include <iostream>` | Imports the `iostream` library allowing us to input and output data to the terminal. |
| `int main() {}` | Defines a [function](functions-section) called `main` which has no inputs and will output an integer value. All C++ programs must contain a function called `main` which is the designated entry point to a program. |
| `std::cout` | Calls the `cout` (pronounced "see-out") object from the `iostream` library. This is used to output text. |
| `<< "hello world\n"` | Sends the text string `hello world` to the `cout` object for printing. `\n`is the newline character which inserts a newline. |
| `<< std::endl;` | Sends the end line character to the `cout` object so the line is printed. Note that every C++ statement must end in a semi-colon `;` (get used to doing this). |
| `return 0;` | Returns the integer `0`. This is because in the function definition we specified that the function `main` would return a `0`. The convention is that a `0` is returned if the program has executed successfully and a `1` is returned if not. |

### Variables

A variable is a portion of memory used to store a value. C++ uses the following types of variables.

| Type | Explanation |
|:---- |:------------|
| `int` | integers (whole numbers), e.g., 1.23 |
| `float` | floating point numbers, e.g., 1.23. |
| `char` | single character, e.g., 'a' (character values are surrounded by single quotes). |
| `string` | text consisting of multiple characters, e.g., "hello world" (strings are surrounded by double quotes). |
| `bool` | Boolean values, e.g., true or false. |

A variable must be identified by a unique names which are called **identifiers**. An identifier can be a sequence of one or more letters, digits and underscores but must begin with a character and cannot contain spaces, punctuation marks and symbols. As long as you don't use an identifier that conflicts with a C++ keyword (e.g., `char`) you are free to choose whatever identifier you wish. It is good practice to use descriptive identifiers, e.g., `position`, and where an identifier contains multiple words is it standard practice for the first letter of each additional word to be an uppercase character, e.g., `framesPerSecond`.

Let's write a simple program which converts an angle from degrees to radians (a <a href="https://en.wikipedia.org/wiki/Radian" target="_blank">radian</a> is an angle measure where 1 radian is the angle subtended at the centre of a circle by an arc equal to the radius (it is the preferred unit of measurement when working with angles). Add the following code to your program.

```cpp
// Degrees to radians conversion
float angleInDegrees = 45;
float angleInRadians;
float pi = 3.1415927f;

angleInRadians = angleInDegrees * pi / 180;

// Output result
std::cout << angleInDegrees << " degrees is equal to "
          << angleInRadians << " radians." << std::endl;
```

Output:

```text
45 degrees is equal to 0.785398 radians.
45 degrees is an acute angle.
```

When working with variables we first need to define the variable types. For example `float angleInDegrees;` defined a variable with the identifier `angleInDegrees` as a float. We could then store the value inputted by the user into this variable using the `std::cin >>` command.

````{note}
When declaring float variables we should use the syntax `1.0f` instead of `1`. For example

```cpp
float myVariable = 1.0f;
```

This is because is we were to use `float myVariable = 1;` the compiler would need to generate extra code to convert from an integer value to a floating point value.
````

### If statements

The if-elseif-else statement in C++ uses the syntax

```cpp
if (condition1) 
{
    // code to be executed if condition1 is true
} 
else if (condition2) 
{
    // code to be executed if condition1 is false and condition2 is true
} 
else
{
    // code to be executed if both condition1 and condition2 are false
}
```

Let add the following code to our program to determine whether an angle is acute ($<90^\circ$) or obtuse ($\geq 90^\circ$).

```cpp
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
```

Output:

```text
100 degrees is an obtuse angle.
```

### For loop

A for loop is used to repeat a set of commands when we know beforehand how many times we want it repeating. The C++ syntax for a for loop is as follows.

```cpp
for (before statement ; condition ; after statement) 
{
    // block of code to be executed
}
```

The `before statement` contains code to be executed before the for loop, the `condition` is a logical check to see whether code within the for loop is executed and the `after statement` is executed after the code within the for loop.

Lets add a for loop to our program to output the first 10 powers of 2.

```cpp
// Output the first 10 powers of 2
std::cout << "\nPowers of two\n-------------" << std::endl;
int powerOfTwo = 1;

for (int i = 0; i < 10; i++) 
{
    powerOfTwo *= 2;
    std::cout << "2^" << i + 1 << " = " << powerOfTwo << std::endl;
}
```

Output:

```text
Powers of two
-------------
2^1 = 2
2^2 = 4
2^3 = 8
2^4 = 16
2^5 = 32
2^6 = 64
2^7 = 128
2^8 = 256
2^9 = 512
2^10 = 1024
```

### While loop

A while loop is used to repeat a block of code as long as a specified condition(s) is reached and used when we do not know how many times a block of code is repeated (if we did a for loop would be used). The C++ syntax for a while loop is as follows.

```cpp
while (condition) 
{
    // block of code to be executed
}
```

Lets add a while loop to our program calculate how many times the following expression is calculated until we get $n = 1$ (known as the <a href="https://en.wikipedia.org/wiki/Collatz_conjecture" target="_blank">Collatz conjecture</a>).

$$ n = \begin{cases} n / 2, & \text{if $n$ is even}, \\ 3n + 1, & \text{if $n$ is odd}.\end{cases} $$

```cpp
// Output the steps of the Collatz conjecture for n = 10
int n, step;
n = 10;
step = 0;
std::cout << "\nCollatz conjecture\n------------------" << std::endl;
std::cout << "0 : " << n << std::endl;

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
    std::cout << step << " : " << n << std::endl;
}
```

Output:

```text
Collatz conjecture
------------------
0 : 10
1 : 5
2 : 16
3 : 8
4 : 4
5 : 2
6 : 1
```

---

### Arrays

An **array** is a collection of multiple values of the same type that can be stored in a single variable. For example, we could store the co-ordinates of multiple points in an array and access all of the co-ordinates at once instead of creating separate variables for each one. The values of an array are stored in consecutive memory locations. The C++ syntax for declaring an array is to use square brackets after the array name, the values of each element in the array is then written in curly brackets. For example, define an array called `indices` with the following.

```cpp
// Arrays
float vertices[] = 
{
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f, 
     0.0f,  0.5f, 0.0f
};
```

To access individual **elements** in an array we specify the **index** which is a positive integer value where the first element has the index 0, the second has the index 1 and so on. For example, add the following code to your program to print the elements of the `vertices` array.

```cpp
std::cout << "\nArrays\n------" << std::endl;

for (int i = 0; i < 9; i++)
{
    std::cout << "vertices[" << i << "] = " << vertices[i] << std::endl;
}
```

Output:

```text
Arrays
------
vertices[0] = -0.5
vertices[1] = -0.5
vertices[2] = 0
vertices[3] = 0.5
vertices[4] = -0.5
vertices[5] = 0
vertices[6] = 0
vertices[7] = 0.5
vertices[8] = 0
```

The size of the memory used to store an array can be determined using the `sizeof` command. Add the following to your program to output the number of bytes used to story the `vertices` array.

```cpp
std::cout << "\nsize of vertices = " << sizeof(vertices) << std::endl;
```

Output:

```text
size of vertices = 36
```

So 36 bytes are used to store the 9-element array `vertices`, therefore each element takes up 4 bytes because they are floats. We can also use the `sizeof` command to return the number of bytes used to store different variable types. So a useful way to determine the number of elements in an array is to divide the memory used to store the array by the memory used for each element. For example, add the following to your program.

```cpp
std::cout << "\nnumber of elements = " << sizeof(vertices) / sizeof(float) << std::endl;
```

Output:

```text
number of elements = 9
```

---

(functions-section)=

## Functions

A **function** is a block of code that is used to perform a (usually) single action. They are useful for when we need to use similar code multiple times, instead of writing the code each time, we can define a function just once and call it to perform the action. You've already declared the `main` function which all C++ programs must have so lets add another function that raises a floating point number to a power. Add the following code before ```int main()```

```cpp
float power(const float x, const int y)
{
    float xPowerY = 1;
    for (int i = 0; i < y; i++)
    {
        xPowerY *= x;
    }
    return xPowerY;
}
```

Here we have declared the function called `power` that takes in an floating point variable `x` and an integer variable `y` and outputs a floating point variable. We can call the function to calculate $\text{2}^\text{10}$ by adding the following inside the `main()` function.

```cpp
// Call the power function
float twoPowerTen = power(2.0f, 10);

std::cout << "\n2^10 = " << twoPowerTen << std::endl;
```

Output:

```text
2^10 = 2048
```

### Function parameters

A function **parameter** is information that is passed into a function, for example, in our function above we passed in the parameters `x` and `y`. We are not changing the values of these parameters so we have declared them as constants using `const`.  The code within a function only knows about the information passed into it via its input parameters and any **global variables** that are declared outside of all functions. Lets write a function that converts an angle in degrees to radians

```cpp
float radians(float angle)
{
    angle *= pi / 180;
    return angle;
}
```

Here our function called `radians` converts the float parameter `angle` to radians by multiplying it by $\pi / 180$ and returns it (we haven't declared the `angle` parameter as a constant since we are changing its value). If you attempt to compile your program you will get a compiler error that `pi` is undefined. This is because `pi` is a **local variable** for the `main` function. We can correct this using the following:

- declare `pi` to be a local variable within the `radians` function (preferred if we are unlikely to be using `pi` elsewhere in the program);
- declare `pi` to be a global variable outside of all functions (preferred if `pi` is likely to be used elsewhere in the program);
- delcare `pi` to be a parameter of the `radians` function.

Fix your program by defining `pi` to be a global variable so that it compiles. Call your function using the following code.

```cpp
// Call the radians function
float angle = angleInDegrees;
angleInRadians = radians(angle);

std::cout << angle << " degrees is equal to " << angleInRadians << " radians." << std::endl;
```

### Pass by reference

When we want a function to change the value of the parameters that are passed to the function we can pass a **reference** to the function. This is done using an ampersand `&` before the name of the parameter. Change your `radians` function above to the following.

```cpp
void radians(float &angle)
{
    angle *= pi / 180;
}
```

Here we have passed the reference to the `angle` variable (the memory address of the variable) so the code within the function changes its value. We do not need to return anything from the function so we remove the `return` command and change the output type of the function to `void`. We now need to change how we call the `radians` function.

```cpp
// Call the radians function
float angle = angleInDegrees;
radians(angle);

std::cout << angleInDegrees << " degrees is equal to " 
          << angle << " radians." << std::endl;
```

---

## Object orientated programming

The main difference between C++ and its predecessor C is that C++ has **classes** which allow us to use <a href="https://en.wikipedia.org/wiki/Object-oriented_programming" target="_blank">**Object Orientated Programming (OOP)**</a>. Procedural programming requires use to write code that performs operations on data whereas object orientated programming allows us to create objects that include both data and code. Object orientated programming makes code easier to write and modify and is often faster than procedural programming.

### Classes/objects

A **class** defines the **attributes** (like variables) and **methods** (like functions) that the objects within the class will possess. For example, lets create `Car` class which has the attributes for the `make`, `model`, `year` and `speed` of a car. The methods that apply to objects in our class may include `accelerate()`, `brake()` etc.

Add the following code to your program outside of any functions.

```cpp
class Car {
public:
    std::string make;
    std::string model;
    int year;
    float speed = 0.0f;
};
```

|  <div style="width:150px">Code</div> | Explanation |
|:--|:--|
| `class Car` | Keyword used to create the class `Car`. |
| `public:` | An **access specifier** that specifies which attributes and methods can be accessed outside of the class. |
| `std::string make;` | Defines the attribute `make` which is a string. The other attributes are the string `model`, the integer `year` and the floating point number `speed` which is initially set to zero for any new object which is created. |

We can then create an object from our `Car` class and set its attributes. Lets create an object called `carObject` for a 1989 Toyota MR2. In your `main()` function add the following code.

```cpp
// Define car object
Car carObject;
carObject.make = "Toyota";
carObject.model = "MR2";
carObject.year = 1989;
```

Here to create the object we specified the class name and the name of our object, e.g., `Car carObject;`. Then we define the car attributes using the name of the object followed by the name of the attribute, e.g., `carObject.make`. Lets print the values of the attributes.

```cpp
std::cout << "\nMake: " << carObject.make
          << "\nModel: " << carObject.model
          << "\nYear: " << carObject.year << std::endl;
```

Output:

```text
Make: Toyota
Model: MR2
Year: 1989
```

### Class methods

The methods for a class a functions that the objects in the class share. For our `Car` class lets define a method for accelerating the car. Inside the class definition add the following code.

```cpp
class Car {
public:
    std::string make;
    std::string model;
    int year;
    float speed = 0.0f;

    // Methods
    void accelerate(const float increment);
};
```

This only declares that the class `Car` has a function called `accelerate()` which has 1 input parameter `increment`. We need to define what the function does. Add the following code outside of the class.

```cpp
void Car::accelerate(const float increment)
{
    speed = +increment;
}
```

Here the name of the method (`accelerate`) follows the name of the class (`Car`) and the `::` operator. We can now call the method by adding the following code to our `main()` function.

```cpp
// Call accelerate method
std::cout << "\n\nSpeed before acceleration: " << carObject.speed << std::endl;

carObject.accelerate(50.0f);

std::cout << "\nSpeed after acceleration: " << carObject.speed << std::endl;
```

### Constructors

A **constructor** is a special method that is automatically called when an object of a class is created. A constructor has the same name as that of the class, so for our `Car` class we declare the constructor using the following code

```cpp
class Car {
public:
    std::string make;
    std::string model;
    int year;
    float speed = 0.0f;

    // Constructor
    Car();

    // Methods
    void accelerate(const float increment);
};
```

and define the constructor so that a message is printed to the terminal

```cpp
Car::Car() 
{
    std::cout << "\nCar object created" << std::endl;
}
```

Now when we create our `carObject` object the following is outputted

```text
Car object created

Make: Toyota
Model: MR2
Year: 1989
```

Constructors are more useful when we uses parameters to set the values of the class attributes. Modify the constructor declaration to include the parameters

```cpp
class Car {
public:
    std::string make;
    std::string model;
    int year;
    float speed = 0.0f;

    // Constructor
    Car(const std::string w, const std::string x, const int y);

    // Methods
    void accelerate(const float increment);
};
```

and modify the constructor definition so that the attributes are specified.

```cpp
Car::Car(const std::string w, const std::string x, const int y)
{
    make = x;
    model = y;
    year = z;
    std::cout << "\nCar object created" << std::endl;
}
```

Now we can define the object attributes by creating the object using the following code.

```cpp
Car carObject("Toyota", "MR2", 1989);
```

### Header files

When dealing with larger programs and larger classes is become necessary to split the code over multiple files. Classes are declared in a **header file** which usually have the extension **.hpp** (this isn't a requirement but has become standard practice in C++ programming). The methods of a class are then defined in a separate source files which have the extension **.cpp**.

Lets create a header file for our `Car` class.

1. Right-click on the project name in the **Solution Explorer** and the select **Add > New Item...** (or press Ctrl+Shift+A).
2. Select **header File (.h)** and click on **Add**. 
3. This creates the file **Header.h** in the **Header files** filter. Right-click on this and select **Rename** (or select it and press F2) and change the filename to **Car.hpp**.

The header file we have created is an empty files with a single line `#pragma once`. This is used to prevent multiple header files of the same name from being included. An alternative way of doing this inherited from C programming is to use <a href="https://en.wikipedia.org/wiki/Include_guard" target="_blank">include guards</a> which for our `Car` class would require the following.

```cpp
#ifndef CAR_H
#define CAR_H

// Contents of the header file

#endif
```

Copy the `Car` class into our `Car.hpp` header file so that it looks like the following.

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
    Car(std::string x, std::string y, int z);

    // Methods
    void accelerate(const float increment);
};
```

Note that we also need the `#include <iostream>` library so we can use strings and input/output commands.

The methods are defined in a separate source file. Add a new C++ file to your project and cut and paste your `Car` class methods from `main.cpp` and edit it so it looks like the following.

```cpp
#include <iostream>
#include "Car.hpp"

Car::Car(const std::string x, const std::string y, const int z)
{
    make = x;
    model = y;
    year = z;
    std::cout << "\nCar object created" << std::endl;
}

void Car::accelerate(const float increment)
{
    speed = +increment;
}
```

Here we have also included the `iostream` library as well as our `Car.hpp` header file. We also need to make sure we include the `Car.hpp` header file in `main.cpp`. Compile and run the program to check everything works ok.

---

## Exercises

1. 
