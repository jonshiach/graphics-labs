(transformations-section)=

# Transformations

Computer graphics requires that shapes are manipulated in space by moving the shapes, shrinking or stretching, rotating and reflection to name a few. We call these manipulations **transformations**. We need a convenient way of telling the computer how to apply our transformations and for this we make use of matrices (hence why we needed to revise them here).

Each transformation has an associated **transformation matrix** which we used to multiply the co-ordinates of our shapes to calculate the co-ordinates of the transformed shape. For example if $A$ is a transformation matrix for a particular transformation and $(x,y,z)$ are the co-ordinates of a point then we apply the transformation using

$$ (x',y',z') = (x, y, z) \cdot A. $$

where $(x',y',z')$ are the co-ordinates of the transformed point.

````{note}
The co-ordinate system used by OpenGL is a <a href="https://en.wikipedia.org/wiki/Right-hand_rule" target="_blank">right-hand</a> 3D co-ordinate system (on your right hand the thumb represents the $x$-axis, the index finger the $y$-axis and the middle finger the $z$-axis) with the $x$-axis pointing to the right, the $y$-axis point upwards and the $z$-axis pointing out of the screen towards the viewer.

```{figure} ../images/opengl_axes.svg
:height: 220

The OpenGL co-ordinate system.
```
````

(translation-section)=

### Translation

The **translation** transformation when applied to a set of points moves each point by the same amount. For example, consider the triangle in {numref}`translation-figure`, each of the vertices has been translated by the same **translation vector** $\underline{t}$ which has that effect of moving the triangle.

```{figure} ../images/translation.svg
:height: 280
:name: translation-figure

Translation of a triangle by the translation vector $\underline{t}= (t_x, t_y)$.
```

A problem we have is that no transformation matrix exists for applying translation to the co-ordinates $(x, y, z)$, e.g., we can't find a matrix $T$ such that

$$(x, y, z) \cdot T = (x + t_x, y + t_y, z + t_z).$$

Don't worry, all is not lost. We can use a trick where we use <a href="https://en.wikipedia.org/wiki/Homogeneous_coordinates" target="_blank">**homogeneous**</a> co-ordinates instead. Homogeneous co-ordinates add another value, $w$ say, to the co-ordinates (known as Cartesian co-ordinates) such that when the $x$, $y$ and $z$ values are divided by $w$ we get the Cartesian co-ordinates.

$$\underbrace{(x, y, z, w)}_{\textsf{homogeneous}} \equiv \underbrace{\left( \frac{x}{w}, \frac{y}{w}, \frac{z}{w} \right)}_{\textsf{Cartesian}}.$$

So if we choose $w=1$ then we can write the Cartesian co-ordinates $(x, y, z)$ as the homogeneous co-ordinates $(x, y, z, 1)$ (remember that 4-element vector with the additional 1 in our [vertex shader](vertex-shader-section)?). So how does that help us with our elusive translation matrix? Well we can now represent translation as a $4 \times 4$ matrix

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

which is our desired translation. So the **translation matrix** for translating a set of points by the vector $\underline{t} = (t_x, t_y, t_z)$ is

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
glm::mat3x4 vertices;
vertices[0] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
vertices[1] = glm::vec4(3.0f, 1.0f, 0.0f, 1.0f);
vertices[2] = glm::vec4(2.0f, 3.0f, 0.0f, 1.0f);

// Define translation matrix
glm::mat4 translationMatrix = glm::mat4(1.0f);
translationMatrix[3][0] = 4.0f;
translationMatrix[3][1] = 1.0f;
translationMatrix[3][2] = 0.0f;  

// Apply translation
glm::mat3x4 translatedVertices = translationMatrix * vertices;

std::cout << "\nTranslating a triangle\n---------------------------" << std::endl;
std::cout << "vertices = " << glm::transpose(vertices) << std::endl;
std::cout << "\ntranslationMatrix = " << glm::transpose(translationMatrix) << std::endl;
std::cout << "\ntranslatedVertices = " << glm::transpose(translatedVertices) << std::endl;
```

Output

```text
Translating a triangle
---------------------------
vertices = 
[[    1.000,    1.000,    0.000,    1.000]
 [    3.000,    1.000,    0.000,    1.000]
 [    2.000,    3.000,    0.000,    1.000]]

translationMatrix = 
[[    1.000,    0.000,    0.000,    0.000]
 [    0.000,    1.000,    0.000,    0.000]
 [    0.000,    0.000,    1.000,    0.000]
 [    4.000,    1.000,    0.000,    1.000]]

translatedVertices = 
[[    5.000,    2.000,    0.000,    1.000]
 [    7.000,    2.000,    0.000,    1.000]
 [    6.000,    4.000,    0.000,    1.000]]
 ```

Whilst it wasn't particularly difficult to define the `translationMatrix` we will be using it often so it make sense that we should use a function to do this. Fortunately there is one already in glm which we can used, `glm::translate(matrix, vector)` outputs the translation matrix for the translation by `vector` applied to `matrix`. So we could replace the two lines of code where we calculate `translationMatrix` with the following

```cpp
glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 1.0f, 0.0f));
```

Note that here we applied the translation to the identity matrix.

### Scaling

Scaling is the simplest transformation we can apply. Multiplying the $x$, $y$ and $z$ co-ordinates of a point by a scalar quantity (a number) has the effect of moving the point closer or further away from the origin (0,0). For example, consider the triangle in {numref}`scaling-figure`. The $x$ co-ordinate of each vertex has been multiplied by $s_x$ and the $y$ co-ordinates have been multiplied by $s_y$ which has the effect of scaling the triangle and moving it further away from the origin (in this case $s_x$ and $s_y$ are both greater than 1).

```{figure} ../images/scaling1.svg
:height: 350
:name: scaling-figure

