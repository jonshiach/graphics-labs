(3D-worlds-section)=

# 3D Worlds

In [Lab 5](transformations-section) we looked at the transformations can can be applied to the vertex co-ordinates $(x, y, z, 1)$ but all of our examples were using 2D objects. In this lab we will take the step into the third spatial dimension and look at 3D worlds.

First download and build the project files for this lab.

1. Go to <a href="https://github.com/jonshiach/Lab06_3D_worlds" target="_blank">https://github.com/jonshiach/Lab06_3D_worlds</a> and follow the instructions to download and build the project files.
2. Open the project file `Lab06_3D_worlds.sln` (or `Lab06_3D_worlds.xcodeproj` on macOS) set the **Lab06_3D_worlds** project as the startup project.
    - Visual Studio: right-click on the **Lab06_3D_worlds** project and select **Set as Startup Project**.
    - Xcode: Click on the target select dropdown (to the right of the name of the project at the top of the window) and select **Lab06_3D_worlds** as the target.

3. Build the project by pressing CTRL + B (or ⌘B on Xcode) which should build the project without errors. Run the executable by pressing F5 (or ⌘R on Xcode).

## 3D models

To demonstrate building a simple 3D world we are going to need a 3D object. One of the simplest 3D objects is a **unit cube** which is a cube centred at (0,0,0) and has side lengths of 2 parallel to the co-ordinate axes ({numref}`unit-cube-figure`) so the co-ordinates of the 8 corners of the cube are combinations of -1 and 1.

```{figure} ../images/06_Unit_cube.svg
:width: 700
:name: unit-cube-figure

A unit cube.
```

Open up the project and take a look at the `main.cpp` file and you will see that the arrays `vertices` and `uvCoords` arrays have been defined for our unit cube.

```cpp
// Define vertex co-ordinates
static const GLfloat vertices[] = {
   // front
   -1.0f, -1.0f,  1.0f,    //              + ------ +
    1.0f, -1.0f,  1.0f,    //             /|       /|
    1.0f,  1.0f,  1.0f,    //   y        / |      / |
   -1.0f, -1.0f,  1.0f,    //   |       + ------ +  |
    1.0f,  1.0f,  1.0f,    //   + - x   |  + ----|- +
   -1.0f,  1.0f,  1.0f,    //  /        | /      | /
   // right                // z         |/       |/
    1.0f, -1.0f,  1.0f,    //           + ------ +
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    // etc.
};

// Define texture vertices
static const GLfloat uvCoords[] = {
   0.0f, 0.0f,     // vertex co-ordinates are the same for each side
   1.0f, 0.0f,     // of the cube so repeat every six vertices
   1.0f, 1.0f,
   0.0f, 0.0f,
   1.0f, 1.0f,
   0.0f, 1.0f,
   // etc.
};
```

If you compile and run this program you will see that the `crate.bmp` texture fills the window.

## Co-ordinate systems

OpenGL uses a co-ordinate system with the $x$ axis pointing horizontally to the right, the $y$ axis pointing vertically upwards and the $z$ axis pointing horizontally towards the viewer. To simplify things when it comes to displaying the 3D world, the axes are limited to a range from -1 to 1 so any object outside of this range will not be shown on the display. These are known as **Normalised Device Co-ordinates (NDC)**.

```{figure} ../images/06_NDC.svg
:width: 500
:name: NDC-figure

Normalise Device Co-ordinates (NDC)
```

The steps used in the creation of a 3D world and eventually displaying it on screen requires that we transform through several intermediate co-ordinate systems:

- **Model space** - each individual 3D object that will appear in the 3D world is defined in its own space usually with the centre of the object at (0,0,0) to make the transformations easier.
- **World space** - the 3D world is constructed by transforming the individual 3D objects using translation, rotation and scaling transformations. The co-ordinates of the objects is arbitrary and left to the choice of the designer of the 3D world.
- **View space** - the world space is transformed so that the position of the viewer, in other words the camera, is at (0,0,0) and the direction the camera is pointing is down the $z$ axis, i.e., parallel to (0,0,-1).
- **Screen space** - the view space is transformed so that the co-ordinates are in NDC. The volume of the view space that is contained in the screen space is chosen by the user.

```{figure} ../images/06_mvp.svg
:width: 500

Transformations between different spaces.
```

## Model, view and projection matrices

