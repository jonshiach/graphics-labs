(vectors-and-matrices-section)=

# Vectors and Matrices
 
Computer graphics relies heavily on mathematics of vectors and matrices. In this lab we will be revising these and show how we can perform calculations in C++.

## Vectors

A vector in is an object with magnitude (length) and direction. A vector is denoted by a lower case letter in boldface, e.g., $\vec{a}$ and represented mathematically by the 3-tuple

$$ \vec{a} = (a_x, a_y, a_z), $$

where $a_x$, $a_y$ and $a_z$ are the lengths of the vector in the $x$, $y$, and $z$ directions.

```{figure} ../images/04_Vector.svg
:height: 350

The vector $\vec{a} = (a_x, a_y, a_z)$.
```

We can use vectors to represent the displacement of one point to another. For example, given two points $A$ and $B$ the displacement from $A$ to $B$ can be thought of as the vector $\vec{a} = B - A$.

```{figure} ../images/04_A_to_B_vector.svg
:width: 250

The vector $\vec{a} = B - A$.
```

### The glm library

The glm (<a href="https://github.com/g-truc/glm" target="_blank">OpenGL Mathematics</a>) library is a popular C++ mathematics library designed to provide classes and functions for mathematical operations, specifically tailored for graphics programming using OpenGL. We will be using functions from glm to perform calculations for us. First download and build the project files for this lab.

1. Go to <a href="https://github.com/jonshiach/Lab04_Vectors_and_matrices" target="_blank">https://github.com/jonshiach/Lab04_Vectors_and_matrices</a> and follow the instructions to download and build the project files.
2. Open the project file `Lab04_Vectors_and_matrices.sln` (or `Lab04_Vectors_and_matrices.xcodeproj` on macOS) set the **Lab04_Vectors_and_matrices** project as the startup project.
    - Visual Studio: right-click on the **Lab04_Vectors_and_matrices** project and select **Set as Startup Project**.
    - Xcode: Click on the target select dropdown (to the right of the name of the project at the top of the window) and select **Lab04_Vectors_and_matrices** as the target.

3. Build the project by pressing CTRL + B (or ⌘B on Xcode) which should build the project without errors. Run the executable by pressing F5 (or ⌘R on Xcode).

Take a look the `main.cpp` file in the `source/` folder.

```cpp
#include <iostream>
#include <cmath>

// Include the glm library
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/io.hpp>

int main()
{
    std::cout << "Lab 4 Vectors and Matrices\n--------------------------\n" << std::endl;
    
    // Defining a vector
    glm::vec3 u = glm::vec3(2.0f, 2.0f, 1.0f);
    
    std::cout << "Defining a vector\n-----------------" << std::endl;
    std::cout << "u = " << u << std::endl;
    
    return 0;
}

```

The output should look like the following.

```text
Defining a vector
-----------------
u = [    2.000,    2.000,    1.000]
```

Taking a look at the code we see that the `glm::vec3 u = glm::vec3(2.0f, 2.0f, 1.0f);` command defines the 3-element vector `u` with the values of the elements (2, 2, 1). This vector is then output to the console using the `std::cout` command.

(vector-magnitude-section)=

### Vector magnitude


The **magnitude** of a vector $\vec{a} = (a_x, a_y, a_z)$ is denoted by $|\vec{a}|$ is the length from the tail of the vector to the head.

```{figure} ../images/04_Vector_magnitude.svg
:height: 100

The magnitude of the vector $\vec{a}$.
``` 

The magnitude is calculated using an extension of Pythagoras' theorem

$$ |\vec{a}| = \sqrt{a_x^2 + a_y^2 + a_z^2}. $$(eq:vector-magnitude)

For example, the magnitude of the vector $\vec{u} = (2, 2, 1)$ is 

$$ | \vec{u} | = \sqrt{2^2 + 2^2 + 1^2} = \sqrt{4 + 4 + 1} = \sqrt{9} = 3. $$