Scaling of a triangle by the scaling vector $\underline{s}=(s_x,s_y)$.
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
std::cout << "scalingMatrix = " << glm::transpose(scalingMatrix) << std::endl;
std::cout << "\nscaledVertices = " << glm::transpose(scaledVertices) << std::endl;
```

Output

```text
Scaling
-------
scalingMatrix = 
[[    3.000,    0.000,    0.000,    0.000]
 [    0.000,    2.000,    0.000,    0.000]
 [    0.000,    0.000,    1.000,    0.000]
 [    0.000,    0.000,    0.000,    1.000]]

scaledVertices = 
[[    3.000,    2.000,    0.000,    1.000]
 [    9.000,    2.000,    0.000,    1.000]
 [    6.000,    6.000,    0.000,    1.000]]
```

Like with the translation matrix, we can use the glm command `glm::scale(matrix, vector)` to generate `scalingMatrix`

```cpp
glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 2.0f, 1.0f));
```

The center of the original triangle was at (2, 1.67, 0) (the sum of the vertices divided by 3) and the center of the scaled triangle is at (6, 3.33, 0) so the scaling transformation has moved our triangle (see {numref}`scaling-figure`). A more useful transformation is to scale shapes so that the position of the center of the shape doesn't move (think of an object in a computer game growing and shrinking). If we have an object which has a centre at the origin then the effect of scaling the vertex co-ordinates means that the centre remains at the origin as shown in {numref}`scaling-about-origin-figure`.

```{figure} ../images/scaling2.svg
:height: 350
:name: scaling-about-origin-figure

Scaling a triangle centred at the origin.
```

So to scale the example triangle so that its centre remains unchanged we first need to translate it to the origin by the translation vector (-2,-1.67,0) (the center co-ordinates with the signs changed), perform the scaling and then translate it back to the its original position by the translation vector (2, 1.67, 0).

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

Lets do this in our program (copy and paste may come in handy here).

```cpp
// Scale a triangle about its centre
// 1. Translate triangle centre to origin

// Calculate centre co-ordinates
glm::vec3 centre = glm::vec3(0.0f);
for (int i = 0; i < 3; i++) // loop through rows
{
    for (int j = 0; j < 3; j++) // loop through columns
    {
        centre[j] += vertices[i][j];
    }
}
centre /= 3;

glm::mat4 translationMatrix1 = glm::translate(glm::mat4(1.0f), -centre);
glm::mat3x4 transformedVertices = translationMatrix1 * vertices;