We saw in [Lab 5](transformations-section) that we apply a transformation by multiplying the object co-ordinates by a transformation matrix. Since we are transforming between difference co-ordinate spaces we have 3 main transformation matrices

- the **model** matrix - combined scaling, rotation and translation applied to each individual object
- the **view** matrix - combined translation and alignment transformations
- the **projection** matrix - combined projection of the view space and scaling to NDC

(model-matrix-section)=
### The model matrix

In [Lab 5](transformations-section) we saw that we can combine transformations such as translation, scaling and rotation by multiplying the individual transformation matrices together. The **model matrix** is the matrix that is used to apply transformations to an object.

Lets compute a model matrix for our cube where it is scaled down by a factor of 0.5 in each co-ordinate direction, rotated about the $y$ axis using the time of the current frame as the rotation angle and translated backwards down the $z$-axis so that its centre is at (0, 0, -4). Add the following code **inside the rendering loop** before we call the `glDrawArrays()` function.

```cpp
// Calculate the model matrix
float time = glfwGetTime();
glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));
glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.0f, 1.0f, 0.0f));

glm::mat4 model = translate * rotate * scale;
```

Here we have calculated the individual transformation matrices for translation, scaling and rotation and multiply them to give the `model` matrix.

(view-matrix-section)=

### The view matrix

OpenGL assumes that the camera is always at (0,0,0) and looking down the $z$-axis so we need to transform the co-ordinates to this **view space** ({numref}`view-space-figure`).

```{figure} ../images/06_view_space.svg
:width: 350
:name: view-space-figure

The view space.
```

To calculate the world space to view space transformation we require three vectors

- $\tt position$ - the co-ordinates of the camera position
- $\tt target$ - the co-ordinates of the target point that the camera is looking towards
- $\tt worldUp$ - a vector pointing straight up in the world space which allows us to orientate the camera, this is usually always (0,1,0)

```{figure} ../images/06_view_space_alignment.svg
:width: 400
:name: view-space-alignment-figure

The vectors used in the transformation to the view space.
```

The $\tt position$ and $\tt target$ vectors are either determined by the user through keyboard, mouse or controller inputs or through some predetermined routine. To determine the view space transformation we first translate the camera position to (0,0,0) using the following translation matrix

$$ \begin{align*}
    \textsf{translation matrix} =
    \begin{pmatrix}
        1 & 0 & 0 & 0 \\
        0 & 1 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        -\tt position.x & -\tt position.y & -\tt position.z & 1
    \end{pmatrix}
\end{align*}. $$

The next step is to align the world space so that the direction vector is pointing down the $z$ axis. To do this we use vectors $\tt right$, $\tt up$ and $\tt front$ which are unit vectors at right-angles to each other the point in directions relative to the camera ({numref}`view-space-alignment-figure`).

The $\tt front$ vector points directly forward of the camera and is calculated using

$$ \texttt{front} = \textsf{normalise}(\tt target - position).$$

The $\tt right$ vector points to the right of the camera so is at right-angles to both the $\tt front$ and $\tt worldUp$ vectors. We can use the [cross product](cross-product-section) between the two vectors to calculate this (note that the order of the vectors is important).

$$ \tt right = \textsf{normalise}(front \times worldUp) .$$

The $\tt up$ vector points in the up direction of the camera and is at right-angles to the $\tt front$ and $\tt right$ vectors we have already calculated. So this can be calculated using another cross product.

$$ \tt up = \textsf{normalise}(right \times front).$$

Once these vectors have been calculated the transformation matrix to align the $\tt front$ vector so that it points down the $z$-axis is

$$\textsf{alignment matrix} = \begin{pmatrix}
    \tt right.x & \tt up.x & \tt -front.x & 0 \\
    \tt right.y & \tt up.y & \tt -front.y & 0 \\
    \tt right.z & \tt up.z & \tt -front.z & 0 \\
    0 & 0 & 0 & 1
\end{pmatrix}.$$

The translation matrix and alignment matrix are multiplied together to form the **view matrix** which transforms the world space co-ordinates to the view space.

$$ \begin{align*}
    \textsf{view matrix} &= \textsf{translation matrix} \cdot \textsf{alignment matrix}.
\end{align*} $$

Lets move the camera to look at our cube from the position (1,1,0) looking towards the center of the cube. Add the following to the `main.cpp` file after we have calculated the `model` matrix.