Let's do this calculating in our program, add the following code.

```cpp
// Vector magnitude
float lengthU = sqrt(u[0] * u[0] + u[1] * u[1] + u[2] * u[2]);

std::cout << "\nVector magnitude\n----------------" << std::endl;
std::cout << "|u| = " << lengthU << std::endl;
```

Here we have calculated the magnitude of the vector `u` using `lengthU = sqrt(u[0] * u[0] + u[1] * u[1] + u[2] * u[2])`. The `sqrt()` function is from the `cmath` library and calculates the square root of a number.

The output of the program is shown below.

```text
Vector magnitude
----------------
|u| = 3
```

Calculating the magnitude of a vector is a very common calculation so shouldn't we write a function to do this? Fortunately there is a glm function `glm:length()` to do this for us, edit your program so it looks like the following to use the glm function instead.

```cpp
// float lengthU = sqrt(u[0] * u[0] + u[1] * u[1] + u[2] * u[2]);
float lengthU = glm::length(u);
```

That makes things much easier.

### Vector multiplication by a scalar

The multiplication of a vector $\vec{a} = (a_x, a_y, a_z)$ by a scalar (another way of saying a number) $k$ is calculated using

$$ k \vec{a} = (ka_x, ka_y, ka_z) $$(eq:vector-multiplication)

Multiplying a vector by a positive scalar has the effect of scaling the length of the vector. Multiplying by a negative scalar reverses the direction of the vector.

```{figure} ../images/04_vector_multiplication.svg
:height: 180
```

For example, multiplying the vector $\vec{u} = (2, 2, 1)$ by the scalar 2 gives

$$ 2\vec{u} = 2(2,2,1) = (4, 4, 2), $$

which has the magnitude

$$ |2 \vec{u} | = \sqrt{4^2 + 4^2 + 2^2} = \sqrt{36} = 6 = 2 |\vec{u}|. $$

To multiply a glm vector by a scalar we simply using the standard `*` operator. Add the following to your code to check it returns the correct vector.

Add the following code to your program.

```cpp
 // Multiplication by a scalar
 glm::vec3 twoU = 2.0f * u;
 float lengthTwoU = glm::length(twoU);

 std::cout << "\nMultiplication by a scalar\n--------------------------" << std::endl;
 std::cout << "2u = " << twoU << std::endl;
 std::cout << "|2u| = " << lengthTwoU << std::endl;
```

Output

```text
Multiplication by a scalar
--------------------------
2u = [    4.000,    4.000,    2.000]
|2u| = 6
```

(unit-vectors-section)= 

### Unit vectors

A unit vector is a vector that has a magnitude of 1. We can find a unit vector that points in the same direction as a non-zero vector $\vec{a}$, which is denoted by $\hat{\vec{a}}$ (pronounced "a-hat"), by dividing by its magnitude, i.e.,

$$ \hat{\vec{a}} = \frac{\vec{a}}{|\vec{a}|}. $$(eq:unit-vector)

This process is callde **normalising a vector**. For example, to determine a unit vector pointing in the same direction as $\vec{u} = (2, 2, 1)$ we normalise it by dividing by its magnitude which is 3

$$ \begin{align*}
    \hat{\vec{u}} &= \frac{(2, 2, 1)}{3} = \left( \frac{2}{3}, \frac{2}{3}, \frac{1}{3} \right) \approx (0.67, 0.67, 0.33).
\end{align*} $$

Checking that $\hat{\vec{u}}$ has a magnitude of 1

$$ |\hat{\vec{u}}| = \sqrt{\left(\frac{2}{3}\right)^2 + \left(\frac{2}{3}\right)^2 + \left(\frac{1}{3}\right)^2} = \sqrt{\frac{4}{9} + \frac{4}{9} + \frac{1}{9}} = \sqrt{1} = 1. $$

Add the following to your program

