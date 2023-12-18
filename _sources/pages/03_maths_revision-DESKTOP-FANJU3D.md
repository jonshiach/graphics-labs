# Vectors and Matrices

Computer graphics relies heavily on mathematics of vectors and matrices. In this lab we will be revising these and show how we can perform calculations in C++.

## The GLM library

The GLM (<a href="https://github.com/g-truc/glm" target="_blank">OpenGL Mathematics</a>) library is a popular C++ mathematics library designed to provide classes and functions for mathematical operations, specifically tailored for graphics programming using OpenGL. We will be using functions from GLM to perform calculations for us. 

1. Create an empty project in Visual Studio or Xcode and called it **lab03_vectors_and_matrices**.

2. Using the command prompt in Windows or the terminal in macOS and navigate to your newly created project folder. Within the project folder clone the GLM repository

```text
git clone https://github.com/g-truc/glm
```

3. Add the glm/ folder to your project. 

    Visual studio:

    1. Right-click on your project in the project viewer and click on 'Properties'.
    2. Select 'C/C++' and then 'General'.
    3. Click on the down arrow next to 'Additional Include Directories' and then select 'Edit'. 
    4. Add your glm/ folder and click OK a couple of times.

    Xcode:

    1. k

## Vectors

A vector in three-dimensional space is an object with magnitude (length) and direction. A vector is denoted by a boldface lower case letter, e.g., $\mathbf{a}$, or an underlined lower case letter, e.g., $\underline{a}$, when handwritten and represented mathematically by the 3-tuple

$$ \mathbf{a} = (a_x, a_y, a_z), $$

where $a_x$, $a_y$ and $a_z$ are the displacement of the vector in the $x$, $y$ and $z$ directions.

```{figure} ../images/vector.svg
:width: 300
```

### Vector magnitude

The **magnitude** of a vector $\mathbf{a} = (a_x, a_y, a_z)$ is denoted by $|\mathbf{a}|$ is the length from the tail of the vector to the head and calculated using 

$$ |\mathbf{a}| = \sqrt{a_x^2 + a_y^2 + a_z^2}. $$(eq:vector-magnitude)

```{figure} ../images/vector_magnitude.svg
:width: 120
```

For example, the magnitude of the vector $\mathbf{u} = (2, 2, 1)$ is 

$$ | \mathbf{u} | = \sqrt{2^2 + 2^2 + 1^2} = \sqrt{4 + 4 + 1} = \sqrt{9} = 3. $$

We will now define this vector in C++ and use the glm library to calculate its magnitude.

```cpp
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL    // needed for the to_string function
#include <glm/glm.hpp>
#include <glm/ext.hpp>

int main() {

    // Vector magnitude
    glm::vec3 u(2.0f, 2.0f, 1.0f);    // define the vector u
    float uMag = glm::length(u);

    std::cout << "Vector magnitude\n----------------" << std::endl;
    std::cout << "u = " << glm::to_string(u) << std::endl;
    std::cout << "|u| = " << uMag << std::endl;

    return 0;

}
```

The `glm::vec u(2.0f, 2.0f, 1.0f)` command defines a 3-element vector `u` and the magnitude is calculated using `glm::length(u)`. The output of the program is shown below.

```text
Vector magnitude
----------------
u = vec3(2.000000, 2.000000, 1.000000)
|u| = 3
```

### Vector multiplication by a scalar

The multiplication of a vector $\mathbf{a} = (a_x, a_y, a_z)$ by a scalar (a number) $k$ is calculated using

$$ k \mathbf{a} = (ka_x, ka_y, ka_z) $$(eq:vector-multiplication)

Multiplying a vector by a positive scalar has the effect of scaling the length of the vector. Multiplying by a negative scalar reverses the direction of the vector.

```{figure} ../images/vector_multiplication.svg
:width: 300
```

For example, multiplying the vector $\mathbf{u} = (2, 2, 1)$ by the scalar 2 gives

$$ 2\mathbf{u} = 2(2,2,1) = (4, 4, 2), $$

which has the magnitude