std::cout << "\nScaling a triangle about its centre\n-----------------------------------" << std::endl;
std::cout << "1. Translate centre to origin:" << std::endl;
std::cout << "\ncentre = " << centre << std::endl;
std::cout << "\ntranslationMatrix1 = " << glm::transpose(translationMatrix1) << std::endl;
std::cout << "\ntransformedVertices = " << glm::transpose(transformedVertices) << std::endl;

// 2. Scale triangle
scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 2.0f, 1.0f));
transformedVertices = scalingMatrix * transformedVertices;

std::cout << "\n2. Scale triangle:" << std::endl;
std::cout << "\nscalingMatrix = " << glm::transpose(scalingMatrix) << std::endl;
std::cout << "\ntransformedVertices = " << glm::transpose(transformedVertices) << std::endl;

// 3. Translate triangle centre back to original position
glm::mat4 translationMatrix2 = glm::translate(glm::mat4(1.0f), centre);
transformedVertices = translationMatrix2 * transformedVertices;

std::cout << "\n3. Translate centre back to original position:" << std::endl;
std::cout << "\nTranslationMatrix2 = " << glm::transpose(translationMatrix2) << std::endl;
std::cout << "\ntransformedVertices = " << glm::transpose(transformedVertices) << std::endl;
```

Output

```text
Scaling a triangle about its centre
-----------------------------------
1. Translate centre to origin:

centre = [    2.000,    1.667,    0.000]

translationMatrix1 = 
[[    1.000,    0.000,    0.000,    0.000]
 [    0.000,    1.000,    0.000,    0.000]
 [    0.000,    0.000,    1.000,    0.000]
 [   -2.000,   -1.667,    0.000,    1.000]]

transformedVertices = 
[[   -1.000,   -0.667,    0.000,    1.000]
 [    1.000,   -0.667,    0.000,    1.000]
 [    0.000,    1.333,    0.000,    1.000]]

2. Scale triangle:

scalingMatrix = 
[[    3.000,    0.000,    0.000,    0.000]
 [    0.000,    2.000,    0.000,    0.000]
 [    0.000,    0.000,    1.000,    0.000]
 [    0.000,    0.000,    0.000,    1.000]]

transformedVertices = 
[[   -3.000,   -1.333,    0.000,    1.000]
 [    3.000,   -1.333,    0.000,    1.000]
 [    0.000,    2.667,    0.000,    1.000]]

3. Translate centre back to original position:

TranslationMatrix2 = 
[[    1.000,    0.000,    0.000,    0.000]
 [    0.000,    1.000,    0.000,    0.000]
 [    0.000,    0.000,    1.000,    0.000]
 [    2.000,    1.667,    0.000,    1.000]]

transformedVertices = 
[[   -1.000,    0.333,    0.000,    1.000]
 [    5.000,    0.333,    0.000,    1.000]
 [    2.000,    4.333,    0.000,    1.000]]
```

### Combining transformations

We have seen that to scale an object about its centre we needed to use three transformations which means three separate matrix multiplications. Instead of performing the three transformations separately we can calculate a single transformation matrix which combines these transformations. If we say that $T_1$, $S$ and $T_2$ are the transformation matrices for the translation of the centre to the origin, scaling and translation of the centre back to the original position then the **composite** of our three transformations is

$$ \begin{align*}
    \begin{pmatrix}
        x_0' & y_0' & z_0' & 1 \\
        x_1' & y_1' & z_1' & 1 \\
        \vdots & \vdots & \vdots & \vdots \\
        x_n' & y_n' & z_n' & 1
    \end{pmatrix}
    &=  \begin{pmatrix}
            x_0 & y_0 & z_0 & 1 \\
            x_1 & y_1 & z_1 & 1 \\
            \vdots & \vdots & \vdots & \vdots \\
            x_n & y_n & z_n & 1
        \end{pmatrix}
        \cdot T_1 \cdot S \cdot T_2.
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

std::cout << "\nComposite transformation\n------------------------" << std::endl;
std::cout << "\ntransformationMatrix = " << glm::transpose(transformationMatrix) << std::endl;
std::cout << "\ntransformedVertices = " << glm::transpose(transformedVertices) << std::endl;
```

(column-major-important-note)=