```cpp
// Unit vectors
glm::vec3 uHat = u / lengthU;

std::cout << "\nUnit vectors\n------------" << std::endl;
std::cout << "uHat = " << uHat << std::endl;
std::cout << "|uHat| = " << glm::length(uHat) << std::endl;
```

Output

```text
Unit vectors
------------
uHat = [    0.667,    0.667,    0.333]
|uHat| = 1
```

The glm function `glm::normalize(u)` normalises the vector `u`. Edit your program so its uses this function to calculate `uHat` and you should get the same result.

```cpp
glm::vec3 uHat = glm::normalize(u);
```

### Vector addition and subtraction

The addition and subtraction of two vectors $\vec{a} = (a_x, a_y, a_z)$ and $\vec{b} = (b_x, b_y, b_z)$ is defined by

$$ \vec{a} \pm \vec{b} = (a_x \pm b_x, a_y \pm b_y, a_z \pm b_z). $$(eq:vector-addition)

For example, given the vectors $\vec{u} = (2,2,1)$ and $\vec{v} = (3, 4, 5)$

$$ \begin{align*}
    \vec{u} + \vec{v} &= (2, 2, 1) + (3, 4, 5) = (2 + 3, 2 + 4, 1 + 5) = (5, 6, 6), \\
    \vec{u} - \vec{v} &= (2, 2, 1) - (3, 4, 5) = (2 - 3, 2 - 4, 1 - 5) = (-1, -2, -4).
\end{align*} $$

The standard `+` and `-` operators are used to add and subtract glm vectors. Add the following code to your program.

```cpp
// Vector addition and subtraction
glm::vec3 v(3.0f, 4.0f, 5.0f);
glm::vec3 uPlusV = u + v;
glm::vec3 uMinusV = u - v;

std::cout << "\nVector addition and subtraction\n-------------------------------" << std::endl;
std::cout << "v = " << v << std::endl;
std::cout << "u + v = " << uPlusV << std::endl;
std::cout << "u - v = " << uMinusV << std::endl;
 ```

Output

```text
Vector addition and subtraction
-------------------------------
v = [    3.000,    4.000,    5.000]
u + v = [    5.000,    6.000,    6.000]
u - v = [   -1.000,   -2.000,   -4.000]
```

Its important to understand what is happening in a geometrical sense when we add and subtract vectors. Take a look at {numref}`vector-addition-figure`, here the vector $\vec{b}$ has been added to the vector $\vec{a}$ where the tail of $\vec{b}$ is placed at the head of $\vec{a}$. The resulting vector $\vec{a} + \vec{b}$ points from the tail of $\vec{a}$ to the head of $\vec{b}$. 

With the subtraction of the vector $\vec{b}$ we do similar but instead multiply $\vec{b}$ by -1 thereby reversing its direction.

```{figure} ../images/04_Vector_addition.svg
:height: 180
:name: vector-addition-figure

Vector addition and subtraction.
```

(dot-product-section)= 

### Dot product

The **dot product** between two vectors $\vec{a} = (a_x, a_y, a_z)$ and $\vec{b} = (b_x, b_y, b_z)$ is denoted by $\vec{a} \cdot \vec{b}$ and returns a scalar quantity (a number). The dot product is calculated using

$$ \vec{a} \cdot \vec{b} = a_xb_x + a_yb_y + a_zb_z. $$(eq:dot-product)

The dot product is related to the angle between the two vectors by

$$ \vec{a} \cdot \vec{b} = |\vec{a}| |\vec{b}| \cos(\theta), $$(eq:dot-product-geometric)

where $\theta$ is the angle between the vectors $\vec{a}$ and $\vec{b}$.

```{figure} ../images/04_Dot_product.svg
:height: 125
```

A useful result for computer graphics is that if $\theta=90^\circ$ then $\cos(90^\circ)= 0$ and equation {eq}`eq:dot-product-geometric` becomes

$$ \vec{a} \cdot \vec{b} = 0. $$

In order words, if the dot product of two vectors is zero then the two vectors are perpendicular.