$$ |2 \mathbf{u} | = \sqrt{4^2 + 4^2 + 2^2} = \sqrt{36} = 6 = 2 |\mathbf{u}|. $$

To multiply a glm vector by a scalar we simply using the standard `*` operator. Add the following to your code to check it returns the correct vector.

Add the following code to your program.

```cpp
// Multiplication by a scalar
float k = 2.0f;
glm::vec3 u2 = k * u;

std::cout << "\nMultiplication by a scalar\n--------------------------" << std::endl;
std::cout << "2u = " << glm::to_string(u2) << std::endl;
```

The output is

```text
Multiplication by a scalar
--------------------------
2u = vec3(4.000000, 4.000000, 2.000000)
```

### Unit vectors

A unit vector is a vector that has a magnitude of 1. We can find a unit vector that points in the same direction as a non-zero vector $\mathbf{a}$, which is denoted by $\hat{\mathbf{a}}$ (pronounced "a-hat"), by dividing by its magnitude, i.e.,

$$ \hat{\mathbf{u}} = \frac{\mathbf{a}}{|\mathbf{a}|}. $$(eq:unit-vector)

For example, to determine a unit vector pointing in the same direction as $\mathbf{u} = (2, 2, 1)$

$$ \begin{align*}
    \hat{\mathbf{a}} &= \frac{(2, 2, 1)}{3} = \left( \frac{2}{3}, \frac{2}{3}, \frac{1}{3} \right).
\end{align*} $$

Checking that $\hat{\mathbf{u}}$ has a magnitude of 1

$$ |\hat{\mathbf{u}}| = \sqrt{\left(\frac{2}{3}\right)^2 + \left(\frac{2}{3}\right)^2 + \left(\frac{1}{3}\right)^2} = \sqrt{\frac{4}{9} + \frac{4}{9} + \frac{1}{9}} = \sqrt{1} = 1. $$

Add the following to your program

```cpp
// Unit vectors
glm::vec3 uHat = u / uMag;
float uHatMag = glm::length(uHat);

std::cout << "\nMultiplication by a scalar\n--------------------------" << std::endl;
std::cout << "uhat = " << glm::to_string(uHat) << std::endl;
std::cout << "|uhat| = " << uHatMag << std::endl;
```

The output is

```text
Unit vectors
------------
uhat = vec3(0.666667, 0.666667, 0.333333)
|uhat| = 1
```

### Vector addition and subtraction

The addition and subtraction of two vectors $\mathbf{a} = (a_x, a_y, a_z)$ and $\mathbf{b} = (b_x, b_y, b_z)$ is defined by 

$$ \mathbf{a} \pm \mathbf{b} = (a_x \pm b_x, a_y \pm b_y, a_z \pm b_z). $$(eq:vector-addition)

For example, given the vectors $\mathbf{u} = (2,2,1)$ and $\mathbf{v} = (3, 4, 5)$

$$ \begin{align*}
    \mathbf{u} + \mathbf{v} &= (2, 2, 1) + (3, 4, 5) = (2 + 3, 2 + 4, 1 + 5) = (5, 6, 6), \\
    \mathbf{u} - \mathbf{v} &= (2, 2, 1) - (3, 4, 5) = (2 - 3, 2 - 4, 1 - 5) = (-1, -2, -4).
\end{align*} $$

The standard `+` and `-` operators are used to add and subtract glm vectors. Add the following code to your program.

```cpp
// Vector addition and subtraction
glm::vec3 v(3.0f, 4.0f, 5.0f);
glm::vec3 uPlusv = u + v;
glm::vec3 uMinusv = u - v;

std::cout << "\nVector addition and subtraction\n-------------------------------" << std::endl;
std::cout << "v = " << glm::to_string(v) << std::endl;
std::cout << "u + v = " << glm::to_string(uPlusv) << std::endl;
std::cout << "u - v = " << glm::to_string(uMinusv) << std::endl;
 ```

The output is

```text
Vector addition and subtraction
-------------------------------
v = vec3(3.000000, 4.000000, 5.000000)
u + v = vec3(5.000000, 6.000000, 6.000000)
u - v = vec3(-1.000000, -2.000000, -4.000000)
```