```cpp
// Calculate view matrix
glm::vec3 position = glm::vec3(1.0f, 1.0f, 0.0f);
glm::vec3 target = glm::vec3(0.0f, 0.0f, -4.0f);
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 front = glm::normalize(target - position);
glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
glm::vec3 up = glm::normalize(glm::cross(right, front));

glm::mat4 align = glm::mat4(1.0f);

for (unsigned int i = 0; i < 3; i++)
{
    translate[3][i] = -position[i];
    align[i][0] = right[i];
    align[i][1] = up[i];
    align[i][2] = -front[i];
}

glm::mat4 view = align * translate;
```

### Projection

The next step is to project the view space onto the screen space. OpenGL uses NDC where the screen space is a unit cube where axes co-ordinates range from -1 to 1. Any fragments with co-ordinates outside of this range are **clipped** and ignored by the shaders. The simplest type of projection is **orthographic projection** where the co-ordinates in the view space are transformed to the screen space by simple translation and scaling transformations.

The region of the view space that will form the screen space is defined by a cuboid bounded by a left, right, bottom, top, near and far clipping planes. Any objects outside of the cuboid are clipped and discarded from the rendering ({numref}`orthographic-projection-figure`).

```{figure} ../images/06_orthographic_projection.svg
:width: 600
:name: orthographic-projection-figure

Orthographic projection.
```

The **orthographic projection matrix** is calculated using the left, right, bottom, top, near and far co-ordinates such that

$$ \begin{align*}
    \textsf{orthographic projection matrix} = 
    \begin{pmatrix}
        \frac{2}{\textsf{right} - \textsf{left}} & 0 & 0 & 0 \\
        0 & \frac{2}{\textsf{top} - \textsf{bottom}} & 0 & 0 \\
        0 & 0 & \frac{2}{\textsf{near} - \textsf{far}} & 0 \\
        -\frac{\textsf{right} + \textsf{left}}{\textsf{right} - \textsf{left}} & -\frac{\textsf{top} + \textsf{bottom}}{\textsf{top} - \textsf{bottom}} & \frac{\textsf{near} + \textsf{far}}{\textsf{near} - \textsf{far}} & 1
    \end{pmatrix}
\end{align*}. $$

You don't really need to know how this matrix is derived but if you are interested click on the dropdown link below.

```{dropdown} Derivation of the orthographic projection matrix

To derive the orthographic projection we first need to translate the co-ordinates so that the centre of the cuboid that represents the clipping volume to (0,0,0). The centre co-ordinates are calculated using the average of the edge co-ordinates, e.g., for the $x$ co-ordinate this would be $\frac{\textsf{right} + \textsf{left}}{2}$, so the translation matrix is

$$ \begin{align*}
    \textsf{translation matrix} = 
    \begin{pmatrix}
        1 & 0 & 0 & 0 \\
        0 & 1 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        -\frac{\textsf{right} + \textsf{left}}{2} & -\frac{\textsf{top} + \textsf{bottom}}{2} & \frac{\textsf{near} + \textsf{far}}{2} & 1
    \end{pmatrix}
\end{align*} $$

The second step is to scale the clipping volume so that the co-ordinates are between -1 and 1. This is done by dividing the distance between the edges of the screen space by the distance between the clipping planes, e.g., for the $x$ co-ordinate this would be $\frac{1 - (-1)}{\textsf{right} - \textsf{left}}=\frac{2}{\textsf{right} - \textsf{left}}$, so the scaling matrix is

$$ \begin{align*}
    \textsf{scaling matrix} = 
    \begin{pmatrix}
        \frac{2}{\textsf{right} - \textsf{left}} & 0 & 0 & 0 \\
        0 & \frac{2}{\textsf{top} - \textsf{bottom}} & 0 & 0 \\
        0 & 0 & \frac{2}{\textsf{near} - \textsf{far}} & 0 \\
        0 & 0 & 0 & 1
    \end{pmatrix}.
\end{align*} $$

Combining the translation and scaling matrices gives the orthographic projection matrix

$$ \begin{align*}
    \textsf{orthographic projection matrix} &=  \textsf{translation matrix} \cdot \textsf{scaling matrix}\\
    &=
    \begin{pmatrix}
        \frac{2}{\textsf{right} - \textsf{left}} & 0 & 0 & 0 \\
        0 & \frac{2}{\textsf{top} - \textsf{bottom}} & 0 & 0 \\
        0 & 0 & \frac{2}{\textsf{near} - \textsf{far}} & 0 \\
        -\frac{\textsf{right} + \textsf{left}}{\textsf{right} - \textsf{left}} & -\frac{\textsf{top} + \textsf{bottom}}{\textsf{top} - \textsf{bottom}} & \frac{\textsf{near} + \textsf{far}}{\textsf{near} - \textsf{far}} & 1
    \end{pmatrix}
\end{align*} $$
```