For example, given the vectors $\vec{u} = (2,2,1)$ and $\vec{v} = (3, 4, 5)$ the dot product $\vec{u} \cdot \vec{v}$ is

$$ \begin{align*}
    \vec{u} \cdot \vec{v} &= (2, 2, 1) \cdot (3, 4, 5) \\
    &= 2 \times 3 + 2 \times 4 + 1 \times 5 \\
    &= 6 + 8 + 5 \\
    &= 19.
\end{align*} $$

Add the following to your program.

```cpp
// The dot product
float uDotV = u[0] * v[0] + u[1] * v[1] + u[2] * v[2];

std::cout << "\nThe dot product\n--------------- " << std::endl;
std::cout << "u . v = " << uDotV << std::endl;
 ```

Output

```text
The dot product
--------------- 
u . v = 19
```

The glm function `glm::dot(u, v)` calculates the dot product of the two vectors `u` and `v`. Use this to calculate $\vec{u} \cdot \vec{v}$ in your program to see that it gives the same result. 

(cross-product-section)=

### Cross product

The **cross product** between two vectors $\vec{a} = (a_x, a_y, a_z)$ and $\vec{b} = (b_x, b_y, b_z)$ is denoted by $\vec{a} \times \vec{b}$ and returns a vector. The cross product is calculated using

$$ \vec{a} \times \vec{b} = (a_yb_z - a_zb_y, a_zb_x - a_xb_z, a_xb_y - a_yb_x). $$(eq:cross-product)

The cross product between two vectors produces another vector that is perpendicular to both vectors. This is another incredibly useful result as it allows use to calculate [**normal vectors**](normal-vector-section) for polygons which are used in calculating how light is reflected off surfaces.

```{figure} ../images/04_cross_product.svg
:height: 200
```

For example, given the vectors $\vec{u} = (2,2,1)$ and $\vec{v} = (3, 4, 5)$ the cross product $\vec{u} \times \vec{v}$ is

$$ \begin{align*}
    \vec{u} \times \vec{v} &= (2, 2, 1) \times (3, 4, 5) \\
    &= (2 \times 5 - 1 \times 4, 1 \times 3 - 2 \times 5, 2 \times 4 - 2 \times 3) \\
    &= (6, -7, 2).
\end{align*} $$

Show that $\vec{u} \times \vec{v}$ is perpendicular to both $\vec{u}$ and $\vec{v}$

$$ \begin{align*}
    \vec{u} \cdot (\vec{u} \times \vec{v}) &= (2, 2, 1) \cdot (6, -7, 2) \\
    &= 2 \times 6 + 2 \times (-7) + 1 \times 2 \\
    &= 12 - 14 + 2 \\
    &= 0, \\
    \vec{v} \cdot (\vec{u} \times \vec{v}) &= (3, 4, 5) \cdot (6, -7, 2) \\
    &= 3 \times 6 + 4 \times (-7) + 5 \times 2 \\
    &= 18 - 28 + 10 \\
    &= 0.
\end{align*} $$

The glm function for calculating the cross product between two vectors is `glm::cross(a,b)`. Add the following code to your program.

```cpp
// The cross product
glm::vec3 uCrossV = glm::cross(u, v);

std::cout << "\nThe cross product\n----------------- " << std::endl;
std::cout << "u x v = " << uCrossV << std::endl;
std::cout << "u . (u x v) = " << glm::dot(u, uCrossV) << std::endl;
std::cout << "v . (u x v) = " << glm::dot(v, uCrossV) << std::endl;
```

 Output

 ```text
The cross product
----------------- 
u x v = [    6.000,   -7.000,    2.000]
u . (u x v) = 0
v . (u x v) = 0
 ```

## Matrices

A matrix is a rectangular array of numbers, e.g.,

$$ A = \begin{pmatrix} 1 & 2 \\ 3 & 4 \end{pmatrix}. $$