````{important}
Remember that OpenGL and glm use column-major ordering so we reverse the order of our matrices when multiplying. So $T_1 \cdot S \cdot T_2$ would be coded as

```cpp
T2 * S * T1
```

i.e., the transformations are applied from right to left.
````

Output

```text
Composite transformation
-----------------------

transformationMatrix = 
[[    3.000,    0.000,    0.000,    0.000]
 [    0.000,    2.000,    0.000,    0.000]
 [    0.000,    0.000,    1.000,    0.000]
 [   -4.000,   -1.667,    0.000,    1.000]]

transformedVertices = 
[[   -1.000,    0.333,    0.000,    1.000]
 [    5.000,    0.333,    0.000,    1.000]
 [    2.000,    4.333,    0.000,    1.000]]
```

### Rotation

As well as moving objects and scaling them, the next most common transformation is the rotation of objects around the three co-ordinate axes $x$, $y$ and $z$. We define the rotation **anti-clockwise** around each of the co-ordinate axes by an angle $\theta$ when looking down the axes ({numref}`3D-rotation-figure`).

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

You don't really need to know how these are derived but if you are curious you can click on the dropdown link below.

````{dropdown} Derivation of the rotation matrices (click to show)

We will consider rotation about the $z$-axis and will restrict our co-ordinates to 2D.

```{figure} ../images/rotation.svg
:height: 300
:name: rotation-figure

Rotating the vector $\underline{u}$ anti-clockwise by angle $\theta$ to the vector $\underline{v}$.
```

Consider {numref}`rotation-figure` where the vector $\underline{u}$ is rotated by angle $\theta$ to the vector $\underline{v}$. To get this rotation we first consider the rotation of the vector $\underline{t}$, which has the same length as $\underline{u}$ and points along the $x$-axis, by angle $\phi$ to get to $\underline{u}$. If we form a right-angled triangle (the blue one) then we know the length of the hypotenuse, $|\underline{u}|$, and the angle so we can calculate the lengths of the adjacent and opposite sides using trigonometry. Remember our trig ratios (SOH-CAH-TOA)

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

Now $u_x$ and $u_y$ are the lengths of the adjacent and opposite sides respectively and $|\underline{u}|$ is the length of the hypotenuse then we have

$$ \begin{align*}
    u_x &= |\underline{u}| \cos(\phi), \\
    u_y &= |\underline{u}| \sin(\phi).
\end{align*} $$

Now consider the rotation from $\underline{t}$ by the angle $\phi + \theta$ to get to $\underline{v}$. Using the same method as before we have

$$ \begin{align*}
    v_x &= |\underline{u}| \cos(\phi + \theta), \\
    v_y &= |\underline{u}| \sin(\phi + \theta).
\end{align*} $$

We can rewrite $\cos(\phi+\theta)$ and $\sin(\phi+\theta)$ using <a href="https://en.wikipedia.org/wiki/List_of_trigonometric_identities#" target="_blank">trigonometric identities</a>

$$ \begin{align*}
    \cos(\phi + \theta) &= \cos(\phi) \cos(\theta) - \sin(\phi) \sin(\theta), \\
    \sin(\phi + \theta) &= \sin(\phi) \cos(\theta) + \cos(\phi) \sin(\theta),
\end{align*} $$

so 

$$ \begin{align*}
    v_x &= |\underline{u}| \cos(\phi) \cos(\theta) - |\underline{u}| \sin(\phi) \sin(\theta), \\
    v_y &= |\underline{u}| \sin(\phi) \cos(\theta) + |\underline{u}| \cos(\phi) \sin(\theta).
\end{align*} $$

Since $ u_x = |\underline{u}| \cos(\phi)$ and $u_y = |\underline{u}| \sin(\phi)$ then

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
You may find some sources present the rotation matrices in a slightly different way where the negative sign for the $\sin(\theta)$ is swapped (see <a href="https://en.wikipedia.org/wiki/Rotation_matrix" target="_blank">Wikipedia</a> for an example). This is because these assume that the vertices are multiplied by the transformation matrix on the left, e.g., $(x', y', z', 1)^\mathsf{T} = T\cdot (x, y, z, 1)^\mathsf{T}$. With OpenGL we multiply by the transformation matrix on the right, e.g., $(x', y', y', 1) = (x, y, z, 1) \cdot T$ (although of course in our code this would be `T * [x, y, z, 1]` since OpenGL uses column-major order - this can get quite confusing!). 
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
glm::mat3x4 rotatedVertices = rotationMatrix * vertices;