Lets calculate the orthographic projection matrix using left = -2, right = 2, bottom = -2, top = 2, near = 0, far = 10. Add the following code after we have calculated the `view` matrix.

```cpp
// Calculate projection matrix (orthographic projection)
float left, right, near, far, top, bottom;
left = -2.0f, right = 2.0f;
bottom = -2.0f, top = 2.0f;
near = 0.0f, far = 10.0f;

glm::mat4 projection = glm::mat4(1.0f);
projection[0][0] = 2.0f / (right - left);
projection[1][1] = 2.0f / (top - bottom);
projection[2][2] = 2.0f / (near - far);
projection[3][0] = - (right + left) / (right - left);
projection[3][1] = - (top + bottom) / (top - bottom);
projection[3][2] = (near + far) / (near - far);
```

### Applying the model, view and projection matrices

Now that we have defined the `model`, `view` and `projection` matrices we need send it to the shader and apply the transformations. First we need to get the handle of the uniforms for each of the matrices, we do this in the same way as we did for the textures in [Lab 3](textures-section).

```cpp
// Get the handles for the shader uniforms
GLuint texture1ID = glGetUniformLocation(shaderID, "texture1Sampler");
GLuint modelID = glGetUniformLocation(shaderID, "model");
GLuint viewID = glGetUniformLocation(shaderID, "view");
GLuint projectionID = glGetUniformLocation(shaderID, "projection");
```

We can now send the matrices to the vertex shader, add the following code after we have calculated the `model`, `view` and `projection` matrices.

```cpp
// Send the model view and projection matrices to the shader
glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
```

Of course we also need to update the vertex shader so that is uses the `model`, `view` and `projection` matrices. Edit `vertexShader.vert` so that the `gl_Position` vector is calculated as follows.

```cpp
#version 330 core

// Input vertex data
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

// Output data
out vec2 uv;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Output vertex postion
    gl_Position = projection * view * model * vec4(position, 1.0);
    
    // Output (u,v) co-ordinates
    uv = vec2(textureCoords);
}
```

Compile and run the program and you should see the following.

<center>
<video controls muted="true" loop="true" width="500">
    <source src="../_static/06_orthogonal_cube_no_depth_test.mp4" type="video/mp4">
</video>
</center>

If you are having difficulty getting to this stage take a look at the source code [main.cpp](../code/Lab06_3D_worlds/main_no_depth_test.cpp) and vertex shader [vertexShader.vert](../code/Lab06_3D_worlds/vertexShader.vert).

## The z buffer

Our rendering of the cube doesn't look quite right. What is happening here is that some parts of the sides of the cube that are further away from where we are viewing it (e.g., the bottom side) have been rendered after the sides that are closer to us ({numref}`depth-test-1-figure`).

```{figure} ../images/06_depth_test.svg
:width: 300
:name: depth-test-1-figure

Rendering the far triangle after the near triangle.
```

To overcome this issue OpenGL uses a **depth test** when computing the fragment shader. When OpenGL creates a frame buffer it also creates another buffer called a **z buffer** (or **depth buffer**) where the $z$ co-ordinate of each pixel in the frame buffer is stored and initialises all the values to -1 (the furthest possible $z$ co-ordinate in the screen space). When the fragment shader is called it checks whether the fragment has a $z$ co-ordinate more than that already stored in the depth buffer and if so it updates the colour of the fragment and stores its $z$ co-ordinate in the depth-buffer as the current nearest fragment (if the fragment has a $z$ co-ordinate less than what is already in the depth buffer the fragment shader does nothing). This means once the fragment shader has been called for all fragments of all objects, the pixels contain colours of the objects closest to the camera.

To enable depth testing we used the following function before the rendering loop.

```cpp
// Enable depth test
glEnable(GL_DEPTH_TEST);
```

We also need to clear the depth buffer at the start of each frame, change `glClear(GL_COLOR_BUFFER_BIT);` to the following.