We refer to the size of a matrix by the number of horizontal rows $\times$ the number of vertical columns. Here the matrix $A$ has 2 rows and 2 columns so we call this matrix a $2 \times 2$ matrix. The individual elements of a matrix are referenced by an **index** which is a pair of numbers, the first of which is the row number and the second is the column number, so $a_{ij}$ is the element in row $i$ and column $j$ of the matrix $A$. For example for the matrix $A$ defined above $a_{11} = 1$, $a_{12} = 2$, $a_{21} = 3$ and $a_{22} = 4$.

(transpose-section)=
### Matrix transpose

The **transpose** of a matrix $A$ is denoted by $A^\mathsf{T}$ and is defined by swapping the rows and columns of $A$.

For example, given the matrix $A$ defined above

$$ A^\mathsf{T} = \begin{pmatrix} 1 & 2 \\ 3 & 4 \end{pmatrix}^\mathsf{T} = \begin{pmatrix} 1 & 3 \\ 2 & 4 \end{pmatrix}. $$

### Defining matrices in glm

The glm command for defining a matrix is `glm::matn(1.0f)` where `n` is the number of rows and columns of the matrix, e.g., `glm::mat2(1.0f)` will define a $2\times 2$ matrix (in fact this defines a $2\times 2$ [identity matrix](identity-matrix-section)). Lets define the $2 \times 2$ matrix $A$ in our program and output it so add the following to your program.

```cpp
// Defining a matrix
glm::mat2 A = glm::mat2(1.0f);
A[0][0] = 1.0f, A[0][1] = 2.0f;
A[1][0] = 3.0f, A[1][1] = 4.0f;

std::cout << "\nDefining a matrix\n-----------------" << std::endl;
std::cout << "A =" << A << std::endl;
```

The indexing of glm matrices is done in a similar way to vectors but instead of having one index we now have two, one for the row number and one for the column number. So the value of the element $a_{11}$ is written in C++ as `A[0][0]`.

Output

```text
Defining a matrix
-----------------
A =
[[    1.000,    3.000]
 [    2.000,    4.000]]
```

Hang on something isn't right here. Looking at the matrix that was outputted we have 1 and 3 on the top row when we were expecting 1 and 2, so what has gone wrong? To explain we need to look at how arrays are stored in the memory. Memory can be considered as being one long 1D array so when a matrix is stored in the memory we have a choice whether to use **row-major order** where we store the rows of the matrix one after the other or **column-major order** where we store the columns of the matrix on after the other. For example, given the matrix

$$ A = \begin{pmatrix} a & b & c \\ d & e & f \\ g & h & i \end{pmatrix},$$

then in row-major order the matrix would be stored as

```text
a b c d e f g h i
```

and in column-major order we have

```text
a d g b e h c f i
```

**OpenGL and glm use column-major order** so when we output the contents of a glm matrix it outputs it column-by-column which is why our elements in the top row of $A$, 1 and 2, are outputted in the first column. So to output a glm matrix so that it looks right we need to output the transpose. Change your output command so that it looks like the following.

```cpp
std::cout << "A = " << glm::transpose(A) << std::endl;
```

The output should now be

```text
Defining a matrix
-----------------
A = 
[[    1.000,    2.000]
 [    3.000,    4.000]]
```

(matrix-multiplication-section)=
### Matrix multiplication

The addition, subtraction and multiplication by a scalar for matrices is the same as it is for vectors so I'm not going to repeat it here. However, the multiplication of two matrices $A$ and $B$ is defined in a very specific way. If $A$ and $B$ are two matrices then the element in row $i$ and column $j$ of the matrix $AB$ is calculated using

$$ [AB]_{ij} = \vec{a}_i \cdot \vec{b}_j, $$(eq:matrix-multiplication)

where $\vec{a}_i$ is the vector formed from row $i$ of $A$ and $\vec{b}_j$ is the vector formed from column $j$ of $B$.

For example, given the matrices $A$ and $B$