### Dot product

The **dot product** between two vectors $\mathbf{a} = (a_x, a_y, a_z)$ and $\mathbf{b} = (b_x, b_y, b_z)$ is denoted by $\mathbf{a} \cdot \mathbf{b}$ and returns a scalar quantity (a number). The dot product is calculated using

$$ \mathbf{a} \cdot \mathbf{b} = a_xb_x + a_yb_y + a_zb_z. $$(eq:dot-product)

The dot product is related to the angle between the two vectors by the relationship

$$ \mathbf{a} \cdot \mathbf{b} = |\mathbf{a}| |\mathbf{b}| \cos(\theta), $$(eq:dot-product-geometric)

where $\theta$ is the angle between the vectors $\mathbf{a}$ and $\mathbf{b}$.

```{figure} ../images/dot_product.svg
:width: 150
```

A useful result for computer graphics is that if $\theta=0$ then $\cos(90^\circ)= 0$ and equation {eq}`eq:dot-product-geometric` becomes

$$ \mathbf{a} \cdot \mathbf{b} = 0. $$

In order words, if the dot product of two vectors is zero then the two vectors are perpendicular.

For example, given the vectors $\mathbf{u} = (2,2,1)$ and $\mathbf{v} = (3, 4, 5)$ the dot product $\mathbf{u} \cdot \mathbf{v}$ is

$$ \begin{align*}
    \mathbf{u} \cdot \mathbf{v} = (2, 2, 1) \cdot (3, 4, 5) = 6 + 8 + 5 = 19.
\end{align*} $$

The glm function for calculating the dot product is `glm::dot(a,b)`. Add the following to your program.

```cpp
// The dot product
float uDotv = glm::dot(u, v);

std::cout << "\nThe dot product\n--------------- " << std::endl;
std::cout << "u . v = " << uDotv << std::endl;
 ```

The output is

```text
The dot product
---------------
u . v = 19
```

### Cross products

The **cross product** between two vectors $\mathbf{a} = (a_x, a_y, a_z)$ and $\mathbf{b} = (b_x, b_y, b_z)$ is denoted by $\mathbf{a} \times \mathbf{b}$ and returns a vector. The cross product is calculated using

$$ \mathbf{a} \times \mathbf{b} = (a_yb_z - a_zb_y, a_zb_x - a_xb_z, a_xb_y - a_yb_x). $$(eq:cross-product)

The cross product between two vectors produces another vector that is perpendicular to both vectors. This is another incredibly useful result as it allows use to calculate **normal** vectors for polygons which are used in calculating how light is reflected off surfaces.

```{figure} ../images/cross_product.svg
:width: 150
```

For example, given the vectors $\mathbf{u} = (2,2,1)$ and $\mathbf{v} = (3, 4, 5)$ the cross product $\mathbf{u} \times \mathbf{v}$ is

$$ \mathbf{u} \times \mathbf{v} = (2, 2, 1) \times (3, 4, 5) = (10 - 4, 3 - 10, 8 - 6) = (6, -7, 2). $$

Show that $\mathbf{u} \times \mathbf{v}$ is perpendicular to both $\mathbf{u}$ and $\mathbf{v}$

$$ \begin{align*}
    \mathbf{u} \cdot (\mathbf{u} \times \mathbf{v}) &= (2, 2, 1) \cdot (6, -7, 2) = 12 - 14 + 2 = 0.
\end{align*} $$

The glm function for calculating the cross product between two vectors is `glm::cross(a,b)`. Add the following code to your program.

```cpp
// The cross product
glm::vec3 uCrossv = glm::cross(u, v);
float uDotuCrossv = glm::dot(u, uCrossv);

std::cout << "\nThe cross product\n----------------- " << std::endl;
std::cout << "u x v = " << glm::to_string(uCrossv) << std::endl;
std::cout << "u . (u x v) = " << uDotuCrossv << std::endl;
 ```

 The output is

 ```text
The cross product
-----------------
u x v = vec3(-1.000000, -2.000000, -4.000000)
u . (u x v) = 0
 ```

## Matrices

A matrix is a rectangular array of numbers