std::cout << "\nRotating a triangle about the origin\n------------------------------------" << std::endl;
std::cout << "rotationMatrix = " << glm::transpose(rotationMatrix) << std::endl;
std::cout << "\nrotatedVertices = " << glm::transpose(rotatedVertices) << std::endl;
```

Output

```text
Rotating a triangle about the origin
------------------------------------
rotationMatrix = 
[[    0.707,    0.707,    0.000,    0.000]
 [   -0.707,    0.707,    0.000,    0.000]
 [    0.000,    0.000,    1.000,    0.000]
 [    0.000,    0.000,    0.000,    1.000]]

rotatedVertices = 
[[   -0.000,    1.414,    0.000,    1.000]
 [    1.414,    2.828,    0.000,    1.000]
 [   -0.707,    3.536,    0.000,    1.000]]
```

```{figure} ../images/rotated_triangle1.svg
:height: 250
:name: rotated-triangle1-figure

A triangle rotated anti-clockwise by $45^\circ$.
```

### Axis-angle rotation

The three rotation transformations are only useful if we want to only rotate around one of the three co-ordinate axes. A more useful transformation is the rotation around the axis that points in the direction of a vector, $\underline{v}$ say, which has its tail at (0,0,0) ({numref}`axis-angle-rotation-figure`).

```{figure} ../images/axis_angle_rotation_1.svg
:height: 250
:name: axis-angle-rotation-figure

Axis-angle rotation.
```

The transformation matrix for rotation around a **unit vector**m $\hat{\underline{v}} = (v_x, v_y, v_z)$, anti-clockwise by angle $\theta$ when looking down the vector is.

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

Again, you don't really need to know how this is derived but if you are curious click on the dropdown link below.

````{dropdown} Derivation of the axis-angle rotation matrix (click to show)

The rotation about the vector $\underline{v} = (v_x, v_y, v_z)$ by angle $\theta$ is the composition of 5 separate rotations:

1. Rotate $\underline{v}$ around the $x$-axis so that it is in the $xz$-plane (the $y$ component of the vector is 0);
1. Rotate the vector around the $y$-axis so that it points along the $z$-axis (the $x$ and $y$ components are 0 and the $z$ component is a positive number);
1. Perform the rotation around the $z$-axis;
1. Reverse the rotation around the $y$-axis;
1. Reverse the rotation around the $x$-axis.

The rotation around the $x$-axis is achieved by forming a right-angled triangle in the $yz$-plane where the the angle of rotation $\theta$ has an adjacent side of length $v_z$, an opposite side of length $v_y$ and a hypotenuse of length $\sqrt{v_y^2 + v_z^2}$ ({numref}`axis-angle-rotation1-figure`). 

```{figure} ../images/axis_angle_rotation_2.svg
:height: 250
:name: axis-angle-rotation1-figure

Rotate $\underline{v}$ around the $x$-axis
```

Therefore $\cos(\theta) = \dfrac{v_z}{\sqrt{v_y^2 + v_z^2}}$ and $\sin(\theta) = \dfrac{v_y}{\sqrt{v_y^2 + v_z^2}}$ so the rotation matrix is

$$ R_1 = \begin{pmatrix} 
    1 & 0 & 0 & 0 \\ 
    0 & \dfrac{v_z}{\sqrt{v_y^2 + v_z^2}} & \dfrac{v_y}{\sqrt{v_y^2 + v_z^2}} & 0 \\
    0 & -\dfrac{v_x}{\sqrt{v_y^2 + v_z^2}} & \dfrac{v_z}{\sqrt{v_y^2 + v_z^2}} & 0 \\
    0 & 0 & 0 & 1
\end{pmatrix}.$$

The rotation around the $y$-axis is achieved by forming another right-angled triangle in the $xz$-plane where $\theta$ has an adjacent side of length $\sqrt{v_y^2 + v_z^2}$, an opposite side of length $v_x$ and a hypotenuse of length $|\underline{v}|$ ({numref}`axis-angle-rotation2-figure`).

```{figure} ../images/axis_angle_rotation_3.svg
:height: 250
:name: axis-angle-rotation2-figure