$$ \begin{align*}
    A &= \begin{pmatrix} 1 & 2 \\ 3 & 4 \end{pmatrix}, &
    B &= \begin{pmatrix} 5 & 6 \\ 7 & 8 \end{pmatrix},
\end{align*} $$

then the multiplication $AB$ is

$$ \begin{align*}
    AB &=
    \begin{pmatrix} 1 & 2 \\ 3 & 4 \end{pmatrix}
    \begin{pmatrix} 5 & 6 \\ 7 & 8 \end{pmatrix} =
    \begin{pmatrix}
        (1, 2) \cdot (5, 7) & (1,2) \cdot (6, 8) \\
        (3, 4) \cdot (5, 7) & (3, 4) \cdot (6, 8)
    \end{pmatrix} \\
    &=
    \begin{pmatrix} 5 + 14 & 6 + 16 \\ 15 + 28 & 18 + 32 \end{pmatrix} =
    \begin{pmatrix} 19 & 22 \\ 43 & 50 \end{pmatrix}.
\end{align*} $$

Note that unlike numbers where is doesn't matter which way round they are when we multiplied (i.e., $1 \cdot 2 = 2 \cdot 1 = 2$) this is **not** the case with matrices. For example, lets calculate $BA$

$$ \begin{align*}
    BA &=
    \begin{pmatrix} 5 & 6 \\ 7 & 8 \end{pmatrix}
    \begin{pmatrix} 1 & 2 \\ 3 & 4 \end{pmatrix} =
    \begin{pmatrix}
        (5, 6) \cdot (1, 3) & (5, 6) \cdot (2, 4) \\
        (7, 8) \cdot (1, 3) & (7, 8) \cdot (2, 4)
    \end{pmatrix} \\
    &=
    \begin{pmatrix} 5 + 18 & 10 + 24 \\ 7 + 24 & 14 + 32 \end{pmatrix} =
    \begin{pmatrix} 23 & 34 \\ 31 & 46 \end{pmatrix}.
\end{align*} $$

So based on these two examples we can see that $AB \neq BA$ which is very important when it comes to calculating [transformations](transformations-section).

```{important}
The glm operator `*` is used to multiply two or more matrices together. However, since glm uses column-major order to store matrices the order of the matrices is reversed so to calculate $AB$ we would use the command `B * A`.
```

Check our examples above by adding the following code to your program.

```cpp
// Matrix multiplication
glm::mat2 B = glm::mat2(1.0f);
B[0][0] = 5, B[0][1] = 6;
B[1][0] = 7, B[1][1] = 8;
glm::mat2 AB = B * A;   // note the ordering!
glm::mat2 BA = A * B;

std::cout << "\nMatrix multiplication\n---------------------" << std::endl;
std::cout << "B =" << glm::transpose(B) << std::endl;
std::cout << "\nAB =" << glm::transpose(AB) << std::endl;
std::cout << "\nBA =" << glm::transpose(BA) << std::endl;
```

Output

```text
Matrix multiplication
---------------------
B =
[[    5.000,    6.000]
 [    7.000,    8.000]]

AB =
[[   19.000,   22.000]
 [   43.000,   50.000]]

BA =
[[   23.000,   34.000]
 [   31.000,   46.000]]
```

(identity-matrix-section)=
### The identity matrix

The **identity matrix** is a special square matrix (same number of rows and columns) where all the elements are zero apart from the elements on the diagonal line from the top-left element down to the bottom-right element (known as the **main diagonal**). For example the $4\times 4$ identity matrix is

$$ I = \begin{pmatrix}
    1 & 0 & 0 & 0 \\
    0 & 1 & 0 & 0 \\
    0 & 0 & 1 & 0 \\
    0 & 0 & 0 & 1
\end{pmatrix}.$$

The identity element is similar to the number 1 in that if we multiply any matrix by an identity matrix the result is unchanged. For example