$$ A = \begin{pmatrix} 1 & 2 & 3 \\ 4 & 5 & 6 \end{pmatrix}. $$

We refer to the size of a matrix by the number of horizontal rows $\times$ the number of vertical columns. Here the matrix $A$ has 2 rows and 3 columns so we call this matrix a $2 \times 3$ matrix. The individual elements of a matrix are referenced by an **index** which is a pair of numbers, the first of which is the row number and the second is the column number, so $a_{ij}$ is the element in row $i$ and column $j$ of the matrix $A$. 

For example for the matrix $A$ defined above $a_{11} = 1$, $a_{12} = 2$, $a_{21} = 4$ etc.

The glm command for defining a matrix is `glm::matn` where `n` is the number of rows and columns of the matrix (e.g., `glm::mat2` will define a $2\times 2$ matrix). Lets define a $2 \times 2$ matrix $A$ in our program and print it. Add the following to your program.

```cpp
// Defining a matrix
glm::mat2 A = glm::mat2(glm::vec2(1,2), glm::vec2(3,4));

std::cout << "\nDefining a matrix\n-----------------" << std::endl;
std::cout << "A = " << std::endl;
std::cout << A[0][0] << " " << A[0][1] << std::endl;
std::cout << A[1][0] << " " << A[1][1] << std::endl;
```

Note that we have defined each row of the matrix `A` using 2-element vectors and that the indexing starts at 0, e.g., element $a_{11}$ is indexed using `A[0][0]` (the same applies to vectors).

The output is

```text
Defining a matrix
-----------------
A =
1 2
3 4
```

We will be print a number of matrices so it would make sense to define a function to do this. Outside of your `main` function add the following code.

```cpp
void print_matrix(glm::mat4 mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
```

You can now replace the lines that print the matrix with `print_matrix(A, 2, 2)` which achieves the same result.

### Matrix transpose

The **transpose** of a matrix $A$ is denoted by $A^\mathsf{T}$ and is defined by swapping the rows and columns of $A$.

For example, given the matrix $A$ defined earlier

$$ A^\mathsf{T} = \begin{pmatrix} 1 & 2 & 3 \\ 4 & 5 & 6 \end{pmatrix}^\mathsf{T} = \begin{pmatrix} 1 & 4 \\ 2 & 5 \\ 3 & 6 \end{pmatrix}. $$

The glm function to calculate the transpose of a matrix is `glm::transpose(A)`. Add the following code to your program.

```cpp
// Matrix transpose
glm::mat2 AT = glm::transpose(A);

std::cout << "\nMatrix transpose\n-----------------" << std::endl;
print_matrix(AT, 2, 2);
```

The output is

```text
Matrix transpose
-----------------
1 3
2 4
```

### Matrix multiplication

The addition, subtraction and multiplication by a scalar for matrices is the same as for vectors. The multiplication of two matrices is defined in a specific way as follows

$$ [AB]_{ij} = \mathbf{a}_i \cdot \mathbf{b}_j, $$

where $\mathbf{a}_i$ is the vector formed from row $i$ of $A$ and $\mathbf{b}_j$ is the vector formed from column $j$ of $B$. For example given the two matrices $A$ and $B$

$$ \begin{align*}
    A &= \begin{pmatrix} 1 & 2 \\ 3 & 4 \end{pmatrix}, &
    B &= \begin{pmatrix} 5 & 6 \\ 7 & 8 \end{pmatrix},
\end{align*} $$

then the multiplication $AB$ is 

$$ \begin{align*}
    \begin{pmatrix} 1 & 2 \\ 3 & 4 \end{pmatrix}
    \begin{pmatrix} 5 & 6 \\ 7 & 8 \end{pmatrix}
    &= 
    \begin{pmatrix} 
        (1,2) \cdot (5,7) & (1,2) \cdot (6,8) \\
        (3,4) \cdot (5,7) & (3,4) \cdot (6,8)
    \end{pmatrix}
    = 
    \begin{pmatrix} 19 & 22 \\ 43 & 50 \end{pmatrix}.
\end{align*} $$

## Co-ordinate Geometry