Rotate around the $y$-axis
```

Therefore $\cos(\theta) = \dfrac{\sqrt{v_y^2 + v_z^2}}{|\underline{v}|}$ and $\sin(\theta) = \dfrac{v_x}{|\underline{v}|}$. Note that here we are rotating in the clockwise direction so the rotation matrix is

$$ R_2 = \begin{pmatrix} 
    \dfrac{\sqrt{v_y^2 + v_z^2}}{|\underline{v}|} & 0 & \dfrac{v_x}{|\underline{v}|} & 0 \\
    0 & 1 & 0 & 0 \\
    -\dfrac{v_x}{|\underline{v}|} & 0 & \dfrac{\sqrt{v_y^2 + v_z^2}}{|\underline{v}|} & 0 \\
    0 & 0 & 0 & 1
\end{pmatrix}.$$

Now that the vector points along the $z$-axis we perform the rotation so the rotation matrix for this is

$$ R_3 = \begin{pmatrix}
    \cos(\theta) & \sin(\theta) & 0 & 0 \\
    -\sin(\theta) & \cos(\theta) & 0 & 0 \\
    0 & 0 & 1 & 0 \\
    0 & 0 & 0 & 1
\end{pmatrix}. $$

The reverse rotation around the $y$ and $x$ axes is simply the rotation matrices $R_2$ and $R_1$ with the negative sign for the $\sin(\theta)$ swapped

$$ \begin{align*}
    R_4 &= \begin{pmatrix} 
       \dfrac{\sqrt{v_y^2 + v_z^2}}{|\underline{v}|} & 0 & -\dfrac{v_x}{|\underline{v}|} & 0 \\
       0 & 1 & 0 & 0 \\
       \dfrac{v_x}{|\underline{v}|} & 0 & \dfrac{\sqrt{v_y^2 + v_z^2}}{|\underline{v}|} & 0 \\
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
        \dfrac{v_x^2 + (v_y^2 + v_z^2) \cos(\theta)}{|\underline{v}|^2} &
        \dfrac{v_xv_y(1 - \cos(\theta))}{|\underline{v}|^2} + \dfrac{v_z\sin(\theta)}{|\underline{v}|} &
        \dfrac{v_xv_z(1 - \cos(\theta))}{|\underline{v}|^2} - \dfrac{v_y\sin(\theta)}{|\underline{v}|} & 
        0 \\
        \dfrac{v_xv_y(1 - \cos(\theta))}{|\underline{v}|^2} - \dfrac{v_z\sin(\theta)}{|\underline{v}|} &
        \dfrac{v_y^2 + (v_x^2 + v_y^2)\cos(\theta)}{|\underline{v}|^2} &
        \dfrac{v_yv_z(1 - \cos(\theta))}{|\underline{v}|^2} - \dfrac{v_x\sin(\theta)}{|\underline{v}|} &
        0 \\
        \dfrac{v_xv_z(1 - \cos(\theta))}{|\underline{v}|^2} + \dfrac{v_y\sin(\theta)}{|\underline{v}|} &
        \dfrac{v_yv_z(1 - \cos(\theta))}{|\underline{v}|^2} - \dfrac{v_x\sin(\theta)}{|\underline{v}|} &
        \dfrac{v_z^2 + (v_x^2 + v_y^2)\cos(\theta)}{|\underline{v}|^2} & 
        0 \\
        0 & 0 & 0 & 1
    \end{pmatrix}
\end{align*} $$

If we make $\underline{v}$ a unit vector so that $|\underline{v}| = 1$ and $v_y^2 + v_z^2 = 1 - v_x^2$ then this simplifies to 

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

Add the following to your program. Note how the order of the transformation matrices is the reverse order to how we have written it above, e.g., `T2 * R * T1`. This is once again because of the [column-major order used by OpenGL and glm](column-major-important-note).

```cpp
// Rotate a triangle about its centre
transformationMatrix = translationMatrix2 * rotationMatrix * translationMatrix1;
transformedVertices = transformationMatrix * vertices;