$$ \begin{align*}
    \begin{pmatrix} 1 & 0 \\ 0 & 1 \end{pmatrix}
    \begin{pmatrix} 1 & 2 \\ 3 & 4 \end{pmatrix} =
    \begin{pmatrix}
        (1,0) \cdot (1, 3) & (1,0) \cdot (2, 4) \\
        (0,1) \cdot (1,3) & (0,1) \cdot (2,4)
    \end{pmatrix} =
    \begin{pmatrix} 1 & 2 \\ 3 & 4 \end{pmatrix}.
\end{align*} $$

We can generate an `n`$\times$`n` identity matrix by using the glm command `glm::matn(1.0f)`. Add the following code to your program

```cpp
// Identity matrix
glm::mat2 I = glm::mat2(1.0f);
glm::mat2 IA = A * I;

std::cout << "\nIdentity matrix\n---------------" << std::endl;
std::cout << "I =" << glm::transpose(I) << std::endl;
std::cout << "\n IA =" << glm::transpose(IA) << std::endl;
```

Output

```text
Identity matrix
---------------
I =
[[    1.000,    0.000]
 [    0.000,    1.000]]

 IA =
[[    1.000,    2.000]
 [    3.000,    4.000]]
```

(inverse-matrix-section)=

### The inverse matrix

Whilst matrix multiplication is defined for certain matrices there is no way of dividing a matrix by another. However, for certain square matrices we can calculate an **inverse matrix** that performs a similar function to divide. Consider the division of two numbers, 4 and 2 say. If we wanted to divide 4 by two we could write

$$ \frac{8}{2} = 4. $$

We could also write this division as the multiplication of $\frac{1}{2}$ and 8, i.e.,

$$ \frac{1}{2} \times 8 = 4. $$

Here we have shown that $\frac{1}{2}$ is the **multiplicative inverse** of 2. A multiplicative inverse of a number $x$ is denoted as $x^{-1}$ and satisfies $x \cdot x^{-1} = 1$. The inverse of a matrix $A$ is denoted by $A^{-1}$ and satisfied $A^{-1}A = I$ where $I$ is the identity matrix. For example, the inverse of the matrix

$$A = \begin{pmatrix} 1 & 2 \\ 3 & 4 \end{pmatrix}, $$

is

$$A^{-1} = \begin{pmatrix} -2 & 1 \\ 3/2 & -1/2 \end{pmatrix}. $$ 

We can check whether this is the inverse of $A$ by calculating $A^{-1}A$

$$ \begin{align*}
    \begin{pmatrix} -2 & 1 \\ 3/2 & -1/2 \end{pmatrix}
    \begin{pmatrix} 1 & 2 \\ 3 & 4 \end{pmatrix}
    &=
    \begin{pmatrix}
        (-2, 1) \cdot (1, 3) & (-2, 1) \cdot (2, 4) \\
        (3/2, -1/2) \cdot (1, 3) & (3/2, -1/2) \cdot (2, 4)
    \end{pmatrix} \\
    &=
    \begin{pmatrix} 1 & 0 \\ 0 & 1 \end{pmatrix}.
\end{align*} $$

We can calculate the inverse of a square matrix using the glm function `glm::inverse()`. Add the following code to your program

```cpp
// Inverse matrix
glm::mat2 invA = glm::inverse(A);
std::cout << "\nInverse matrix\n--------------" << std::endl;
std::cout << "invA =" << glm::transpose(invA) << std::endl;
std::cout << "invA * A" << glm::transpose(invA * A) << std::endl;
```

Output

```text
Inverse matrix
--------------
invA =
[[   -2.000,    1.000]
 [    1.500,   -0.500]]

invA * A =
[[    1.000,    0.000]
 [    0.000,    1.000]]
```

Not all square matrices have an inverse. For example, no inverse exists for the matrix

$$ C = \begin{pmatrix} 1 & 2 \\ 3 & 6 \end{pmatrix}.$$

Lets try to use glm to calculate the inverse of $C$.