```cpp
// Clear the window
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```

Make these changes to your code and you should get a much better result.

<center>
<video controls muted="true" loop="true" width="500">
    <source src="../_static/06_orthogonal_cube.mp4" type="video/mp4">
</video>
</center>

## Perspective projection

The problem with using orthographic projection is that is does not give us any clues to how far an object is from the viewer. We would expect that objects further away from the camera would appear smaller whereas objects closer to the camera would appear larger. 

Perspective project uses the same near and far clipping planes as orthographic projection but the clipping planes on the sides are not parallel, rather they angle in such that the four planes meet at (0,0,0) ({numref}`perspective-projection-figure`). The clipping volume bounded by the size clipping planes is called the **viewing frustum**. 

```{figure} ../images/06_perspective_projection.svg
:width: 600
:name: perspective-projection-figure

Perspective projection.
```

The shape of the viewing frustum is determined by four factors:

- $\textsf{near}$ -- the distance from (0,0,0) to the near clipping plane
- $\textsf{far}$ -- the distance from (0,0,0) to the far clipping plane
- $\textsf{fov}$ -- the **field of view** angle between the bottom and top clipping planes (used to determine how much of the view space is visible)
- $\textsf{aspect}$ -- the width-to-height **aspect ratio** of the window

Given these four factors we can calculate the **perspective projection matrix** using

$$ \begin{align*}
    \textsf{perspective projection matrix} = 
    \begin{pmatrix}
        \frac{\textsf{near}}{\textsf{right}} & 0 & 0 & 0 \\
        0 & \frac{\textsf{near}}{\textsf{top}} & 0 & 0 \\
        0 & 0 & -\frac{\textsf{far} + \textsf{near}}{\textsf{far} - \textsf{near}} & -1 \\
        0 & 0 & - \frac{2 \cdot \textsf{far} \cdot \textsf{near}}{\textsf{far} - \textsf{near}} & 0
    \end{pmatrix},
\end{align*} $$

where $\textsf{top} = \textsf{near} \cdot \tan(\frac{\textsf{fov}}{2}$) and $\textsf{right} = \textsf{aspect} \cdot \textsf{top}$. You don't really need to know how this is derived but it you are interested click on the dropdown below.

````{dropdown} Derivation of the perspective projection matrix

The mapping of a point in the view space with co-ordinates $(x, y, z)$ onto the near clipping plane to the point $(x', y', -\textsf{near})$ is shown in {numref}`perspective-mapping-figure`.

```{figure} ../images/06_perspective_projection_mapping.svg
:width: 500
:name: perspective-mapping-figure

Mapping of the point at $(x,y,z)$ onto the near plane using perspective.
```

The ratio of $x$ to $-z$ distance is the same as the ratio of $x'$ to $\textsf{near}$ distance (and similar for $y'$) so