std::cout << "\nRotating a triangle about its centre\n------------------------------------" << std::endl;
std::cout << "transformationMatrix = " << glm::transpose(transformationMatrix) << std::endl;
std::cout << "\ntransformedVertices = " << glm::transpose(transformedVertices) << std::endl;
```

Output

```text
Rotating a triangle about its centre
------------------------------------
transformationMatrix = 
[[    0.707,    0.707,    0.000,    0.000]
 [   -0.707,    0.707,    0.000,    0.000]
 [    0.000,    0.000,    1.000,    0.000]
 [    1.764,   -0.926,    0.000,    1.000]]

transformedVertices = 
[[    1.764,    0.488,    0.000,    1.000]
 [    3.179,    1.902,    0.000,    1.000]
 [    1.057,    2.609,    0.000,    1.000]]
```

```{figure} ../images/rotated_triangle2.svg
:height: 250
:name: rotated-triangle2-figure

A triangle rotated anti-clockwise by 45$^\circ$ about its centre.
```

---

## Exercises

Use C++ to answer the following questions.

1. A triangle has vertices at (1, 1, 2), (2, 0, 1) and (3, 1, 2). Calculate a unit normal vector to this triangle (a unit normal vector is perpendicular to a polygon and is scaled so it has a length of 1). To ensure our unit vector is pointing in the correct direction, if $\underline{v}_1$, $\underline{v}_2$ and $\underline{v}_3$ are the vertices of the triangle listed in an anti-clockwise direction then the normal vector is calculated using

$$ \underline{n} = (\underline{v}_2 - \underline{v}_1) \times (\underline{v}_3 - \underline{v}_2) $$

````{dropdown} Solution
```text
Exercise 1. Unit normal vector
------------------------------
normal = [    0.000,   -0.707,    0.707]
```

Code:
```cpp
// Exercise 1 - unit normal vector
glm::vec3 v1, v2, v3, normal;
v1 = glm::vec3(1.0f, 1.0f, 2.0f);
v2 = glm::vec3(2.0f, 0.0f, 1.0f);
v3 = glm::vec3(3.0f, 1.0f, 2.0f);

normal = glm::cross(v2 - v1, v3 - v2);
normal /= glm::length(normal);

std::cout << "\nExercise 1. Unit normal vector\n------------------------------" << std::endl;
std::cout << "normal = " << normal << std::endl;
```
````

2. A square with side lengths of 2 parallel to the $x$ and $y$ axes is centred at (4, 3, 0). It is to be scaled so that it is double its original height and half its original width and then rotated anti-clockwise about its centre. Calculate the single transformation matrix to achieve this and calculate the vertices of the transformed square.

````{dropdown} Solution
```text
Exercise 2. Transform square
----------------------------
transformationMatrix = 
[[    0.433,    0.250,    0.000,    0.000]
 [   -1.000,    1.732,    0.000,    0.000]
 [    0.000,    0.000,    1.000,    0.000]
 [    5.268,   -3.196,    0.000,    1.000]]

transformedSquare = 
[[    4.567,    1.018,    0.000,    1.000]
 [    5.433,    1.518,    0.000,    1.000]
 [    3.433,    4.982,    0.000,    1.000]
 [    2.567,    4.482,    0.000,    1.000]]
```

Code:
```cpp
// Exercise 2 - Transform square
centre = glm::vec3(4.0f, 3.0f, 0.0f);
glm::mat4 square;
square[0] = glm::vec4(3.0f, 2.0f, 0.0f, 1.0f);
square[1] = glm::vec4(5.0f, 2.0f, 0.0f, 1.0f);
square[2] = glm::vec4(5.0f, 4.0f, 0.0f, 1.0f);
square[3] = glm::vec4(3.0f, 4.0f, 0.0f, 1.0f);

transformationMatrix =
  glm::translate(glm::mat4(1.0f), centre) *
  glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
  glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 2.0f, 1.0f)) *
  glm::translate(glm::mat4(1.0f), -centre);

glm::mat4 transformedSquare = transformationMatrix * square;