```cpp
glm::mat2 C = glm::mat2(1.0f);
C[0][0] = 1, C[0][1] = 2;
C[1][0] = 3, C[1][1] = 6;
glm::mat2 invC = glm::inverse(C);
std::cout << "\nC =" <<glm::transpose(C) << std::endl;
std::cout << "\ninvC =" <<glm::transpose(invC) << std::endl;
```

Output

```text
invC =
[[      inf,     -inf]
 [     -inf,      inf]]
```

Here glm has encountered a divide by zero error. We can determine whether a matrix has an inverse or not by calculating a special value called a **determinant**. The determinant of a matrix $A$ is denoted by $\det(A)$ and if the determinant is equal to zero then the matrix does not have an inverse. The method of calculating the determinant is outside the scope of what we are doing here but fortunately glm has a function to do this which is `glm::determinant()`

```cpp
std::cout << "\ndet(C) = " << glm::determinant(C) << std::endl;
```

Output

```text
det(C) = 0
```

---

(vectors-exercises)=

## Exercises

1. Three points have the co-ordinates P = (5, 1, 3), Q = (10, 7, 4) and R = (0, 5, -3). Use C++ code to output the following:

    (a) The vector $\vec{p}$ that points from P to Q;<br>
    (b) The vector $\vec{q}$ that points from Q to R;<br>
    (c) The vector $\vec{r}$ that points from R to P;<br>
    (d) The length of the vector $\vec{p}$;<br>
    (e) A unit vector that points in the direction of the vector $\vec{q}$;<br>
    (f) The dot product $\vec{p} \cdot \vec{q}$;<br>
    (g) The cross product $\vec{q} \times \vec{r}$.

2. The three matrices $A$, $B$ and $C$ are defined as

$$ \begin{align*}
    A &= \begin{pmatrix} -1 & 3 \\ 2 & -5 \end{pmatrix}, &
    B &= \begin{pmatrix} 0 & 2 \\ 7 & 1 \end{pmatrix}, &
    C &= \begin{pmatrix} 3 & 2 \\ -3 & -4 \end{pmatrix}.
\end{align*} $$

&emsp;&emsp; Use C++ code to output the following:

&emsp;&emsp; (a) $AB$;<br>
&emsp;&emsp; (b) $ABC$;<br>
&emsp;&emsp; (c) $CBA$;<br>
&emsp;&emsp; (d) $A^\mathsf{T}B$;<br>
&emsp;&emsp; (f) $A^{-1}$.

3. Create your own C++ class called MyLib using header and code files **MyLib.hpp** and **MyLib.cpp** respectively and define methods for calculating the following:

    (a) the length of a vector (equation {eq}`eq:vector-magnitude`);<br>
    (b) normalising a vector (equation{eq}`eq:unit-vector`);<br>
    (c) the dot product of two vectors (equation {eq}`eq:dot-product`);<br>
    (d) the cross product of two vectors (equation {eq}`eq:cross-product`);<br>
    (e) the transpose of a $2 \times 2$ matrix; <br>
    (f) the multiplication of two $2\times 2$ matrices (equation {eq}`eq:matrix-multiplication`);<br>
    (g) the calculation of the inverse of a $2\times 2$ matrix. The inverse of the matrix $A = \begin{pmatrix} a & b \\ c & d \end{pmatrix}$ is

 $$A^{-1} = \dfrac{1}{ad - bc} \begin{pmatrix} d & -b \\ -c & a \end{pmatrix}.$$

Use your class to answer questions 1 and 2 (you may use glm vectors and matrices).

Refer to the section on [Object Orientated Programming](oop-section) for a reminder on how to add classes to your program.

## Source code

The source code for this lab, including the exercise solutions, can be downloaded using the links below.

- [main.cpp](../code/Lab04_Vectors_and_matrices/main.cpp)
- [MyLib.hpp](../code/Lab04_Vectors_and_matrices/MyLib.hpp)
- [MyLib.cpp](../code/Lab04_Vectors_and_matrices/MyLib.cpp)