$$ \begin{align*}
    \frac{x}{-z} &= \frac{x'}{\textsf{near}} &\implies
    x' &= -\frac{x \cdot \textsf{near}}{z}, \\
    \frac{y}{-z} &= \frac{y'}{\textsf{near}} &\implies
    y' &= -\frac{y \cdot \textsf{near}}{z},
\end{align*} $$

So we are mapping $(x, y)$ to $\left( -\frac{x \cdot \textsf{near}}{z}, -\frac{y \cdot \textsf{near}}{z} \right)$. As well as the perspective mapping we also need to ensure that the mapped co-ordinates $(x', y', z')$ are between -1 and 1. Consider the mapping of the $x$ co-ordinate

$$ \begin{align*}
    \textsf{left} &\leq x' \leq \textsf{right} \\
    -\textsf{right} &\leq x' \leq \textsf{right}  && \textsf{(since left $= -$ right)} \\
    -1 &\leq \frac{x'}{\textsf{right}} \leq 1 && \textsf{(divide by right)}
\end{align*} $$

Since $x' = -\frac{x \cdot \textsf{near}}{z}$ then

$$ \begin{align*}
    -1 &\leq -\frac{x \cdot \textsf{near}}{z \cdot \textsf{right}}\leq 1
\end{align*} $$

and doing similar for $y$ we get

$$ \begin{align*}
    -1 &\leq -\frac{y \cdot \textsf{near}}{z \cdot \textsf{top}}\leq 1
\end{align*} $$

If we use [homogeneous co-ordinates](homogeneous-coordinates-section) then this mapping can be represented by the matrix equation

$$ \begin{align*}
    (x, y, z, 1)
    \begin{pmatrix}
        \frac{\textsf{near}}{\textsf{right}} & 0 & 0 & 0 \\
        0 & \frac{\textsf{near}}{\textsf{top}} & 0 & 0 \\
        0 & 0 & A & -1 \\
        0 & 0 & B & 0
    \end{pmatrix}
    =
    \left( \frac{x \cdot \textsf{near}}{\textsf{right}}, \frac{y \cdot \textsf{near}}{\textsf{top}}, Az + B, -z \right)
\end{align*} $$

where $A$ and $B$ are placeholder variables for now. Since we divide homogeneous co-ordinates by the fourth element then

$$ \left( -\frac{x \cdot \textsf{near}}{z \cdot \textsf{right}}, -\frac{y \cdot \textsf{near}}{z \cdot \textsf{top}}, \frac{Az + B}{-z}, 1 \right), $$

so the mapping for $x'$ and $y'$ is correct. We need $z'$ to be between -1 and 1 so $A$ and $B$ must satisfy

$$ \begin{align*}
    \textsf{near plane:} &&\frac{Az + B}{-z} &= -1, & \implies  Az + B &= z, \\
    \textsf{far plane:} && \frac{Az + B}{-z} &= 1, & \implies Az + B &= -z.
\end{align*} $$

At the near clipping plane $z = -\textsf{near}$ and at the far clipping plane $z = -\textsf{far}$ so

$$ \begin{align*}
    -A\cdot \textsf{near} + B &= -\textsf{near}, \\
    -A\cdot \textsf{far} + B &= \textsf{far}.
\end{align*} $$

Subtracting the first equation from the second gives

$$ \begin{align*}
    -A (\textsf{far} - \textsf{near}) &= \textsf{far} + \textsf{near} \\
    \therefore A &= -\frac{\textsf{far} + \textsf{near}}{\textsf{far} - \textsf{near}}.
\end{align*} $$

Substituting $A$ in the second equation gives

$$ \begin{align*}
    \frac{\textsf{far} + \textsf{near}}{\textsf{far} - \textsf{near}} \cdot \textsf{near} + B &= -\textsf{near} \\
    B &= -\textsf{near} \left( 1 +  \frac{\textsf{far} + \textsf{near}}{\textsf{far} - \textsf{near}}\right) \\
    &= -\textsf{near} \left( \frac{\textsf{far} - \textsf{near} + \textsf{far} + \textsf{near}}{\textsf{far} - \textsf{near}}\right) \\
    &= - \frac{2 \cdot \textsf{far} \cdot \textsf{near}}{\textsf{far} - \textsf{near}}.
\end{align*} $$

So the perspective projection matrix is

$$ \begin{align*}
    \textsf{perspective projection matrix} = 
    \begin{pmatrix}
        \frac{\textsf{near}}{\textsf{right}} & 0 & 0 & 0 \\
        0 & \frac{\textsf{near}}{\textsf{top}} & 0 & 0 \\
        0 & 0 & -\frac{\textsf{far} + \textsf{near}}{\textsf{far} - \textsf{near}} & -1 \\
        0 & 0 & - \frac{2 \cdot \textsf{far} \cdot \textsf{near}}{\textsf{far} - \textsf{near}} & 0
    \end{pmatrix},
\end{align*} $$

We now need to calculate the values of $\textsf{right}$ and $\textsf{top}$. The $\textsf{top}$ co-ordinate is the opposite side of a right angled triangle with angle $\frac{\textsf{fov}}{2}$ and adjacent side $\textsf{near}$ so it is easily calculated using trigonometry 

$$ \begin{align*}
    \tan \left( \frac{\textsf{fov}}{2} \right) &= \frac{\textsf{opposite}}{\textsf{adjacent}} = \frac{\textsf{top}}{\textsf{near}} \\
    \textsf{top} &= \textsf{near} \cdot \tan \left( \frac{\textsf{fov}}{2} \right).
\end{align*} $$

Since $\textsf{aspect}$ with the width of the window divided by the height and $\textsf{left} = -\textsf{right}$ and $\textsf{bottom} = -\textsf{top}$ then

$$ \begin{align*}
    \textsf{aspect} &= \frac{\textsf{right} - \textsf{left}}{\textsf{top} - \textsf{bottom}} = \frac{2 \cdot \textsf{right}}{2 \cdot \textsf{top}} \\
    \therefore \textsf{right} &= \textsf{aspect} \cdot \textsf{top}.
\end{align*} $$
````

Lets apply perspective projection to our cube using a near and far clipping planes at 0.2 and 10 respectively and a field of view angle of 45$^\circ$. Add the following code to your `main.cpp` file (you may want to comment out the code used to calculate the orthogonal projection matrix as we aren't using it now).

```cpp
// Calculate perspective projection matrix
float right, top, near, far, fov, aspect;
fov = glm::radians(45.0f);
aspect = 1024.0f / 768.0f;
near = 0.2f;
far = 100.0f;
top = near * glm::tan(fov / 2);
right = aspect * top;

glm::mat4 projection = glm::mat4(1.0f);
projection[0][0] = near / right;
projection[1][1] = near / top;
projection[2][2] = - (far + near) / (far - near);
projection[2][3] = -1.0f;
projection[3][2] = - 2 * far * near / (far - near);;
projection[3][3] = 0.0f;
```

<center>
<video controls muted="true" loop="true" width="500">
    <source src="../_static/06_perspective_cube.mp4" type="video/mp4">
</video>
</center>

### Changing the fov angle

The field of view angle determines how much of the view space we can see in the screen space where the larger the angle the more we can see. When we increase the fov angle it appears to the user that our view is zooming out whereas when we decrease the fov it has the effect of zooming in (this is used a lot in first person shooter games to model the effect of a pair of binoculars or a sniper scope). This effect is shown on our cube object below.

`````{grid}
````{grid-item}
```{figure} ../images/06_fov_15.png

$\textsf{fov} = 15^\circ$
```
````

````{grid-item}
```{figure} ../images/06_fov_90.png

$\textsf{fov} = 90^\circ$
```
````
`````

## glm transformations

Here we have defined the view matrix and projection matrices ourselves but it shouldn't surprise you that there are glm functions that do this for us. These are:

- `glm::lookAt(position, target, worldUp)` - calculates the view matrix
- `glm::ortho(left, right, bottom, top, near, far)` - calculates the orthographic projection matrix
- `glm::perspective(fov, aspect, near, far)` - calculates the perspective projection matrix

Comment out all the code you've used to calculate the `view` and `projection` matrices and add the following.

```cpp
// Calculate the view matrix
glm::vec3 position = glm::vec3(1.0f, 1.0f, 0.0f);
glm::vec3 target = glm::vec3(0.0f, 0.0f, -4.0f);
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 view = glm::lookAt(position, target, worldUp);

// Calculate the projection matrix
float fov = glm::radians(45.0f);
float aspect = 1024.0f / 768.0f;
float near = 0.02f;
float far = 100.0f;
glm::mat4 projection = glm::perspective(fov, aspect, near, far);
```

You should see the same output (which is good as it means our own matrices were correct).

## Multiple objects

Lets add some more cubes to our 3D world. We can do this by defining the position of each cube and then, in the render loop, we loop through each cube and calculate its `model` matrix and render it. Add the following code to your program before the do/while loop.

```cpp
// Cube positions
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -10.0f),
    glm::vec3(-3.0f, -2.0f, -3.0f),
    glm::vec3(-4.0f, -2.0f, -8.0f),
    glm::vec3( 2.0f, -1.0f, -4.0f),
    glm::vec3(-4.0f,  3.0f, -8.0f),
    glm::vec3( 3.0f, -2.0f, -5.0f),
    glm::vec3( 4.0f,  2.0f, -5.0f),
    glm::vec3( 2.0f,  0.0f, -2.0f),
    glm::vec3(-1.0f,  1.0f, -2.0f)
};
```

This creates an array of 3-element vectors that contain the co-ordinates of the centre of 10 cubes. In the **render loop** comment out the code used to calculate the `model` matrix and the `glDrawArrays()` function for the previous examples and add this code.

```cpp
// Loop through cubes and draw each one
for (int i = 0; i < 10; i++)
{
    // Calculate model matrix
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), cubePositions[i]);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f * i), glm::vec3(1.0f, 1.0f, 0.0f));
    glm::mat4 model = translate * rotate * scale;
    
    // Send the model view and projection matrices to the shader
    glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
    
    // Draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (sizeof(float) * 3));
}
```

Here we loop through each of the elements of the `cubePositions` array. The `translate` and `rotate` matrices for each cube is calculated using the positions of the cube and the rotation vector (1, 1, 0). Note that since the camera is in the same position for each cube we send the view and projection uniforms to the shader before we loop through the cubes.

Change position of the camera to (0, 0, 5) and the target to (0, 0, 0),  compile and run and you should see the following.

```{figure} ../images/06_multiple_cubes.png
:width: 500
```

If you are having difficulty getting to this stage take a look at [main.cpp](../code/Lab06_3D_worlds/main.cpp), [camera.hpp](../code/Lab06_3D_worlds/camera.hpp) and [camera.cpp](../code/Lab06_3D_worlds/camera.cpp).

## A camera class

Our render loop is starting to look a bit messy and it would make sense to write a class to deal with the view and projection operations. Create a header and code file called `camera.hpp` and `camera.cpp` respectively in your `source/` folder. In the header file add the following code

```cpp
#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera
{
public:
    // Projection parameters
    float fov = glm::radians(45.0f);
    float aspect = 1024.0f / 768.0f;
    float near = 0.02f;
    float far = 100.0f;
    
    // Camera vectors
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 worldUp;
    
    // Transformation matrices
    glm::mat4 view;
    glm::mat4 projection;
    
    // Constructor
    Camera(const glm::vec3 Position);
    
    // Methods
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    void calculateMatrices();
};
```

The `getViewMatrix()` and `getProjectionMatrix()` functions will be used to return the `view` and `projection` matrices and the `calculateMatrices()` function will be used to calculate these matrices (you can see I like sensible function names).

In the `camera.cpp` code file add the following code

```cpp
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.hpp"

Camera::Camera(const glm::vec3 Position)
{
    position = Position;
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::getViewMatrix()
{
    return view;
}

glm::mat4 Camera::getProjectionMatrix()
{
    return projection;
}

void Camera::calculateMatrices()
{
    // Calculate view matrix
    view = glm::lookAt(position, target, worldUp);
    
    // Calculate projection matrix
    projection = glm::perspective(fov, aspect, near, far);
}
```

The `Camera()` constructor takes in a single input of the camera position and instantiates the `position` attribute with this position. The `target` and `worldUp` vectors are instantiated with default values.

Now we've created our Camera class we need to include it and make calls to the library functions. Add `#include "camera.hpp"` to the top of the `main.cpp` file and create a Camera object before the `main()` function declaration.

```cpp
// Create camera object
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
```

Now all we need to do is replace the code where we calculate the view and projection matrices with the following.

```cpp
// Get the view and projection matrices from the camera library
camera.calculateMatrices();
glm::mat4 view = camera.getViewMatrix();
glm::mat4 projection = camera.getProjectionMatrix();
```

Compile and run your code and it should give the same result as before.

---

## Exercises

1. Experiment with changing the camera position and target to see the effect this has on the 3D world.
2. Experiment with changing the field of view angle to see the effect this has.
3. Use orthographic projection for the view to screen space transformation.
4. Rotate the camera position around the first cube. Hint: $x = \tt radius \cdot \cos(\tt time)$ and $z = \tt radius \cdot \sin(\tt time)$ gives the co-ordinates on a circle centred at (0,0,0).
5. Rotate every other cube whilst keeping the remaining cubes stationary.

---

## Source code

The source code for this lab, including the exercise solutions, can be downloaded using the links below.

- [main_no_depth_test.cpp](../code/Lab06_3D_worlds/main_no_depth_test.cpp) - single cube with orthographic projection and no depth testing
- [main.cpp](../code/Lab06_3D_worlds/main.cpp) - multiple cubes using the Camera class
- [camera.hpp](../code/Lab06_3D_worlds/camera.hpp)
- [camera.cpp](../code/Lab06_3D_worlds/camera.cpp)
- [vertexShader.vert](../code/Lab06_3D_worlds/vertexShader.vert)
- [main_exercise_solutions.cpp](../code/Lab06_3D_worlds/main_exercise_solutions.cpp)
- [camera_exercise_solutions.cpp](../code/Lab06_3D_worlds/camera_exercise_solutions.cpp)

---
## Video walkthrough

The video below walks you through these lab materials.

<iframe width="560" height="315" src="https://www.youtube.com/embed/kKxjh1FGw0E?si=oTSZRExkG6A9pQ3b" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>