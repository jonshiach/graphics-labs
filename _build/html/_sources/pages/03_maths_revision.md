# Vectors and Matrices

Computer graphics relies heavily on mathematics of vectors and matrices. In this lab we will be revising these and show how we can perform calculations in C++.

## The glm library

The glm (<a href="https://github.com/g-truc/glm" target="_blank">OpenGL Mathematics</a>) library is a popular C++ mathematics library designed to provide classes and functions for mathematical operations, specifically tailored for graphics programming using OpenGL. We will be using functions from GLM to perform calculations for us.

1. Create an empty project in Visual Studio or Xcode and call it **lab03_vectors_and_matrices**.

2. Using the command prompt in Windows or the terminal in macOS and navigate to your newly created project folder. Within the project folder clone the GLM repository

```text
git clone https://github.com/g-truc/glm
```

3. Add the **glm/** folder to your project. 

    **Visual studio:**

    1. Right-click on your project in the project viewer and click on 'Properties'.
    2. Select 'C/C++' and then 'General'.
    3. Click on the down arrow next to 'Additional Include Directories' and then select 'Edit'. 
    4. Add your glm/ folder and click OK a couple of times.

    **Xcode:**

    1. Click on the project name on the left side of the window and then on 'Build Phases'.
    2. In the search bar do a search for 'Search' which should show section titled 'Search Paths'.
    3. Edit the 'Header Search Paths' and 'Library Search Paths' to include the folder glm/.

## Vectors

A vector in three-dimensional space is an object with magnitude (length) and direction. A vector is denoted by a boldface lower case letter, e.g., $\mathbf{a}$, or an underlined lower case letter, e.g., $\underline{a}$, when handwritten and represented mathematically by the 3-tuple

$$ \mathbf{a} = (a_x, a_y, a_z), $$

where $a_x$, $a_y$ and $a_z$ are the displacement of the vector in the $x$, $y$ and $z$ directions.

```{figure} ../images/vector.svg
:height: 250
```

### Vector magnitude

The **magnitude** of a vector $\mathbf{a} = (a_x, a_y, a_z)$ is denoted by $|\mathbf{a}|$ is the length from the tail of the vector to the head and calculated using 

$$ |\mathbf{a}| = \sqrt{a_x^2 + a_y^2 + a_z^2}. $$(eq:vector-magnitude)

```{figure} ../images/vector_magnitude.svg
:height: 75
```

For example, the magnitude of the vector $\mathbf{u} = (2, 2, 1)$ is 

$$ | \mathbf{u} | = \sqrt{2^2 + 2^2 + 1^2} = \sqrt{4 + 4 + 1} = \sqrt{9} = 3. $$

We will now define this vector in C++ and use the glm library to calculate its magnitude.

```cpp
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL    // needed for the to_string function
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main() {

    // Vector magnitude (length)
    glm::vec3 u = glm::vec3(2.0f, 2.0f, 1.0f);
    float lenu = glm::length(u);

    std::cout << "Vector magnitude\n----------------" << std::endl;
    std::cout << "u = " << glm::to_string(u) << std::endl;
    std::cout << "|u| = " << lenu << std::endl;

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
:height: 180
```

For example, multiplying the vector $\mathbf{u} = (2, 2, 1)$ by the scalar 2 gives

$$ 2\mathbf{u} = 2(2,2,1) = (4, 4, 2), $$

which has the magnitude

$$ |2 \mathbf{u} | = \sqrt{4^2 + 4^2 + 2^2} = \sqrt{36} = 6 = 2 |\mathbf{u}|. $$

To multiply a glm vector by a scalar we simply using the standard `*` operator. Add the following to your code to check it returns the correct vector.

Add the following code to your program.

```cpp
// Multiplication by a scalar
glm::vec3 _2u = 2.0f * u;
float len_2u = glm::length(_2u);

std::cout << "\nMultiplication by a scalar\n--------------------------" << std::endl;
std::cout << "2u = " << glm::to_string(_2u) << std::endl;
std::cout << "|2u| = " << len_2u << std::endl;
```

Output

```text
Multiplication by a scalar
--------------------------
2u = vec3(4.000000, 4.000000, 2.000000)
|2u| = 6
```

### Unit vectors

A unit vector is a vector that has a magnitude of 1. We can find a unit vector that points in the same direction as a non-zero vector $\mathbf{a}$, which is denoted by $\hat{\mathbf{a}}$ (pronounced "a-hat"), by dividing by its magnitude, i.e.,

$$ \hat{\mathbf{u}} = \frac{\mathbf{a}}{|\mathbf{a}|}. $$(eq:unit-vector)

For example, to determine a unit vector pointing in the same direction as $\mathbf{u} = (2, 2, 1)$

$$ \begin{align*}
    \hat{\mathbf{a}} &= \frac{(2, 2, 1)}{3} = \left( \frac{2}{3}, \frac{2}{3}, \frac{1}{3} \right) \approx (0.67, 0.67, 0.33).
\end{align*} $$

Checking that $\hat{\mathbf{u}}$ has a magnitude of 1

$$ |\hat{\mathbf{u}}| = \sqrt{\left(\frac{2}{3}\right)^2 + \left(\frac{2}{3}\right)^2 + \left(\frac{1}{3}\right)^2} = \sqrt{\frac{4}{9} + \frac{4}{9} + \frac{1}{9}} = \sqrt{1} = 1. $$

Add the following to your program

```cpp
// Unit vectors
glm::vec3 uHat = u / lenu;
float lenuHat = glm::length(uHat);

std::cout << "\nUnit vectors\n------------" << std::endl;
std::cout << "uhat = " << glm::to_string(uHat) << std::endl;
std::cout << "|uhat| = " << lenuHat << std::endl;
```

Output

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

Output

```text
Vector addition and subtraction
-------------------------------
v = vec3(3.000000, 4.000000, 5.000000)
u + v = vec3(5.000000, 6.000000, 6.000000)
u - v = vec3(-1.000000, -2.000000, -4.000000)
```

Its important to understand what is happening in a geometrical sense when we add and subtract vectors. Take a look at {numref}`vector-addition-figure`, here the vector $\mathbf{b}$ has been added to the vector $\mathbf{a}$ where the tail of $\mathbf{b}$ is places at the head of $\mathbf{a}$. The resulting vector $\mathbf{a} + \mathbf{b}$ points from the tail of $\mathbf{a}$ to the head of $\mathbf{b}$. 

With the subtraction of the vector $\mathbf{b}$ we do similar but instead multiply $\mathbf{b}$ by -1 thereby reversing its direction.

```{figure} ../images/vector_addition.svg
:height: 180
:name: vector-addition-figure

Vector addition and subtraction.
```

### Dot product

The **dot product** between two vectors $\mathbf{a} = (a_x, a_y, a_z)$ and $\mathbf{b} = (b_x, b_y, b_z)$ is denoted by $\mathbf{a} \cdot \mathbf{b}$ and returns a scalar quantity (a number). The dot product is calculated using

$$ \mathbf{a} \cdot \mathbf{b} = a_xb_x + a_yb_y + a_zb_z. $$(eq:dot-product)

The dot product is related to the angle between the two vectors by the relationship

$$ \mathbf{a} \cdot \mathbf{b} = |\mathbf{a}| |\mathbf{b}| \cos(\theta), $$(eq:dot-product-geometric)

where $\theta$ is the angle between the vectors $\mathbf{a}$ and $\mathbf{b}$.

```{figure} ../images/dot_product.svg
:height: 150
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

Output

```text
The dot product
---------------
u . v = 19
```

### Cross product

The **cross product** between two vectors $\mathbf{a} = (a_x, a_y, a_z)$ and $\mathbf{b} = (b_x, b_y, b_z)$ is denoted by $\mathbf{a} \times \mathbf{b}$ and returns a vector. The cross product is calculated using

$$ \mathbf{a} \times \mathbf{b} = (a_yb_z - a_zb_y, a_zb_x - a_xb_z, a_xb_y - a_yb_x). $$(eq:cross-product)

The cross product between two vectors produces another vector that is perpendicular to both vectors. This is another incredibly useful result as it allows use to calculate **normal** vectors for polygons which are used in calculating how light is reflected off surfaces.

```{figure} ../images/cross_product.svg
:height: 200
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

 Output

 ```text
The cross product
-----------------
u x v = vec3(-1.000000, -2.000000, -4.000000)
u . (u x v) = 0
 ```

## Matrices

A matrix is a rectangular array of numbers

$$ A = \begin{pmatrix} 1 & 2 \\ 3 & 4 \end{pmatrix}. $$

We refer to the size of a matrix by the number of horizontal rows $\times$ the number of vertical columns. Here the matrix $A$ has 2 rows and 2 columns so we call this matrix a $2 \times 2$ matrix. The individual elements of a matrix are referenced by an **index** which is a pair of numbers, the first of which is the row number and the second is the column number, so $a_{ij}$ is the element in row $i$ and column $j$ of the matrix $A$. 

For example for the matrix $A$ defined above $a_{11} = 1$, $a_{12} = 2$, $a_{21} = 3$ and $a_{22} = 4$.

The glm command for defining a matrix is `glm::matn` where `n` is the number of rows and columns of the matrix (e.g., `glm::mat2` will define a $2\times 2$ matrix). Lets define the $2 \times 2$ matrix $A$ in our program and print it. Add the following to your program.

```cpp
// Defining a matrix
glm::mat2 A = glm::mat2(glm::vec2(1.0f, 2.0f), glm::vec2(3.0f, 4.0f));

std::cout << "\nDefining a matrix\n-----------------" << std::endl;
std::cout << "A = " << std::endl;
std::cout << "[ " << A[0][0] << " " << A[0][1] << " ]" << std::endl;
std::cout << "[ " << A[1][0] << " " << A[1][1] << " ]" << std::endl;
```

Note that we have defined each row of the matrix `A` using 2-element vectors and that the indexing starts at 0, e.g., element $a_{11}$ is indexed using `A[0][0]` (the same applies to vectors).

Output

```text
Defining a matrix
-----------------
A =
[ 1 2 ]
[ 3 4 ]
```

We will be print a number of matrices so it would make sense to define a function to do this. Outside of your `main` function add the following code.

```cpp
void print_matrix(glm::mat4 mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        std::cout << "[ ";
        for (int j = 0; j < cols; j++) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << std::endl;
}
```

You can now replace the lines that print the matrix with `print_matrix(A, 2, 2)` which achieves the same result.

### Matrix transpose

The **transpose** of a matrix $A$ is denoted by $A^\mathsf{T}$ and is defined by swapping the rows and columns of $A$.

For example, given the matrix $A$ defined earlier

$$ A^\mathsf{T} = \begin{pmatrix} 1 & 2 \\ 3 & 4 \end{pmatrix}^\mathsf{T} = \begin{pmatrix} 1 & 3 \\ 2 & 4 \end{pmatrix}. $$

The glm function to calculate the transpose of a matrix is `glm::transpose(A)`. Add the following code to your program.

```cpp
// Matrix transpose
glm::mat2 ATranspose = glm::transpose(A);

std::cout << "\nMatrix transpose\n-----------------" << std::endl;
std::cout << "A^T = " << std::endl;
print_matrix(ATranspose, 2, 2);
```

Output 

```text
Matrix transpose
-----------------
A^T =
[ 1 3 ]
[ 2 4 ]
```

(matrix-multiplication-section)=
### Matrix multiplication

The addition, subtraction and multiplication by a scalar for matrices is the same as it is for vectors so I'm not going to repeat it here. However, the multiplication of two matrices $A$ and $B$ is defined in a very specific way

$$ [AB]_{ij} = \mathbf{a}_i \cdot \mathbf{b}_j, $$(eq:matrix-multiplication)

where $\mathbf{a}_i$ is the vector formed from row $i$ of $A$ and $\mathbf{b}_j$ is the vector formed from column $j$ of $B$. 

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

If we were to transpose both $A$ and $B$ then we would see that $AB = B^\mathsf{T}A^\mathsf{T}$, for example, using the matrices defined above

$$ \begin{align*}
    B^\mathsf{T}A^\mathsf{T} &=
    \begin{pmatrix} 5 & 7 \\ 6 & 8 \end{pmatrix}
    \begin{pmatrix} 1 & 3 \\ 2 & 4 \end{pmatrix} =
    \begin{pmatrix}
        (5, 7) \cdot (1, 2) & (5, 7) \cdot (3, 4) \\
        (6, 8) \cdot (1, 2) & (6, 8) \cdot (3, 4))
    \end{pmatrix} =
    \begin{pmatrix} 19 & 22 \\ 43 & 50 \end{pmatrix}.
\end{align*} $$

````{note}
Memory in a computer can be considered as being one long 1D array, so when a matrix is stored in the memory we have a choice whether to use **row-major order** where we store the rows of the matrix one after the other or **column-major order** where we store the columns of the matrix on after the other. For example, given the matrix

$$ A = \begin{pmatrix} a & b & c \\ d & e & f \\ g & h & i \end{pmatrix},$$

then in row-major order the matrix would be stored as

```
a b c d e f g h i
```

and in column-major order we have

```
a d g b e h c f i
```

Note that the transpose of $A$ is

$$ A^\mathsf{T} = \begin{pmatrix} a & d & g \\ b & e & h \\ c & f & i \end{pmatrix}, $$

and the row-major/column major order are swapped. Mathematically it doesn't matter whether we use row or column-major as long as we multiply in the correct order. OpenGL uses column-major order so the glm operator for matrix multiplication is reversed, e.g., $AB$ is calculated using `B * A`.
````

The `*` operator is used to multiply two or more glm matrices together but the order is reversed to how it is written (see above). Add the following code to your program.

```cpp
// Matrix multiplication
glm::mat2 B = glm::mat2(glm::vec2(5,6), glm::vec2(7,8));
glm::mat2 AB = B * A;   // note the ordering!

std::cout << "\nMatrix multiplication\n---------------------" << std::endl;
std::cout << "B = " << std::endl;
print_matrix(B, 2, 2);
std::cout << "AB = " << std::endl;
print_matrix(AB, 2, 2);
```

Output

```text
 Matrix multiplication
---------------------
B =
[ 5 6 ]
[ 7 8 ]

AB =
[ 19 22 ]
[ 43 50 ]
```

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

 std::cout << "\nIdentity matrix\n-----------" << std::endl;
 std::cout << "I = " << std::endl;
 print_matrix(I, 2, 2);
 std::cout << "IA = " << std::endl;
 print_matrix(IA, 2, 2);
```

Output

```text
Identity matrix
-----------
I = 
[ 1 0 ]
[ 0 1 ]

IA = 
[ 1 2 ]
[ 3 4 ]
```

---

## Transformations

````{note}
The co-ordinate system used by OpenGL is a <a href="https://en.wikipedia.org/wiki/Right-hand_rule" target="_blank">right-hand</a> 3D co-ordinate system (on your right hand the thumb represents the $x$-axis, the index finger the $y$-axis and the middle finger the $z$-axis) with the $x$-axis pointing to the right, the $y$-axis point upwards and the $z$-axis pointing out of the screen towards the viewer.

```{figure} ../images/opengl_axes.svg
:height: 220

The OpenGL co-ordinate system.
```
````

Computer graphics requires that shapes are manipulated in space by moving the shapes, shrinking or stretching, rotating and reflection to name a few. We call these manipulations **transformations**. We need a convenient way of telling the computer how to apply our transformations and for this we make use of matrices (hence why we needed to revise them here).

Each transformation has an associated **transformation matrix** which we used to multiply the co-ordinates of our shapes to calculate the co-ordinates of the transformed shape. For example if $A$ is a transformation matrix for a particular transformation and $(x,y,z)$ are the co-ordinates of a point then we apply the transformation using

$$ (x',y',z') = (x, y, z) \cdot A. $$

where $(x',y',z')$ are the co-ordinates of the transformed point.

(translation-section)=
### Translation

The **translation** transformation when applied to a set of points moves each point by the same amount. For example, consider the triangle in {numref}`translation-figure`, each of the vertices has been translated by the same **translation vector** $\mathbf{t}$ which has that effect of moving the triangle.

```{figure} ../images/translation.svg
:height: 280
:name: translation-figure

Translation of a triangle by the translation vector $\mathbf{t}= (t_x, t_y)$.
```

A problem we have is that no transformation matrix exist for applying translation to the co-ordinates $(x, y, z)$, e.g., we can't find a matrix $T$ such that

$$(x, y, z) \cdot T = (x+t_x,y + t_y, z + t_z).$$

Don't worry, all is not lost. We can use a trick where we use **homogeneous** co-ordinates instead. Homogeneous co-ordinates add another value, $w$ say, to the co-ordinates (known as Cartesian co-ordinates) such that when the $x$, $y$ and $z$ values are divided by $w$ we get the Cartesian co-ordinates.

$$\underbrace{(x, y, z, w)}_{\textsf{homogeneous}} \equiv \underbrace{\left( \frac{x}{w}, \frac{y}{w}, \frac{z}{w} \right)}_{\textsf{Cartesian}}.$$

So if we choose $w=1$ then we can write the Cartesian co-ordinates $(x,y,z)$ as the homogeneous co-ordinates $(x,y,z,1)$ (remember that 4-element vector with the additional 1 in our [vertex shader](vertex-shader-section)?). So how does that help us with our elusive translation matrix? Well we can now represent translation as a $4\times 4$ matrix

$$ \begin{align*}
   (x, y, z, 1)
   \begin{pmatrix}
       1 & 0 & 0 & 0 \\
       0 & 1 & 0 & 0 \\
       0 & 0 & 1 & 0 \\
       t_x & t_y & t_z & 1
   \end{pmatrix} =
   (x + t_x, y + t_y, z + t_z, 1),
\end{align*}$$

which is our desired translation. So the **translation matrix** for translating a set of points by the vector $\mathbf{t} = (t_x, t_y, t_z)$ is

$$ T = \begin{pmatrix}
    1 & 0 & 0 & 0 \\
    0 & 1 & 0 & 0 \\
    0 & 0 & 1 & 0 \\
    t_x & t_y & t_z & 1
\end{pmatrix}. $$(eq:translation-matrix)

The beauty of using matrices is that we can apply the same transformation to multiple points with a single matrix multiplication. Lets say we have points with co-ordinates $(x_0, y_0, z_0)$, $(x_1, y_1, z_1)$, etc. then we can form a single matrix where our co-ordinates form the rows and then multiply this by the translation matrix

$$ \begin{align*}
    \begin{pmatrix}
        x_0 & y_0 & z_0 & 1 \\
        x_1 & y_1 & z_1 & 1 \\
        \vdots & \vdots & \vdots & \vdots \\
        x_n & y_n & z_n & 1
    \end{pmatrix}
    \begin{pmatrix}
       1 & 0 & 0 & 0 \\
       0 & 1 & 0 & 0 \\
       0 & 0 & 1 & 0 \\
       t_x & t_y & t_z & 1
    \end{pmatrix} =
    \begin{pmatrix} 
        x_0 + t_x & y_0 + t_y & z_0 + t_z & 1 \\
        x_1 + t_x & y_1 + t_y & z_1 + t_z & 1 \\
        \vdots & \vdots & \vdots & \vdots \\
        x_n + t_x & y_n + t_y & z_n + t_z & 1 \\
    \end{pmatrix}
\end{align*}. $$

For example, translate the triangle with vertices at (1,1,0), (3,1,0) and (2,3,0) by the translation vector (4,1,0).

$$ \begin{align*}
    \begin{pmatrix}
        1 & 1 & 0 & 1 \\
        3 & 1 & 0 & 1 \\
        2 & 3 & 0 & 1
    \end{pmatrix}
    \begin{pmatrix}
       1 & 0 & 0 & 0 \\
       0 & 1 & 0 & 0 \\
       0 & 0 & 1 & 0 \\
       4 & 1 & 0 & 1
    \end{pmatrix} =
    \begin{pmatrix}
        5 & 2 & 0 & 1 \\
        7 & 2 & 0 & 1 \\
        6 & 4 & 0 & 1
    \end{pmatrix}
\end{align*} $$

so the translated triangle has vertices at (5, 2, 0), (7, 2, 0) and (6, 4, 0) (this is the translation shown in {numref}`translation-figure`).

Lets write some code to apply this translation. Add the following to your program.

```cpp
 // Translation
 // Define vertex co-ordinates (using homogenous co-ordinates)
 glm::mat3x4 vertices = glm::mat3x4(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
                                    glm::vec4(3.0f, 1.0f, 0.0f, 1.0f),
                                    glm::vec4(2.0f, 3.0f, 0.0f, 1.0f));
 
 // Define translation matrix
glm::mat4 translationMatrix = glm::mat4(1.0f);
translationMatrix[3][0] = 4.0f;
translationMatrix[3][1] = 1.0f;
translationMatrix[3][2] = 0.0f;
 
 // Apply translation
 glm::mat3x4 translatedVertices = translationMatrix * vertices;  // apply translation
 
 std::cout << "\nTranslation\n---------------" << std::endl;
 std::cout << "vertices = " << std::endl;
 print_matrix(vertices, 3, 4);
 std::cout << "translationMatrix = " << std::endl;
 print_matrix(translationMatrix, 4, 4);
 std::cout << "translatedVertices = " << std::endl;
 print_matrix(translatedVertices, 3, 4);
 ```

 Output

 ```text
 Translation
---------------
vertices = 
[ 1 1 0 1 ]
[ 3 1 0 1 ]
[ 2 3 0 1 ]

translationMatrix = 
[ 1 0 0 0 ]
[ 0 1 0 0 ]
[ 0 0 1 0 ]
[ 4 1 0 1 ]

translatedVertices = 
[ 5 2 0 1 ]
[ 7 2 0 1 ]
[ 6 4 0 1 ]
 ```

Whilst it wasn't particularly difficult to define the `translationMatrix` we will be using it often so it make sense that we should use a function to do this. Fortunately there is one already in glm which we can used, `glm::translate(matrix, vector)` outputs the translation matrix for the translation by `vector` applied to `matrix`. So we could replace the two lines of code where we calculate `translationMatrix` with the following

```cpp
glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4, 1, 0));
```

Note that here we applied the translation to the identity matrix.

### Scaling

Scaling is the simplest transformation we can apply. Multiplying the $x$, $y$ and $z$ co-ordinates of a point by a scalar quantity (a number) has the effect of moving the point closer or further away from the origin (0,0). For example, consider the triangle in {numref}`scaling-figure`. The $x$ co-ordinate of each vertex has been multiplied by $s_x$ and the $y$ co-ordinates have been multiplied by $s_y$ which has the effect of scaling the triangle and moving it further away from the origin (in this case $s_x$ and $s_y$ are both greater than 1).

```{figure} ../images/scaling1.svg
:height: 350
:name: scaling-figure

Scaling of a triangle by the scaling vector $\mathbf{s}=(s_x,s_y)$.
```

The **scaling matrix** for applying the scaling transformation is 

$$ S = 
\begin{pmatrix} 
    s_x & 0 & 0 & 0 \\
    0 & s_y & 0 & 0 \\
    0 & 0 & s_z & 0 \\
    0 & 0 & 0 & 1
\end{pmatrix}. $$(eq:scaling-matrix)

For example, lets scale the same triangle as we used for demonstrating [transformation](translation-section) by the scaling vector (3, 2, 1).

$$ \begin{align*}
    \begin{pmatrix}
        1 & 1 & 0 & 1 \\
        3 & 1 & 0 & 1 \\
        2 & 3 & 0 & 1
    \end{pmatrix}
    \begin{pmatrix}
       3 & 0 & 0 & 0 \\
       0 & 2 & 0 & 0 \\
       0 & 0 & 1 & 0 \\
       0 & 0 & 0 & 1
    \end{pmatrix} = 
    \begin{pmatrix}
        3 & 2 & 0 & 1 \\
        9 & 2 & 0 & 1 \\
        6 & 6 & 0 & 1
    \end{pmatrix}
\end{align*}, $$

so the scaled triangle now has vertices at (3,2,0), (6,2,0) and (4,6,0) (this is the scaling shown in {numref}`scaling-figure`). 

Lets do this in our program.

```cpp
 // Scaling
 // Define the scaling matrix
 glm::mat4 scalingMatrix = glm::mat4(1.0f);
 scalingMatrix[0][0] = 3.0f;
 scalingMatrix[1][1] = 2.0f;
 
 //Apply scaling
 glm::mat3x4 scaledVertices = scalingMatrix * vertices;
 
 std::cout << "\nScaling\n-------" << std::endl;
 std::cout << "scalingMatrix = " << std::endl;
 print_matrix(scalingMatrix, 4, 4);
 std::cout << "scaledVertices = " << std::endl;
 print_matrix(scaledVertices, 3, 4);
```

Output

```text
Scaling
-------
scalingMatrix = 
[ 3 0 0 0 ]
[ 0 2 0 0 ]
[ 0 0 1 0 ]
[ 0 0 0 1 ]

scaledVertices = 
[ 3 2 0 1 ]
[ 9 2 0 1 ]
[ 6 6 0 1 ]
```

Like with the translation matrix, we can use the glm command `glm::scale(matrix, vector)` to generate `scalingMatrix`

```cpp
glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(3, 2, 1));
```

The center of the original triangle was at (2, 1.67, 0) (the sum of the vertices divided by 3) and the center of the scaled triangle is at (6, 3.33, 0). A more useful transformation is to scale shapes so that the position of the center of the shape doesn't move (think of an object in a computer game growing and shrinking).

If we have an object which has a centre at the origin then the effect of scaling the vertex co-ordinates means that the centre remains at the origin as shown in {numref}`scaling-about-origin-figure`.

```{figure} ../images/scaling2.svg
:height: 350
:name: scaling-about-origin-figure

Scaling a triangle centred at the origin.
```

So to scale the example triangle so that its centre remains unchanged we first need to translate it to the origin by the translation vector (-2,-1.67,0) (the center co-ordinates with the signs changed), perform the scaling, and then translate it back to the its original position by the translation vector (2, 1.67, 0).

- Translate to the origin:

$$ \begin{align*}
    \begin{pmatrix}
        1 & 1 & 0 & 1 \\
        3 & 1 & 0 & 1 \\
        2 & 3 & 0 & 1
    \end{pmatrix}
    \begin{pmatrix}
        1 & 0 & 0 & 0 \\
        0 & 1 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        -2 & -1.67 & 0 & 1
    \end{pmatrix} =
    \begin{pmatrix}
        -1 & -0.67 & 0 & 1 \\
        1 & -0.67 & 0 & 1 \\
        0 & 1.33 & 0 & 1
    \end{pmatrix}
  \end{align*}. $$

- Scale:

$$ \begin{align*}
    \begin{pmatrix}
        -1 & -0.67 & 0 & 1 \\
        1 & -0.67 & 0 & 1 \\
        0 & 1.33 & 0 & 1
    \end{pmatrix}
    \begin{pmatrix}
        3 & 0 & 0 & 0 \\
        0 & 2 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        0 & 0 & 0 & 1
    \end{pmatrix} =
    \begin{pmatrix}
        -3 & -1.33 & 0 & 1 \\
        3 & -1.33 & 0 & 1 \\
        0 & 2.66 & 0 & 1
    \end{pmatrix}
\end{align*}. $$

- Translate back to the original position:

$$ \begin{align*}
    \begin{pmatrix}
        -3 & -1.33 & 0 & 1 \\
        3 & -1.33 & 0 & 1 \\
        0 & 2.66 & 0 & 1
    \end{pmatrix}
    \begin{pmatrix}
        1 & 0 & 0 & 0 \\
        0 & 1 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        2 & 1.67 & 0 & 1
    \end{pmatrix} =
    \begin{pmatrix}
        -1 & 0.33 & 0 & 1 \\
        5 & 0.33 & 0 & 1 \\
        2 & 4.33 & 0 & 1
    \end{pmatrix}
  \end{align*}. $$

So the scaled triangle has vertices at (-1,0.33,0), (5,0.33,0) and (2,4.33,0) and the centre of the scaled triangle is at (2, 1.67, 0) as before.

```{figure} ../images/scaling3.svg
:height: 300
:name: scaling-about-centre-figure

Scaling a triangle about its centre.
```

Lets do this in our program.

```cpp
// Scale a triangle about its centre
// Calculate centre co-ordinates
glm::vec3 centre = glm::vec3(0, 0, 0);
for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 3; i++) {
        centre[j] += vertices[i][j];
    }
}
centre /= 3;

// 1. Translate triangle centre to origin
glm::mat4 translationMatrix1 = glm::translate(glm::mat4(1.0f), -centre);
glm::mat4 transformedVertices = translationMatrix1 * vertices;

std::cout << "\nScaling a triangle about its centre\n----------------------" << std::endl;
std::cout << "Translate centre to origin:\n\ntransformedVertices = " << std::endl;
print_matrix(transformedVertices, 3, 4);

// 2. Scale triangle
transformedVertices = scalingMatrix * transformedVertices;

std::cout << "Scale triangle:\n\ntransformedVertices = " << std::endl;
print_matrix(transformedVertices, 3, 4);

// 3. Translate triangle centre back to original position
glm::mat4 translationMatrix2 = glm::translate(glm::mat4(1.0f), centre);
transformedVertices = translationMatrix2 * vertices;

std::cout << "Translate centre back to original position:\n\ntransformedVertices = " << std::endl;
print_matrix(transformedVertices, 3, 4);
```

Output

```text
Scaling a triangle about its centre
----------------------
Translate centre to origin:

transformedVertices =
[ -1 -0.666667 0 1 ]
[ 1 -0.666667 0 1 ]
[ 0 1.33333 0 1 ]

Scale triangle:

transformedVertices =
[ -3 -1.33333 0 1 ]
[ 3 -1.33333 0 1 ]
[ 0 2.66667 0 1 ]

Translate centre back to original position:

transformedVertices =
[ -1 0.333333 0 1 ]
[ 5 0.333333 0 1 ]
[ 2 4.33333 0 1 ]
```

### Composite transformations

We have seen that to scale an object about its centre we needed to use three transformations which means three separate matrix multiplications. Instead of performing the three transformations separately we can calculate a single transformation matrix which combines these transformations. If we say that $T_1$, $S$ and $T_2$ are the transformation matrices for the translation of the centre to the origin, scaling and translation of the centre back to the original position then the **composite** of our three transformations is

$$ \begin{align*}
    \begin{pmatrix}
        x_0' & y_0' & z_0' & 1 \\
        x_1' & y_1' & z_1' & 1 \\
        \vdots & \vdots & \vdots & \vdots \\
        x_n' & y_n' & z_n' & 1
    \end{pmatrix}
    &=  \underbrace{
            \overbrace{
                \underbrace{
                    \begin{pmatrix}
                        x_0 & y_0 & z_0 & 1 \\
                        x_1 & y_1 & z_1 & 1 \\
                        \vdots & \vdots & \vdots & \vdots \\
                        x_n & y_n & z_n & 1
                    \end{pmatrix}
                    \cdot T_1
                }_{\text{translate to origin}}
            \cdot S }^{\text{scale}}
        \cdot T_2}_{\text{translate back to original position}}.
\end{align*} $$

If we say that $A = T_1 \cdot S \cdot T_2$ then $A$ is a **composite transformation matrix** that combines our three transformations (fun fact - this is why matrix multiplication is defined the way it is in equation {eq}`eq:matrix-multiplication`). So for our example

$$ \begin{align*}
    A &= T_1 \cdot S \cdot T_2 \\
    &= 
    \begin{pmatrix}
        1 & 0 & 0 & 0 \\
        0 & 1 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        -2 & -1.67 & 0 & 1
    \end{pmatrix}
    \begin{pmatrix}
        3 & 0 & 0 & 0 \\
        0 & 2 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        0 & 0 & 0 & 1
    \end{pmatrix}
    \begin{pmatrix}
        1 & 0 & 0 & 0 \\
        0 & 1 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        2 & 1.67 & 0 & 1
    \end{pmatrix} \\
    &=
    \begin{pmatrix}
        3 & 0 & 0 & 0 \\
        0 & 2 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        -6 & -3.33 & 0 & 1
    \end{pmatrix}
    \begin{pmatrix}
        1 & 0 & 0 & 0 \\
        0 & 1 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        2 & 1.67 & 0 & 1
    \end{pmatrix} \\
    &=
    \begin{pmatrix}
        3 & 0 & 0 & 0 \\
        0 & 2 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        -4 & -1.67 & 0 & 1
    \end{pmatrix}.
\end{align*} $$

Applying our composite transformation matrix to the triangle vertices

$$ \begin{align*}
    \begin{pmatrix}
        1 & 1 & 0 & 1 \\
        3 & 1 & 0 & 1 \\
        2 & 3 & 0 & 1
    \end{pmatrix}
    \begin{pmatrix}
        3 & 0 & 0 & 0 \\
        0 & 2 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        -4 & -1.67 & 0 & 1
    \end{pmatrix} = 
    \begin{pmatrix}
        -1 & 0.33 & 0 & 1 \\
        5 & 0.33 & 0 & 1 \\
        2 & 4.33 & 0 & 1
    \end{pmatrix},
\end{align*} $$

which are the same as when we did the three transformations separately. Lets do this with code. Add the following to your program.

```cpp
// Composite transformations
glm::mat4 transformationMatrix = translationMatrix2 * scalingMatrix * translationMatrix1;
transformedVertices = transformationMatrix * vertices;

std::cout << "transformationMatrix = " << std::endl;
print_matrix(transformationMatrix, 4, 4);
std::cout << "transformedVertices = " << std::endl;
print_matrix(transformedVertices, 3, 4);
```

````{important}
Remember the OpenGL and glm use column-major ordering so we reverse the order of our matrices when multiplying. So $T_1 \cdot S \cdot T_2$ would be coded as

```cpp
T2 * S * T1
```

i.e., the transformations are applied from right to left.
````

Output

```text
transformationMatrix =
[ 3 0 0 0 ]
[ 0 2 0 0 ]
[ 0 0 1 0 ]
[ -4 -1.66667 0 1 ]

transformedVertices =
[ -3 -1.33333 0 1 ]
[ 3 -1.33333 0 1 ]
[ 0 2.66667 0 1 ]
```

### Rotation

As well as moving objects and scaling them, the next most common transformation is the rotation of objects around the three co-ordinate axes $x$, $y$ and $z$. We define the rotation **anticlockwise** around each of the co-ordinate axes by an angle $\theta$ when looking down the axes ({numref}`3D-rotation-figure`).

```{figure} ../images/3D_rotation.svg
:height: 350
:name: 3D-rotation-figure

Rotation is assumed to be in the anti-clockwise direction when looking down the axis.
```

The **rotation matrices** for achieving these rotations are

| Axis | Rotation matrix |
|:--:|:--:|
| $x$ | $\begin{pmatrix} 1 & 0 & 0 & 0 \\ 0 & \cos(\theta) & \sin(\theta) & 0 \\ 0 & -\sin(\theta) & \cos(\theta) & 0 \\ 0 & 0 & 0 & 1 \end{pmatrix}$ |
| $y$ | $\begin{pmatrix} \cos(\theta) & 0 & -\sin(\theta) & 0\\ 0 & 1 & 0 & 0 \\ \sin(\theta) & 0 & \cos(\theta) & 0 \\ 0 & 0 & 0 & 1 \end{pmatrix}$ |
| $z$ | $\begin{pmatrix} \cos(\theta) & \sin(\theta) & 0 & 0 \\ -\sin(\theta) & \cos(\theta) & 0 & 0 \\ 0 & 0 & 1 & 0 \\ 0 & 0 & 0 & 1 \end{pmatrix}$|

````{dropdown} Derivation of the rotation matrices (click to show)

We will consider rotation about the $z$-axis and will restrict our co-ordinates to 2D.

```{figure} ../images/rotation.svg
:height: 300
:name: rotation-figure

Rotating the vector $\mathbf{u}$ anti-clockwise by angle $\theta$ to the vector $\mathbf{v}$.
```

Consider {numref}`rotation-figure` where the vector $\mathbf{u}$ is rotated by angle $\theta$ to the vector $\mathbf{v}$. To get this rotation we first consider the rotation of a vector with the same length as $\mathbf{u}$ but points along the $x$-axis by angle $\phi$ to get to $\mathbf{u}$. If we form a right-angled triangle (the blue one) then we know the length of the hypotenuse, $|\mathbf{u}|$, and the angle so we can calculate the lengths of the adjacent and opposite sides using trigonometry. Remember our trig ratios (SOC-CAH-TOA)

$$ \begin{align*}
    \sin(\phi) &= \frac{opposite}{hypotenuse}, &
    \cos(\phi) &= \frac{adjacent}{hypotenuse}, &
    \tan(\phi) &= \frac{adjacent}{opposite},
\end{align*} $$

so the length of the adjacent and opposite sides of the blue triangle is

$$ \begin{align*}
    adjacent &= hypotenuse \cdot \cos(\phi), \\
    opposite &= hypotenuse \cdot \sin(\phi).
\end{align*} $$

Now $u_x$ and $u_y$ are the lengths of the adjacent and opposite sides respectively and $|\mathbf{u}|$ is the length of the hypotenuse then we have

$$ \begin{align*}
    u_x &= |\mathbf{u}| \cos(\phi), \\
    u_y &= |\mathbf{u}| \sin(\phi).
\end{align*} $$

Now consider the rotation from $\mathbf{t}$ by the angle $\phi + \theta$ to get to $\mathbf{v}$. Using the same method as before we have

$$ \begin{align*}
    v_x &= |\mathbf{u}| \cos(\phi + \theta), \\
    v_y &= |\mathbf{u}| \sin(\phi + \theta).
\end{align*} $$

We can rewrite $\cos(\phi+\theta)$ and $\sin(\phi+\theta)$ using <a href="https://en.wikipedia.org/wiki/List_of_trigonometric_identities#" target="_blank">trigonometric identities</a>

$$ \begin{align*}
    \cos(\phi + \theta) &= \cos(\phi) \cos(\theta) - \sin(\phi) \sin(\theta), \\
    \sin(\phi + \theta) &= \sin(\phi) \cos(\theta) + \cos(\phi) \sin(\theta),
\end{align*} $$

so 

$$ \begin{align*}
    v_x &= |\mathbf{u}| \cos(\phi) \cos(\theta) - |\mathbf{u}| \sin(\phi) \sin(\theta), \\
    v_y &= |\mathbf{u}| \sin(\phi) \cos(\theta) + |\mathbf{u}| \cos(\phi) \sin(\theta).
\end{align*} $$

Since $ u_x = |\mathbf{u}| \cos(\phi)$ and $u_y = |\mathbf{u}| \sin(\phi)$ then

$$ \begin{align*}
    v_x &= u_x \cos(\theta) - u_y \sin(\theta), \\
    v_y &= u_y \sin(\phi) + u_x \sin(\theta),
\end{align*} $$

which can be written using matrices as

$$ \begin{align*}
    \begin{pmatrix} v_x & v_y \end{pmatrix} =
    \begin{pmatrix} u_x & u_y \end{pmatrix}
    \begin{pmatrix} \cos(\theta) & \sin(\theta) \\ -\sin(\theta) & \cos(\theta) \end{pmatrix},
\end{align*} $$

so the transformation matrix for rotating around the $z$-axis in 2D is

$$ \begin{pmatrix} \cos(\theta) & \sin(\theta) \\ -\sin(\theta) & \cos(\theta) \end{pmatrix}. $$

We need a $4\times 4$ matrix to represent 3D rotation around the $z$-axis so we replace the 3rd and 4th row and columns with the 3rd and 4th row and column from the $4\times 4$ identity matrix giving

$$ \begin{pmatrix} 
    \cos(\theta) & \sin(\theta) & 0 & 0 \\
    -\sin(\theta) & \cos(\theta) & 0 & 0 \\
    0 & 0 & 1 & 0 \\
    0 & 0 & 0 & 1
\end{pmatrix}. $$

The rotation matrices for the rotation around the $x$ and $y$ axes are derived using a similar process.
````

```{note}
You may find some sources present the rotation matrices in a slightly different way where the negative sign for the $\sin(\theta)$ is swapped (see <a href="https://en.wikipedia.org/wiki/Rotation_matrix" target="_blank">Wikipedia</a> for an example). This is because these assume that the vertices are multiplied by the transformation matrix on the left, e.g., $(x', y', z', 1)^\mathsf{T} = T\cdot (x, y, z, 1)^\mathsf{T}$. With OpenGL we multiply by the transformation matrix on the right, e.g., $(x', y', y', 1) = (x, y, z, 1) \cdot T$ (although of course in our code this would be `T * [x, y, z, 1]` - this can get quite confusing!). 
```

For example, let's rotate our triangle anti-clockwise about the $z$-axis by $\theta = 45^\circ$. The rotation matrix is

$$ \begin{pmatrix}
    \cos(45^\circ) & \sin(45^\circ) & 0 & 0 \\
    -\sin(45^\circ) & \cos(45^\circ) & 0 & 0 \\
    0 & 0 & 1 & 0 \\
    0 & 0 & 0 & 1
\end{pmatrix} =
\begin{pmatrix}
    0.707 & 0.707 & 0 & 0 \\
    -0.707 & 0.707 & 0 & 0 \\
    0 & 0 & 1 & 0 \\
    0 & 0 & 0 & 1
\end{pmatrix} $$

Applying the transformation

$$ \begin{align*}
    \begin{pmatrix}
        1 & 1 & 0 & 1 \\
        3 & 1 & 0 & 1 \\
        2 & 3 & 0 & 1
    \end{pmatrix}
    \begin{pmatrix}
        0.707 & 0.707 & 0 & 0 \\
        -0.707 & 0.707 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        0 & 0 & 0 & 1
    \end{pmatrix} =
    \begin{pmatrix}
        0 & 1.414 & 0 & 1 \\
        1.414 & 2.828 & 0 & 1 \\
        -0.707 & 3.536 & 0 & 1 \\
    \end{pmatrix}
\end{align*}. $$

Add the following to your program (the function `glm::radians(angle)` converts from degrees to radians).

```cpp
// Rotation
glm::mat4 rotationMatrix = glm::mat4(1.0f);
float theta = glm::radians(45.0f);
rotationMatrix[0][0] = glm::cos(theta);
rotationMatrix[0][1] = glm::sin(theta);
rotationMatrix[1][0] = - glm::sin(theta);
rotationMatrix[1][1] = glm::cos(theta);
glm::mat4 rotatedVertices = rotationMatrix * vertices;

std::cout << "\nRotating a triangle about the origin\n------------------------------------" << std::endl;
std::cout << "rotationMatrix = " << std::endl;
print_matrix(rotationMatrix, 4, 4);
std::cout << "rotatedVertices = " << std::endl;
print_matrix(rotatedVertices, 3, 4);
```

Output

```text
Rotation
--------
zRotationMatrix =
[ 0.707107 0.707107 0 0 ]
[ -0.707107 0.707107 0 0 ]
[ 0 0 1 0 ]
[ 0 0 0 1 ]

rotatedVertices =
[ 0 1.41421 0 1 ]
[ 1.41421 2.82843 0 1 ]
[ -0.707107 3.53553 0 1 ]
```

```{figure} ../images/rotated_triangle1.svg
:height: 250
:name: rotated-triangle1-figure

A triangle rotated anti-clockwise by $45^\circ$.
```

### Axis-angle rotation

The three rotation transformations are only useful if we want to only rotate around one of the three co-ordinate axes. A more useful transformation is the rotation around the axis that points in the direction of a vector ({numref}`axis-angle-rotation-figure`).

```{figure} ../images/axis_angle_rotation.svg
:height: 250
:name: axis-angle-rotation-figure

Axis-angle rotation.
```

The transformation matrix for rotation around a **unit vector** $\mathbf{v} = (v_x, v_y, v_z)$ anti-clockwise by angle $\theta$ when looking down the vector is.

$$ \begin{align*}
    R =
    \begin{pmatrix}
        v_x^2 (1 - \cos(\theta)) + \cos(\theta) &
        v_x v_y (1 - \cos(\theta)) + v_z\sin(\theta) &
        v_x v_z (1 - \cos(\theta)) - v_y\sin(\theta) &
        0 \\
        v_y v_x (1 - \cos(\theta)) - v_z\sin(\theta) &
        v_y^2 (1 - \cos(\theta)) + \cos(\theta) &
        v_y v_z (1 - \cos(\theta)) + v_x\sin(\theta) &
        0 \\
        v_z v_x (1 - \cos(\theta)) + v_y\sin(\theta) &
        v_z v_y (1 - \cos(\theta)) - v_x\sin(\theta) &
        v_z^2 (1 - \cos(\theta)) + \cos(\theta) &
        0 \\
        0 & 0 & 0 & 1
    \end{pmatrix}
\end{align*} $$(eq:axis-angle-rotation-matrix)

````{dropdown} Derivation of the axis-angle rotation matrix (click to show)

The rotation about the vector $\mathbf{v} = (v_x, v_y, v_z)$ by angle $\theta$ is the composition of 5 separate rotations:

1. Rotate $\mathbf{v}$ around the $x$-axis so that it is in the $xz$-plane (the $y$ component of the vector is 0);
1. Rotate the vector around the $y$-axis so that it points along the $z$-axis (the $x$ and $y$ components are 0 and the $z$ component is a positive number);
1. Perform the rotation around the $z$-axis;
1. Reverse the rotation around the $y$-axis;
1. Reverse the rotation around the $x$-axis.

The rotation around the $x$-axis is achieved by forming a right-angled triangle in the $yz$-plane where the the angle of rotation $\theta$ has an adjacent side of length $v_z$, an opposite side of length $v_y$ and a hypotenuse of length $\sqrt{v_y^2 + v_z^2}$ ({numref}`axis-angle-rotation1-figure`). 

```{figure} ../images/axis_angle_rotation1.svg
:height: 250
:name: axis-angle-rotation1-figure

Rotate $\mathbf{v}$ around the $x$-axis
```

Therefore $\cos(\theta) = \dfrac{v_z}{\sqrt{v_y^2 + v_z^2}}$ and $\sin(\theta) = \dfrac{v_y}{\sqrt{v_y^2 + v_z^2}}$ so the rotation matrix is

$$ R_1 = \begin{pmatrix} 
    1 & 0 & 0 & 0 \\ 
    0 & \dfrac{v_z}{\sqrt{v_y^2 + v_z^2}} & \dfrac{v_y}{\sqrt{v_y^2 + v_z^2}} & 0 \\
    0 & -\dfrac{v_x}{\sqrt{v_y^2 + v_z^2}} & \dfrac{v_z}{\sqrt{v_y^2 + v_z^2}} & 0 \\
    0 & 0 & 0 & 1
\end{pmatrix}.$$

The rotation around the $y$-axis is achieved by forming another right-angled triangle in the $xz$_plane where $\theta$ has an adjacent side of length $\sqrt{v_y^2 + v_z^2}$, an opposite side of length $v_x$ and a hypotenuse of length $|\mathbf{v}|$ ({numref}`axis-angle-rotation2-figure`).

```{figure} ../images/axis_angle_rotation2.svg
:height: 250
:name: axis-angle-rotation2-figure

Rotate around the $y$-axis
```

Therefore $\cos(\theta) = \dfrac{\sqrt{v_y^2 + v_z^2}}{|\mathbf{v}|}$ and $\sin(\theta) = \dfrac{v_x}{|\mathbf{v}|}$. Note that here we are rotating in the clockwise direction so the rotation matrix is

$$ R_2 = \begin{pmatrix} 
    \dfrac{\sqrt{v_y^2 + v_z^2}}{|\mathbf{v}|} & 0 & \dfrac{v_x}{|\mathbf{v}|} & 0 \\
    0 & 1 & 0 & 0 \\
    -\dfrac{v_x}{|\mathbf{v}|} & 0 & \dfrac{\sqrt{v_y^2 + v_z^2}}{|\mathbf{v}|} & 0 \\
    0 & 0 & 0 & 1
\end{pmatrix}.$$

Now that the vector points along the $z$-axis we perform the rotation so the rotation matrix is

$$ R_3 = \begin{pmatrix}
    \cos(\theta) & \sin(\theta) & 0 & 0 \\
    -\sin(\theta) & \cos(\theta) & 0 & 0 \\
    0 & 0 & 1 & 0 \\
    0 & 0 & 0 & 1
\end{pmatrix}. $$

The reverse rotation around the $y$ and $x$ axes is simply the rotation matrices $R_2$ and $R_1$ with the negative sign for the $\sin(\theta)$ swapped

$$ \begin{align*}
    R_4 &= \begin{pmatrix} 
       \dfrac{\sqrt{v_y^2 + v_z^2}}{|\mathbf{v}|} & 0 & -\dfrac{v_x}{|\mathbf{v}|} & 0 \\
       0 & 1 & 0 & 0 \\
       \dfrac{v_x}{|\mathbf{v}|} & 0 & \dfrac{\sqrt{v_y^2 + v_z^2}}{|\mathbf{v}|} & 0 \\
       0 & 0 & 0 & 1
   \end{pmatrix}, \\
   R_5 &= \begin{pmatrix} 
       1 & 0 & 0 & 0 \\ 
       0 & \dfrac{v_z}{\sqrt{v_y^2 + v_z^2}} & -\dfrac{v_y}{\sqrt{v_y^2 + v_z^2}} & 0 \\
       0 & \dfrac{v_x}{\sqrt{v_y^2 + v_z^2}} & \dfrac{v_z}{\sqrt{v_y^2 + v_z^2}} & 0 \\
       0 & 0 & 0 & 1
   \end{pmatrix}.
\end{align*} $$

Multiplying all of the separate matrices together gives

$$ \begin{align*}
    R &= R_1 \cdot R_2 \cdot R_3 \cdot R_4 \cdot R_5 \\
    &=
    \begin{pmatrix}
        \dfrac{v_x^2 + (v_y^2 + v_z^2) \cos(\theta)}{|\mathbf{v}|^2} &
        \dfrac{v_xv_y(1 - \cos(\theta))}{|\mathbf{v}|^2} + \dfrac{v_z\sin(\theta)}{|\mathbf{v}|} &
        \dfrac{v_xv_z(1 - \cos(\theta))}{|\mathbf{v}|^2} - \dfrac{v_y\sin(\theta)}{|\mathbf{v}|} & 
        0 \\
        \dfrac{v_xv_y(1 - \cos(\theta))}{|\mathbf{v}|^2} - \dfrac{v_z\sin(\theta)}{|\mathbf{v}|} &
        \dfrac{v_y^2 + (v_x^2 + v_y^2)\cos(\theta)}{|\mathbf{v}|^2} &
        \dfrac{v_yv_z(1 - \cos(\theta))}{|\mathbf{v}|^2} - \dfrac{v_x\sin(\theta)}{|\mathbf{v}|} &
        0 \\
        \dfrac{v_xv_z(1 - \cos(\theta))}{|\mathbf{v}|^2} + \dfrac{v_y\sin(\theta)}{|\mathbf{v}|} &
        \dfrac{v_yv_z(1 - \cos(\theta))}{|\mathbf{v}|^2} - \dfrac{v_x\sin(\theta)}{|\mathbf{v}|} &
        \dfrac{v_z^2 + (v_x^2 + v_y^2)\cos(\theta)}{|\mathbf{v}|^2} & 
        0 \\
        0 & 0 & 0 & 1
    \end{pmatrix}
\end{align*} $$

If we make $\mathbf{v}$ a unit vector so that $|\mathbf{v}| = 1$ and $v_y^2 + v_z^2 = 1 - v_x^2$ then this simplifies to 

$$ \begin{align*}
    R =
    \begin{pmatrix}
        v_x^2 (1 - \cos(\theta)) + \cos(\theta) &
        v_x v_y (1 - \cos(\theta)) + v_z\sin(\theta) &
        v_x v_z (1 - \cos(\theta)) - v_y\sin(\theta) &
        0 \\
        v_y v_x (1 - \cos(\theta)) - v_z\sin(\theta) &
        v_y^2 (1 - \cos(\theta)) + \cos(\theta) &
        v_y v_z (1 - \cos(\theta)) + v_x\sin(\theta) &
        0 \\
        v_z v_x (1 - \cos(\theta)) + v_y\sin(\theta) &
        v_z v_y (1 - \cos(\theta)) - v_x\sin(\theta) &
        v_z^2 (1 - \cos(\theta)) + \cos(\theta) &
        0 \\
        0 & 0 & 0 & 1
    \end{pmatrix}
\end{align*} $$
````

Fortunately we do not need to code this matrix into C++ as glm has a function to calculate the axis-angle rotation. The function is `glm::rotate(matrix, angle, vector)` where `angle` is in <a href="https://en.wikipedia.org/wiki/Radian" target="_blank">radians</a> and the `vector` is the direction which were are rotating around. Replace the commands for defining `rotationMatrix` in your program with the following

```cpp
glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
```

Here we have used the vector (0, 0, 1) as we wanted to rotate around the $z$-axis.

A plot of the original triangle and a rotated triangle is shown in {numref}`rotated-triangle1-figure`. Note how the position of the centre of the triangle has changed. As with the scaling of the triangle about its centre, we need to form a composite transformation with translating to and from the origin.

$$ \begin{align*}
    A &= T_1 \cdot R \cdot T_2 \\
    &=
    \begin{pmatrix}
        1 & 0 & 0 & 0 \\
        0 & 1 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        -2 & -1.67 & 0 & 1
    \end{pmatrix}
    \begin{pmatrix}
        0.707 & 0.707 & 0 & 0 \\
        -0.707 & 0.707 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        0 & 0 & 0 & 1
    \end{pmatrix}
    \begin{pmatrix}
        1 & 0 & 0 & 0 \\
        0 & 1 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        2 & 1.67 & 0 & 1
    \end{pmatrix} \\
    &=
    \begin{pmatrix}
        0.707 & 0.707 & 0 & 0 \\
        -0.707 & 0.707 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        1.764 & -0.926 & 0 & 1
    \end{pmatrix}
\end{align*} $$

Applying the transformation

$$ \begin{align*}
    \begin{pmatrix}
        1 & 1 & 0 & 1 \\
        3 & 1 & 0 & 1 \\
        2 & 3 & 0 & 1
    \end{pmatrix}
    \begin{pmatrix}
        0.707 & 0.707 & 0 & 0 \\
        -0.707 & 0.707 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        1.764 & -0.926 & 0 & 1
    \end{pmatrix} =
    \begin{pmatrix}
        0 & 1.414 & 0 & 1 \\
        1.414 & 2.828 & 0 & 1 \\
        -0.707 & 3.536 & 0 & 1 \\
    \end{pmatrix}
\end{align*}. $$

```cpp
// Rotate a triangle about its centre
transformationMatrix = translationMatrix2 * zRotationMatrix * translationMatrix1;

std::cout << "transformationMatrix = " << std::endl;
print_matrix(transformationMatrix, 4, 4);
std::cout << "transformedVertices = " << std::endl;
print_matrix(transformedVertices, 3, 4);
```

Output

```text
transformationMatrix =
[ 0.707107 0.707107 0 0 ]
[ -0.707107 0.707107 0 0 ]
[ 0 0 1 0 ]
[ 1.7643 -0.926058 0 1 ]

transformedVertices =
[ 1.7643 0.488155 0 1 ]
[ 3.17851 1.90237 0 1 ]
[ 1.05719 2.60948 0 1 ]
```

```{figure} ../images/rotated_triangle2.svg
:height: 250
:name: rotated-triangle2-figure

A triangle rotated anti-clockwise by 45$^\circ$ about its centre.
```

---

## Exercises

Use C++ to answer the following questions.

1. A triangle has vertices at (1, 1, 2), (2, 0, 1) and (3, 1, 2). Calculate a unit normal vector to this triangle (a unit normal vector is perpendicular to a polygon and is scaled so it has a length of 1). To ensure our unit vector is pointing in the correct direction, if $\mathbf{v}_1$, $\mathbf{v}_2$ and $\mathbf{v}_3$ are the vertices of the triangle listed in an anti-clockwise direction then the normal vector is calculated using

$$ \mathbf{n} = (\mathbf{v}_2 - \mathbf{v}_1) \times (\mathbf{v}_3 - \mathbf{v}_2) $$

1. A square with side lengths of 2 parallel to the $x$ and $y$ axes is centred at (4, 3, 0). It is to be scaled so that it is double its original height and half its original width, and rotated anti-clockwise about its centre. Calculate the single transformation matrix to achieve this and calculate the vertices of the transformed square.

1. Write your own C++ class called `mylib` with methods for calculating the following:

    - the length of a vector (equation {eq}`eq:vector-magnitude`);
    - the cross product (equation {eq}`eq:cross-product`);
    - the translation transformation matrix (equation {eq}`eq:translation-matrix`);
    - the scaling transformation matrix (equation {eq}`eq:scaling-matrix`);
    - the angle-axis rotation matrix (equation {eq}`eq:axis-angle-rotation-matrix`).

 Use your class to answer questions 1 and 2.
