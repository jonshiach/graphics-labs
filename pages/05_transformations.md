(transformations-section)=

# Transformations

Computer graphics requires that shapes are manipulated in space by moving the shapes, shrinking or stretching, rotating and reflection to name a few. We call these manipulations **transformations**. We need a convenient way of telling the computer how to apply our transformations and for this we make use of matrices (hence why we needed to revise them here).

Each transformation has an associated **transformation matrix** which we used to multiply the co-ordinates of our shapes to calculate the co-ordinates of the transformed shape. For example if $A$ is a transformation matrix for a particular transformation and $(x,y,z)$ are the co-ordinates of a point then we apply the transformation using

$$ (x',y',z') = (x, y, z) \cdot A. $$

where $(x',y',z')$ are the co-ordinates of the transformed point.

````{note}
The co-ordinate system used by OpenGL is a <a href="https://en.wikipedia.org/wiki/Right-hand_rule" target="_blank">right-hand</a> 3D co-ordinate system (on your right hand the thumb represents the $x$-axis, the index finger the $y$-axis and the middle finger the $z$-axis) with the $x$-axis pointing to the right, the $y$-axis point upwards and the $z$-axis pointing out of the screen towards the user.

```{figure} ../images/05_opengl_axes.svg
:width: 300

The OpenGL co-ordinate system.
```
````

(translation-section)=

## Translation

The **translation** transformation when applied to a set of points moves each point by the same amount. For example, consider the triangle in {numref}`translation-figure`, each of the vertices has been translated by the same **translation vector** $\vec{t}$ which has that effect of moving the triangle.

```{figure} ../images/05_translation.svg
:height: 280
:name: translation-figure

Translation of a triangle by the translation vector $\vec{t}= (t_x, t_y)$.
```

A problem we have is that no transformation matrix exists for applying translation to the co-ordinates $(x, y, z)$, e.g., we can't find a matrix $T$ such that

$$(x, y, z) \cdot T = (x + t_x, y + t_y, z + t_z).$$

(homogeneous-coordinates-section)=
Don't worry, all is not lost. We can use a trick where we use <a href="https://en.wikipedia.org/wiki/Homogeneous_coordinates" target="_blank">**homogeneous**</a> co-ordinates instead. Homogeneous co-ordinates add another value, $w$ say, to the $(x, y, z)$ co-ordinates (these are known as Cartesian co-ordinates) such that when the $x$, $y$ and $z$ values are divided by $w$ we get the Cartesian co-ordinates.

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

which is our desired translation. So the **translation matrix** for translating a set of points by the vector $\vec{t} = (t_x, t_y, t_z)$ is

$$ T = \begin{pmatrix}
    1 & 0 & 0 & 0 \\
    0 & 1 & 0 & 0 \\
    0 & 0 & 1 & 0 \\
    t_x & t_y & t_z & 1
\end{pmatrix}. $$(eq:translation-matrix)

### Translation in OpenGL

Now we know the mathematical theory behind applying a transformation lets apply it to OpenGL. First download and build the project files for this lab.

1. Go to <a href="https://github.com/jonshiach/Lab05_Transformations" target="_blank">https://github.com/jonshiach/Lab05_Transformations</a> and follow the instructions to download and build the project files.
2. Open the project file `Lab05_Transformations.sln` (or `Lab05_Transformations.xcodeproj` on macOS) set the **Lab05_Transformations** project as the startup project.
    - Visual Studio: right-click on the **Lab05_Transformations** project and select **Set as Startup Project**.
    - Xcode: Click on the target select dropdown (to the right of the name of the project at the top of the window) and select **Lab05_Transformations** as the target.

3. Build the project by pressing CTRL + B (or ⌘B on Xcode) which should build the project without errors. Run the executable by pressing F5 (or ⌘R on Xcode).

If all has gone to plan you should be presented with our smiley texture from [Lab 3](textures-section) applied to a rectangle.

```{figure} ../images/05_texture.png
:width: 500
```

Lets translate the rectangle 0.5 to the right and 0.3 upwards (remember we are dealing with normlised device co-ordinates so the window co-ordinates are between -1 and 1). The transformation matrix to perform this translation is

$$ T = \begin{pmatrix}
    1 & 0 & 0 & 0 \\
    0 & 1 & 0 & 0 \\
    0 & 0 & 1 & 0 \\
    0.5 & 0.3 & 0 & 1
\end{pmatrix}. $$

Add the following code to the `main.cpp` file before the do/while loop.

```cpp
// Define the translation matrix
glm::mat4 translate = glm::mat4(1.0f);
translate[3][0] = 0.5f, translate[3][1] = 0.3f;

std::cout << "translate = " << glm::transpose(translate) << "\n" << std::endl;
```

The output command is there to check we have defined the translation matrix correctly. You should see the following in the output terminal (the window hasn't changed, we'll do that in a minute).

```text
translate = 
[[    1.000,    0.000,    0.000,    0.000]
 [    0.000,    1.000,    0.000,    0.000]
 [    0.000,    0.000,    1.000,    0.000]
 [    0.500,    0.300,    0.000,    1.000]]
 ```

We need a way of passing the `translate` matrix to the shader. Since we are going to be using multiple transformations we will define a `transformation` matrix (for now we set this equal to `translate`) and send it to the vertex shader using a [uniform](uniforms-section). Add the following code to your `main.cpp` file to define `transformation` and get the handle of the uniform.

```cpp
// Calculate the transformation matrix
glm::mat4 transformation = translate;
 
// Get the handle for the transformation matrix
GLuint transformationID = glGetUniformLocation(shaderID, "transformation");
```

We send the uniform to the vertex shader **in the rendering loop** just before we draw the triangles with the `glDrawArrays()` function. Since we have a 4 $\times$ 4 matrix we need to use the `glUniformMatrix4fv()` function to do this.

```cpp
// Send our transformation matrix to the vertex shader
glUniformMatrix4fv(transformationID, 1, GL_FALSE, &transformation[0][0]);
```

The four inputs of `glUniformMatrix4fv()` are:
 - the handle of the uniform we are sending;
 - the number of matrices we have;
 - an OpenGL Boolean value that determines whether we want to transpose the matrix;
 - a pointer to the matrix.

All we now have to do is modify the vertex shader to use the transformation matrix.

```cpp
#version 330 core

// Input vertex data
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

// Output data
out vec2 uv;

// Values that stay constant for the whole mesh
uniform mat4 transformation;

void main()
{
    // Output vertex postion
    gl_Position = transformation * vec4(position, 1.0);
    
    // Output (u,v) co-ordinates
    uv = vec2(textureCoords);
}
```

The only thing we've changed here is specify that we are passing the `transformation` matrix via a uniform and we have multiplied the 4-element vector containing the homogeneous co-ordinates of the vertex by the `transformation` matrix (remember that we reverse the order of the matrix multiplication in OpenGL). Compile and run the program and you should see that our rectangle has been translated to the right and up a bit.

```{figure} ../images/05_translation.png
:width: 500
```

If you are having difficulty getting this to work take a look at [main.cpp](../code/Lab05_Transformations/main.cpp) and [vertexShader.vert](../code/Lab05_Transformations/vertexShader.vert).

Whilst it wasn't particularly difficult to define the `translate` matrix it is something it is a common operation so the glm function `glm::translate(matrix, vector)` outputs the translation matrix for the translation by `vector` applied to `matrix`. So we can replace the line of code where we calculated `translate` with the following

```cpp
translate = glm::translate(glm::mat4(1.0f),                 // matrix that translation is being applied to
                           glm::vec3(0.5f, 0.3f, 0.0f));    // translation vector (tx, ty, tz)
```

The first input to the `glm::translate()` function is the matrix we want to apply the translation to (here we have used the identity matrix) and the second input is the 3-element translation vector.

## Scaling

Scaling is the simplest transformation we can apply. Multiplying the $x$, $y$ and $z$ co-ordinates of a point by a scalar quantity (a number) has the effect of moving the point closer or further away from the origin (0,0). For example, consider the triangle in {numref}`scaling-about-origin-figure`. The $x$ co-ordinate of each vertex has been multiplied by $s_x$ and the $y$ co-ordinates have been multiplied by $s_y$ which has the effect of scaling the triangle and moving the vertices further away from the origin (in this case $s_x$ and $s_y$ are both greater than 1).

```{figure} ../images/05_scaling.svg
:height: 350
:name: scaling-about-origin-figure

Scaling a triangle centred at the origin.
```

Since scaling is simply multiplying the co-ordinates by a number we have

$$ \begin{align*}
    (x, y, z, 1) 
    \begin{pmatrix} 
        s_x & 0 & 0 & 0 \\
        0 & s_y & 0 & 0 \\
        0 & 0 & s_z & 0 \\
        0 & 0 & 0 & 1 
    \end{pmatrix}
    = (s_xx, s_yy, s_zz, 1),
\end{align*} $$

so the **scaling matrix** for applying the scaling transformation is

$$ S = 
\begin{pmatrix} 
    s_x & 0 & 0 & 0 \\
    0 & s_y & 0 & 0 \\
    0 & 0 & s_z & 0 \\
    0 & 0 & 0 & 1
\end{pmatrix}. $$(eq:scaling-matrix)

### Scaling in OpenGL

Lets now apply scaling to our rectangle in OpenGL to increase its size by a factor of 2 and 1.5 in the $x$ and $y$ directions respectively. The process is very similar to how we did the translation and since we have already created a uniform, passed it to the vertex shader and modified the vertex shader, all we need to do to apply shading is calculate the shading matrix. The scaling matrix for doubling the size of the rectangle is

$$ S = \begin{pmatrix}
    2 & 0 & 0 & 0 \\
    0 & 1.5 & 0 & 0 \\
    0 & 0 & 1 & 0 \\
    0 & 0 & 0 & 1
\end{pmatrix}.$$

Define the scaling matrix using the code below.

```cpp
// Scaling matrix
glm::mat4 scale = glm::mat4(1.0f);
scale[0][0] = 2.0f, scale[1][1] = 1.5f;

std::cout << "\nscale = " << glm::transpose(scale) << "\n" << std::endl;
```

Also, set the `transformation` matrix equal to the `scale` matrix.

```cpp
glm::mat4 transformation = scale;
```

Compiling and running the program should output the `scale` matrix so we can check this is correct and present use with our textured rectangle now twice the size it was originally.

```text
scale = 
[[    2.000,    0.000,    0.000,    0.000]
 [    0.000,    1.500,    0.000,    0.000]
 [    0.000,    0.000,    1.000,    0.000]
 [    0.000,    0.000,    0.000,    1.000]]
```

```{figure} ../images/05_scaling.png
:width: 500
```

As with the translation matrix, glm has the function `glm::scale()` which returns a scaling matrix. Replace the line of code where we calculated the `scale` matrix is the following.

```cpp
scale = glm::scale(glm::mat4(1.0f),               // matrix that scaling is applied to
                   glm::vec3(2.0f, 1.5f, 1.0f));  // scaling factors (sx, sy, sz)
```

If you compile and run the program you will notice nothing has changed.

(rotation-section)=

## Rotation

As well as translating and scaling objects, the next most common transformation is the rotation of objects around the three co-ordinate axes $x$, $y$ and $z$. We define the rotation **anti-clockwise** around each of the co-ordinate axes by an angle $\theta$ when looking down the axes ({numref}`3D-rotation-figure`).

```{figure} ../images/05_3D_rotation.svg
:height: 350
:name: 3D-rotation-figure

Rotation is assumed to be in the anti-clockwise direction when looking down the axis.
```

The **rotation matrices** for achieving these rotations are

$$ \begin{align*}
    R_x &= 
    \begin{pmatrix} 
        1 & 0 & 0 & 0 \\ 
        0 & \cos(\theta) & \sin(\theta) & 0 \\
        0 & -\sin(\theta) & \cos(\theta) & 0 \\
        0 & 0 & 0 & 1
    \end{pmatrix}, \\
    R_y &= 
    \begin{pmatrix} 
        \cos(\theta) & 0 & -\sin(\theta) & 0 \\
        0 & 1 & 0 & 0 \\
        \sin(\theta) & 0 & \cos(\theta) & 0 \\
        0 & 0 & 0 & 1
    \end{pmatrix}, \\
    R_z &= 
    \begin{pmatrix} 
        \cos(\theta) & \sin(\theta) & 0 & 0 \\
        -\sin(\theta) & \cos(\theta) & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        0 & 0 & 0 & 1
    \end{pmatrix}.
\end{align*} $$

You don't really need to know how these are derived but if you are curious you can click on the dropdown link below.

````{dropdown} Derivation of the rotation matrices (click to show)

We will consider rotation about the $z$-axis and will restrict our co-ordinates to 2D.

```{figure} ../images/05_rotation.svg
:height: 300
:name: rotation-figure

Rotating the vector $\vec{u}$ anti-clockwise by angle $\theta$ to the vector $\vec{v}$.
```

Consider {numref}`rotation-figure` where the vector $\vec{u}$ is rotated by angle $\theta$ to the vector $\vec{v}$. To get this rotation we first consider the rotation of the vector $\vec{t}$, which has the same length as $\vec{u}$ and points along the $x$-axis, by angle $\phi$ to get to $\vec{u}$. If we form a right-angled triangle (the blue one) then we know the length of the hypotenuse, $|\vec{u}|$, and the angle so we can calculate the lengths of the adjacent and opposite sides using trigonometry. Remember our trig ratios (SOH-CAH-TOA)

$$ \begin{align*}
    \sin(\phi) &= \frac{\textsf{opposite}}{\textsf{hypotenuse}}, &
    \cos(\phi) &= \frac{\textsf{adjacent}}{\textsf{hypotenuse}}, &
    \tan(\phi) &= \frac{\textsf{opposite}}{\textsf{adjacent}},
\end{align*} $$

so the length of the adjacent and opposite sides of the blue triangle is

$$ \begin{align*}
    \textsf{adjacent} &= \textsf{hypotenuse} \cdot \cos(\phi), \\
    \textsf{opposite} &= \textsf{hypotenuse} \cdot \sin(\phi).
\end{align*} $$

Now $u_x$ and $u_y$ are the lengths of the adjacent and opposite sides respectively and $|\vec{u}|$ is the length of the hypotenuse so we have

$$ \begin{align*}
    u_x &= |\vec{u}| \cos(\phi), \\
    u_y &= |\vec{u}| \sin(\phi).
\end{align*} $$

Now consider the rotation from $\vec{t}$ by the angle $\phi + \theta$ to get to $\vec{v}$. Using the same method as before we have

$$ \begin{align*}
    v_x &= |\vec{u}| \cos(\phi + \theta), \\
    v_y &= |\vec{u}| \sin(\phi + \theta).
\end{align*} $$

We can rewrite $\cos(\phi+\theta)$ and $\sin(\phi+\theta)$ using <a href="https://en.wikipedia.org/wiki/List_of_trigonometric_identities#" target="_blank">trigonometric identities</a>

$$ \begin{align*}
    \cos(\phi + \theta) &= \cos(\phi) \cos(\theta) - \sin(\phi) \sin(\theta), \\
    \sin(\phi + \theta) &= \sin(\phi) \cos(\theta) + \cos(\phi) \sin(\theta),
\end{align*} $$

so 

$$ \begin{align*}
    v_x &= |\vec{u}| \cos(\phi) \cos(\theta) - |\vec{u}| \sin(\phi) \sin(\theta), \\
    v_y &= |\vec{u}| \sin(\phi) \cos(\theta) + |\vec{u}| \cos(\phi) \sin(\theta).
\end{align*} $$

Since $ u_x = |\vec{u}| \cos(\phi)$ and $u_y = |\vec{u}| \sin(\phi)$ then

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
You may find some sources present the rotation matrices in a slightly different way where the negative sign for $\sin(\theta)$ is swapped (see <a href="https://en.wikipedia.org/wiki/Rotation_matrix" target="_blank">Wikipedia</a> for an example). This is because these assume that the vertices are multiplied by the transformation matrix on the left, e.g., $(x', y', z', 1)^\mathsf{T} = T\cdot (x, y, z, 1)^\mathsf{T}$. With OpenGL we multiply by the transformation matrix on the right, e.g., $(x', y', y', 1) = (x, y, z, 1) \cdot T$ so our rotation matrices are the transpose of those given elsewhere (although of course in our code we use `T * [x, y, z, 1]` since OpenGL uses column-major order to store the matrices - this can get quite confusing!). 
```

### Rotation in OpenGL

Lets rotate our original rectangle anti-clockwise about the $z$-axis by $\theta = 45^\circ$. The rotation matrix to do this is

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
\end{pmatrix}. $$

Define the rotation matrix using the code below.

```cpp
// Define rotation matrix
glm::mat4 rotate = glm::mat4(1.0f);
float angle = glm::radians(45.0f);
rotate[0][0] = cos(angle),  rotate[0][1] = sin(angle);
rotate[1][0] = -sin(angle), rotate[1][1] = cos(angle);

std::cout << "\nrotate = " << glm::transpose(rotate) << "\n" << std::endl;
```

Note that here we needed to convert 45$^\circ$ into <a href="https://en.wikipedia.org/wiki/Radian" target="_blank">**radians**</a> since OpenGL expects angles to be in radians. We now set the `transformation` matrix equal to our `rotate` matrix

```cpp
glm::mat4 transformation = rotate;
```

Compiling and running the program should output the `rotate` matrix to the terminal so we can check this is correct and we can see in our application window that the textured rectangle has been rotated 45$^\circ$ in the anti-clockwise direction.

```text
rotate = 
[[    0.707,    0.707,    0.000,    0.000]
 [   -0.707,    0.707,    0.000,    0.000]
 [    0.000,    0.000,    1.000,    0.000]
 [    0.000,    0.000,    0.000,    1.000]]
```

```{figure} ../images/05_rotation.png
:width: 500
```

(axis-angle-rotation-section)= 

### Axis-angle rotation

The three rotation transformations are only useful if we want to only rotate around one of the three co-ordinate axes. A more useful transformation is the rotation around the axis that points in the direction of a vector, $\vec{v}$ say, which has its tail at (0,0,0) ({numref}`axis-angle-rotation-figure`).

```{figure} ../images/05_axis_angle_rotation_1.svg
:height: 250
:name: axis-angle-rotation-figure

Axis-angle rotation.
```

The transformation matrix for rotation around a **unit vector** $\hat{\vec{v}} = (v_x, v_y, v_z)$, anti-clockwise by angle $\theta$ when looking down the vector is.

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

The rotation about the vector $\vec{v} = (v_x, v_y, v_z)$ by angle $\theta$ is the [composition](composite-transformations-section) of 5 separate rotations:

1. Rotate $\vec{v}$ around the $x$-axis so that it is in the $xz$-plane (the $y$ component of the vector is 0);
1. Rotate the vector around the $y$-axis so that it points along the $z$-axis (the $x$ and $y$ components are 0 and the $z$ component is a positive number);
1. Perform the rotation around the $z$-axis;
1. Reverse the rotation around the $y$-axis;
1. Reverse the rotation around the $x$-axis.

The rotation around the $x$-axis is achieved by forming a right-angled triangle in the $yz$-plane where the the angle of rotation $\theta$ has an adjacent side of length $v_z$, an opposite side of length $v_y$ and a hypotenuse of length $\sqrt{v_y^2 + v_z^2}$ ({numref}`axis-angle-rotation1-figure`). 

```{figure} ../images/05_axis_angle_rotation_2.svg
:height: 220
:name: axis-angle-rotation1-figure

Rotate $\vec{v}$ around the $x$-axis
```

Therefore $\cos(\theta) = \dfrac{v_z}{\sqrt{v_y^2 + v_z^2}}$ and $\sin(\theta) = \dfrac{v_y}{\sqrt{v_y^2 + v_z^2}}$ so the rotation matrix is

$$ R_1 = \begin{pmatrix} 
    1 & 0 & 0 & 0 \\ 
    0 & \dfrac{v_z}{\sqrt{v_y^2 + v_z^2}} & \dfrac{v_y}{\sqrt{v_y^2 + v_z^2}} & 0 \\
    0 & -\dfrac{v_x}{\sqrt{v_y^2 + v_z^2}} & \dfrac{v_z}{\sqrt{v_y^2 + v_z^2}} & 0 \\
    0 & 0 & 0 & 1
\end{pmatrix}.$$

The rotation around the $y$-axis is achieved by forming another right-angled triangle in the $xz$-plane where $\theta$ has an adjacent side of length $\sqrt{v_y^2 + v_z^2}$, an opposite side of length $v_x$ and a hypotenuse of length $|\vec{v}|$ ({numref}`axis-angle-rotation2-figure`).

```{figure} ../images/05_axis_angle_rotation_3.svg
:height: 250
:name: axis-angle-rotation2-figure

Rotate around the $y$-axis
```

Therefore $\cos(\theta) = \dfrac{\sqrt{v_y^2 + v_z^2}}{|\vec{v}|}$ and $\sin(\theta) = \dfrac{v_x}{|\vec{v}|}$. Note that here we are rotating in the clockwise direction so the rotation matrix is

$$ R_2 = \begin{pmatrix} 
    \dfrac{\sqrt{v_y^2 + v_z^2}}{|\vec{v}|} & 0 & \dfrac{v_x}{|\vec{v}|} & 0 \\
    0 & 1 & 0 & 0 \\
    -\dfrac{v_x}{|\vec{v}|} & 0 & \dfrac{\sqrt{v_y^2 + v_z^2}}{|\vec{v}|} & 0 \\
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
       \dfrac{\sqrt{v_y^2 + v_z^2}}{|\vec{v}|} & 0 & -\dfrac{v_x}{|\vec{v}|} & 0 \\
       0 & 1 & 0 & 0 \\
       \dfrac{v_x}{|\vec{v}|} & 0 & \dfrac{\sqrt{v_y^2 + v_z^2}}{|\vec{v}|} & 0 \\
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
        \dfrac{v_x^2 + (v_y^2 + v_z^2) \cos(\theta)}{|\vec{v}|^2} &
        \dfrac{v_xv_y(1 - \cos(\theta))}{|\vec{v}|^2} + \dfrac{v_z\sin(\theta)}{|\vec{v}|} &
        \dfrac{v_xv_z(1 - \cos(\theta))}{|\vec{v}|^2} - \dfrac{v_y\sin(\theta)}{|\vec{v}|} & 
        0 \\
        \dfrac{v_xv_y(1 - \cos(\theta))}{|\vec{v}|^2} - \dfrac{v_z\sin(\theta)}{|\vec{v}|} &
        \dfrac{v_y^2 + (v_x^2 + v_y^2)\cos(\theta)}{|\vec{v}|^2} &
        \dfrac{v_yv_z(1 - \cos(\theta))}{|\vec{v}|^2} - \dfrac{v_x\sin(\theta)}{|\vec{v}|} &
        0 \\
        \dfrac{v_xv_z(1 - \cos(\theta))}{|\vec{v}|^2} + \dfrac{v_y\sin(\theta)}{|\vec{v}|} &
        \dfrac{v_yv_z(1 - \cos(\theta))}{|\vec{v}|^2} - \dfrac{v_x\sin(\theta)}{|\vec{v}|} &
        \dfrac{v_z^2 + (v_x^2 + v_y^2)\cos(\theta)}{|\vec{v}|^2} & 
        0 \\
        0 & 0 & 0 & 1
    \end{pmatrix}
\end{align*} $$

If we make $\vec{v}$ a unit vector so that $|\vec{v}| = 1$ and $v_y^2 + v_z^2 = 1 - v_x^2$ then this simplifies to 

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

Fortunately we do not need to code this matrix into C++ as glm has a function to calculate the axis-angle rotation. The function is `glm::rotate(matrix, angle, vector)` where `angle` is in radians and `vector` is the direction vector which were are rotating around. Replace the two lines of code where we have defined the `rotate` matrix with the following

```cpp
rotate = glm::rotate(glm::mat4(1.0f),               // matrix that rotation is applied to
                     glm::radians(45.0f),           // rotation angle
                     glm::vec3(0.0f, 0.0f, 1.0f));  // vector to rotate around
```

Here we have used the vector (0, 0, 1) as we wanted to rotate around the $z$-axis. You should see no change in the output.

(composite-transformations-section)=
## Composite transformations

So far we have performed translation, scaling and rotation transformations on our rectangle separately. What if we wanted to combine these transformations so that we can control the size, rotation and position of the rectangle? If $T$, $S$ and $R$ are the transformation matrices for translation, scaling and rotation, then if we want to scale first, then rotate and then translate the scaled object we have

$$ \begin{align*}
    (x', y', z', 1) &= (((x, y, z, 1) \cdot S) \cdot R) \cdot T \\
    &= (x, y, z, 1) \cdot (S \cdot R \cdot T)
\end{align*} $$

$S \cdot R \cdot T$ is a single $4 \times 4$ transformation matrix that combines the three transformations known as the **composite transformation matrix**. 

### Composite transformations in OpenGL

Lets apply scaling, rotation and translation (in that order) to our rectangle. Since we have already calculated the separate transformation matrices all we need to do is to multiply them together and set it equal to `transformation`.

```cpp
glm::mat4 transformation = translate * rotate * scale;
```

(column-major-important-note)=

````{important}
Remember that OpenGL and glm use column-major ordering so we reverse the order of our matrices when multiplying. So $S \cdot R \cdot T$ would be coded as

```cpp
T * R * S
```

i.e., the transformations are applied from right to left.
````

After compiling and running the program you should see the following.

```{figure} ../images/05_composite_transformation.png
:width: 500
```

(animating-objects-section)= 

## Animating objects

It may appear that our application is displaying a static image of the textured rectangle but what is actually happing is that the window is constantly being updated with new frame buffers as and when they have been calculated. We can animate our rectangle by applying the transformations within the render loop. We are going to rotate and translate the rectangle so that it appears to be rotating about its centre.

If we calculate the transformation matrices inside the render loop we can move the rectangle around the window. A useful function to help us is `glfwGetTime()` from the GLFW library which returns the time in seconds since the GLFW window was created. If we have a time value that is always increasing we can use this to animate our rectangle.

Comment out all of the code used to calculate the transformation matrices we have entered so far this lab but leave the line where we get the location of the transformation uniform.

Inside the **render loop** just before we send the `transformation` matrix to the shader add the following code.

 ```cpp
// Calculate transformations
float time = glfwGetTime();
glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.3f, 0.0f));
glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.0f, 0.0f, 1.0f));
glm::mat4 transformation = translate * rotate * scale;
```

Here we have defined the `translate`, `scale` and `rotate` matrices and used them to calculate the composite transformation matrix. Note that we have used the `time` variable which will cause the rectangle to rotate. Compile and run the program and you should see something similar to the following.

<center>
<video controls muted="true" loop="true" width="500">
    <source src="../_static/05_rotating_smiley_1.mp4" type="video/mp4">
</video>
</center>

When calculating the composite transformation matrix the order in which we multiply the individual transformations will determine the effects of the composite transformation. To see the effect of this lets translate the rectangle first before rotating it by changing the `transformation` calculation to the following.

```cpp
glm::mat4 transformation = rotate * translate * scale;
```

Compile and run the program and we have something quite different.

<center>
<video controls muted="true" loop="true" width="500">
    <source src="../_static/05_rotating_smiley_2.mp4" type="video/mp4">
</video>
</center>

---

## Exercises

1. Use translation to produce an animation of the original rectangle rotating around a circle of radius 0.5 centered at the centre of the window. Hint: the co-ordinates of points on a circle centered at (0,0) can be calculated using $x = {\tt radius} \cdot \cos({\tt time})$ and $y = {\tt radius} \cdot \sin({\tt time})$ where $t$ is some number.

<center>
<video controls muted="true" loop="true" width="500">
    <source src="../_static/05_Ex1.mp4" type="video/mp4">
</video>
</center>

2. Use rotation to rotate the rectangle from exercise 1 clockwise about its own centre with a rotation speed double that of the rotation speed used to rotate it around the circle.

<center>
<video controls muted="true" loop="true" width="500">
    <source src="../_static/05_Ex2.mp4" type="video/mp4">
</video>
</center>

1. Use scaling to scale the rectangle so that it grows and shrinks as it is moving. Hint: the function $s = 1 + 0.5 \sin(a \cdot {\tt time})$ oscillates between 0.5 and 1.5 as time increases. Experiment with the speed of which the rectangle grows and shrinks by changing the value of $a$.

<center>
<video controls muted="true" loop="true" width="500">
    <source src="../_static/05_Ex3.mp4" type="video/mp4">
</video>
</center>

4. Create your own class called `MyLib` using header and code files `MyLib.hpp` and `MyLib.cpp` that includes static member functions to calculate the translation, scaling and angle-axis rotation matrices (you may use `glm::mat4()`, `glm::vec3()` and the `cmath` library). Replace the glm functions `glm::translate()`, `glm::scale()` and `glm::rotate()` with functions from MyLib to answer exercises 1 to 3.

---

## Source code

The source code for this lab, including the exercise solutions, can be downloaded using the links below.

- [main.cpp](../code/Lab05_Transformations/Lab05_solutions.cpp)
- [MyLib.hpp](../code/Lab05_Transformations/MyLib.hpp)
- [MyLib.cpp](../code/Lab05_Transformations/MyLib.cpp)
- [vertexShader.vert](../code/Lab05_Transformations/vertexShader.vert)