std::cout << "\nExercise 2. Transform square\n----------------------------" << std::endl;
std::cout << "transformationMatrix = " << glm::transpose(transformationMatrix) << std::endl;
std::cout << "\ntransformedSquare = " << glm::transpose(transformedSquare) << std::endl;
```
````

3. Create your own C++ class called MyLib using header and code files **MyLib.hpp** and **MyLib.cpp** respectively and define methods for calculating the following:

    - the length of a vector (equation {eq}`eq:vector-magnitude`);
    - the cross product (equation {eq}`eq:cross-product`);
    - the translation transformation matrix (equation {eq}`eq:translation-matrix`);
    - the scaling transformation matrix (equation {eq}`eq:scaling-matrix`);
    - the angle-axis rotation matrix (equation {eq}`eq:axis-angle-rotation-matrix`).

 Use your class to answer questions 1 and 2.

````{dropdown} Solution
Header file:
```cpp
# pragma once

#include <iostream>

class MyLib
{
public:
    static float length(const glm::vec3 vector);
    static glm::vec3 cross(const glm::vec3 a, const glm::vec3 b);
    static glm::vec3 normal(const glm::vec3 v1, const glm::vec3 v2, const glm::vec3 v3);
    static glm::mat4 translate(const glm::mat4 matrix, const glm::vec3 vector);
    static glm::mat4 scale(const glm::mat4 matrix, const glm::vec3 vector);
    static glm::mat4 rotate(const glm::mat4 matrix, const float theta, const glm::vec3 vector);
};
```

Code file:
```cpp
#include <glm/glm.hpp>
#include "MyLib.hpp"

float MyLib::length(const glm::vec3 vector)
{
    float length = glm::sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
    return length;
}

glm::vec3 MyLib::cross(const glm::vec3 a, const glm::vec3 b)
{
    glm::vec3 axb;
    axb[0] = a[1] * b[2] - a[2] * b[1];
    axb[1] = a[2] * b[0] - a[0] * b[2];
    axb[2] = a[0] * b[1] - a[1] * b[0];

    return axb;
}

glm::mat4 MyLib::translate(const glm::mat4 matrix, const glm::vec3 vector)
{
    glm::mat4 transMatrix = glm::mat4(1.0f);
    transMatrix[3][0] = vector[0];
    transMatrix[3][1] = vector[1];
    transMatrix[3][2] = vector[2];

    return transMatrix * matrix;
}

glm::mat4 MyLib::scale(const glm::mat4 matrix, const glm::vec3 vector)
{
    glm::mat4 transMatrix = glm::mat4(1.0f);
    transMatrix[0][0] = vector[0];
    transMatrix[1][1] = vector[1];
    transMatrix[2][2] = vector[2];

    return transMatrix * matrix;
}

glm::mat4 MyLib::rotate(const glm::mat4 matrix, const float theta, const glm::vec3 vector)
{
    glm::mat4 transMatrix = glm::mat4(1.0f);
    float vx, vy, vz, cos, sin;
    cos = glm::cos(theta);
    sin = glm::sin(theta);

    // Normalise  the vector
    vx = vector[0] / length(vector);
    vy = vector[1] / length(vector);
    vz = vector[2] / length(vector);

    // Calculate rotation matrix
    transMatrix[0][0] = vx * vx * (1 - cos) + cos;
    transMatrix[0][1] = vx * vy * (1 - cos) + vz * sin;
    transMatrix[0][2] = vx * vz * (1 - cos) - vy * sin;
    transMatrix[1][0] = vy * vx * (1 - cos) - vz * sin;
    transMatrix[1][1] = vy * vy * (1 - cos) + cos;
    transMatrix[1][2] = vy * vz * (1 - cos) + vx * sin;
    transMatrix[2][0] = vz * vx * (1 - cos) + vy * sin;
    transMatrix[2][1] = vz * vy * (1 - cos) - vx * sin;
    transMatrix[2][2] = vz * vz * (1 - cos) + cos;

    return transMatrix * matrix;
}
```

Then replace `glm::` with `MyLib::` for the functions used in exercises 1 and 2.

````

---
## Exercuses

Use C++ to answer the following questions.

1. Three points have the co-ordinates A = (5, 1, 3), B = (10, 7, 4) and C = (0, 5, -3). 