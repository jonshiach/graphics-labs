(hello-triangle-section)=

# Basic Shapes in OpenGL

In this lab we will be creating our first graphics application in OpenGL.

(hello-window-section)=
## Hello window

To start using OpenGL we are going to have to download and compile several libraries and configure the IDE (we will be using Visual Studio on Windows). This can be quite an involved process (see <a href="https://learnopengl.com/Getting-started/Creating-a-window" target="_blank">Learn OpenGL</a> for a detailed tutorial on doing this). However, I have adapted the excellent resource <a href="https://www.opengl-tutorial.org/" target="_blank">opengl-tutorial.org</a> to do this for us. 

1. Go to <a href="https://github.com/jonshiach/Lab02_Basic_shapes" target="_blank">https://github.com/jonshiach/Lab02_Basic_shapes</a> and follow the instructions to download and build the project files.
2. Open the project file `Lab02_Basic_shapes.sln` (or `Lab02_Basic_shapes.xcodeproj` on macOS) set the **Lab02_Basic_shapes** project as the startup project.
    - Visual Studio: right-click on the **Lab02_Basic_shapes** project and select **Set as Startup Project**.
    - Xcode: Click on the target select dropdown (to the right of the name of the project at the top of the window) and select **Lab02_Basic_shapes** as the target.

3. Build the project by pressing CTRL + B (or ⌘B on Xcode) which should build the project without errors. Run the executable by pressing F5 (or ⌘R on Xcode).

If all has gone to plan you should be looking at a boring window with a grey background. Familiarise yourself with the source files. For now, this contains the main C++ program `main.cpp`, the header file `shader.hpp` and associated code file `shader.cpp` in the `source/` folder.

```{figure} ../images/02_hello_window.png
:width: 500

The "hello window" example (boring isn't it)
```

You can terminate your application by pressing the escape key or simply closing the window.

---

## Define a triangle

As you will probably agree, creating a plain grey window isn't the most interesting of applications. What would make it much more exciting is to draw simple shapes in the window. The simplest shape, and one which we use extensively in computer graphics, is a triangle.

The first thing we need to do is create the <a href="https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object" target="_blank">**Vertex Array Object (VAO)**</a> which is a container for the vertex attributes and associated vertex data (e.g., the vertex co-ordinates, colour, texture, normal vectors etc.) and bind it so that OpenGL knows that this is the current object it is dealing with. Enter the following into your `main.cpp` file before the do/while loop.

```cpp
// Create the Vertex Array Object (VAO)
GLuint VAO;
glGenVertexArrays(1, &VAO);
glBindVertexArray(VAO);
```

```{note}
Here we have defined the integer `VAO` using `GLunit` instead of plain old `unsigned int`. `GLuint` stands for <a href="https://www.khronos.org/opengl/wiki/OpenGL_Type" target="_blank">**OpenGL unsigned integer**</a> and we use this because different architectures stores variables using different memory sizes. In order to ensure our application works across various platforms we use OpenGL types.
```

OpenGL expects the x, y and z co-ordinates of all vertices to be between -1.0 and 1.0 where the x and y axes point to the right and up respectively and the z axes points out from the screen (these are known as **Normalised Device Co-ordinates (NDC)** - more on this later). For now we are going to draw a triangle with vertex co-ordinates (-0.5,-0.5,0), (0.5,-0.5,0) and (0,0.5,0) for the bottom-left, bottom-right and top vertices respectively.

```{figure} ../images/02_opengl_window.svg
:width: 400

OpenGL Normalised Device Co-ordinates are in the range -1 to 1.
```

We define these co-ordinates in an array of floats called `vertices` using the following code.

```cpp
// Define vertex positions
static const GLfloat vertices[] = {
    // x     y      z
    -0.5f, -0.5f,  0.0f,
     0.5f, -0.5f,  0.0f,
     0.0f,  0.5f,  0.0f
};
```

(vbo-section)=

We now need to define a <a href="https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Buffer_Object" target="_blank">**Vertex Buffer Object (VBO)**</a> which is how we transfer the vertex data from the CPU to the GPU.

```cpp
// Create Vertex Buffer Object (VBO)
GLuint vertexBuffer;
glGenBuffers(1, &vertexBuffer);
glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

Here, after creating and binding the vertex buffer we copy across the co-ordinates stored in the `vertices` array to the vertex buffer using the `glBufferData()` function.

---

## Write and compile the shader

Now we have defined our triangle and copied the information over to OpenGL we now need to tell OpenGL how to display the triangle. This is done using a shader program that OpenGL uses to tell it how to display each pixel in our window. The shader programs are written in <a href="https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)" target="_blank">**GLSL (OpenGL Shader Language)**</a> which is a language similar to C.

```{figure} /images/02_shaders.svg
:width: 600
```

At its most basic it consists of a <a href="https://www.khronos.org/opengl/wiki/Vertex_Shader" target="_blank">**vertex shader**</a> and a <a href="https://www.khronos.org/opengl/wiki/Fragment_Shader" target="_blank">**fragment shader**</a>. The vertex shader is called once for each vertex and calculates the position of the current vertex and stores it in a special GLSL vector `gl_Position`. The vertices that form a polygon are then passed to OpenGL which maps the polygon to the corresponding pixels on the display. A **fragment** is a data structure that represents a potential pixel on the display. The fragment shader is called once for each pixel in the fragment and determines the colour of that pixel.

The shaders are compiled by the application at runtime.

(vertex-shader-section)=

### Vertex shader

Click on **File > New > File...** (or just press CTRL + N) and select text file. Enter the following program into the new file and save it in the `Lab02_Basic_shapes/source` folder using the filename `simpleVertexShader.vert` (the file extension doesn't matter but it is common to use `.vert` to denote the vertex shader).

```{important}
Make sure the file is saved with the correct file extension, i.e., `simpleVertexShader.vert` and not something like `simpleVertexShader.vert.txt` else the shader program will not be able to find the vertex shader.
```

```glsl
#version 330 core

layout(location = 0) in vec3 position;

void main() 
{
    gl_Position = vec4(position, 1.0);
}
```

This is the GLSL program for a simple vertex shader. It takes in a single 3-element vector `position` that contains the (x,y,z) co-ordinates of a vertex and outputs the 4-element vector `gl_Position` containing the co-ordinates. The individual elements of a vector in GLSL can be accessed using `<vector name>.x`, `<vector name>.y` and `<vector name>.z` so we could have used the following instead.

```cpp
gl_Position = vec4(position.x, position.y, position.z, 1.0)
```

You may be wondering why we have this additional element `1.0`, don't worry about this for now, it will be explained [later on](translation-section).

### Fragment shader

Create a new file as you did with the vertex shader but with the following code and save it using the filename `simpleFragmentShader.frag` (again, makes sure the file has the correct extension).

```glsl
#version 330 core

// Output data
out vec4 colour;

void main() 
{
    colour = vec4(1.0, 0.0, 0.0, 1.0);    // RGBA
}
```

This fragment shader outputs a single 4-element vector called `colour` which defines the colour of the fragment using RGBA which stands for Red-Green-Blue-Alpha. The values are in the range 0 to 1 and so here we have red = 1, blue = 0, green = 0 so our pixel (and all pixels in the triangle) will be rendered in red with an Alpha value of 1 which means it is fully opaque.

### Shader program

We now need to combine the vertex and fragment shaders into a single shader program. To do this we are going use the function `LoadShaders()` written by contributors of <a href = "https://www.opengl-tutorial.org" target="_blank">opengl-tutorial.org</a>. The header and code files are already in the `source/` folder and the `shader` class has been included at the top of the `main.cpp` file.

We can now compile the shader program using the `LoadShaders()` function.

```cpp
// Compile shader program
GLuint shaderID = LoadShaders("simpleVertexShader.vert", "simpleFragmentShader.frag");
```

This code creates a program object which will be referred to by `shaderID`. The code for the `LoadShaders()` function can be seen in the `shaders.cpp` file (we aren't really concerned with this but it's nice to know what's going on).

After you have created the shader files the basic structure of your `Lab02_Basic_shapes/` folder should resemble the following. 

```text
Lab02_Basic_shapes/
├── build
├── external/
│   ├── glew
│   └── glfw
└── source/
    ├── main.cpp
    ├── shader.hpp
    ├── shader.cpp
    ├── simpleVertexShader.vert
    └── simpleFragmentShader.frag
```

---

## Draw the triangle

Finally, we can now draw the triangle. After clearing the window, we need to instruct OpenGL to use our shader program so enter the following code (this can be anywhere after the shader program has been compiled but before the do/while loop).

```cpp
// Use the shader program
glUseProgram(shaderID);
```

Now we need to bind the vertex buffer to the VAO to send all of the data to the shaders for OpenGL to interpret. In the do/while loop add the following code after the window has been cleared.

```cpp
// Send the vertex buffer to the shaders
glEnableVertexAttribArray(0);
glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
glVertexAttribPointer(
                      0,           // attribute
                      3,           // size
                      GL_FLOAT,    // type
                      GL_FALSE,    // normalise
                      0,           // stride
                      (void*)0     // offset
                      );
```

The three functions we've used here are:

 - `glEnableVertexAttribArray()` enables a generic vertex array so we can pass our triangle data to OpenGL;
 - `glBindBuffer()` binds our vertex buffer to OpenGL;
 - `glVertexAttribPointer()` tells OpenGL how to interpret the data we are sending it. The input arguments are explained below.

| Argument | Explanation |
|:--|:--|
| Attribute | A number that defines which vertex attribute we want to configure. In the vertex shader we used `location = 0` for the vertex co-ordinates and since we are passing vertex co-ordinates, we set the attribute to 0. |
| Size | How many values does the vertex attribute have. Here we have (x,y,z) co-ordinates so this is 3. |
| Type | Our co-ordinates are floats. |
| Normalise | We have already set out vertex co-ordinates in NDC (i.e., in the range -1 to 1) so we set this to false. |
| Stride | The space between consecutive vertex attributes. Here one vertex immediately follows the next, so this is zero. |
| Offset | Where does the first data point appear in the buffer? For us this is at the beginning, so we set it to 0. |

Now we instruct OpenGL to draw the triangle.

```cpp
// Draw the triangle
glDrawArrays(GL_TRIANGLES, 0, 3);
glDisableVertexAttribArray(0);
```

The `glDrawArrays()` command tells OpenGL that we wish to draw triangles, the `0` specifies that the first vertex starts at the 0 index in the buffer and the `3` specifies the number of vertices we want to draw. Since we no longer need the vertex attribute array, we disable it using `glDisableVertexAttribArray()`.

Don't get too excited just yet. As good programmers we should clean up after ourselves and not leave bits of data lying around. After the close of the do/while loop we de-allocate the vertex and buffer objects as well as deleting the shader program.

```cpp
// Cleanup
glDeleteBuffers(1, &vertexBuffer);
glDeleteVertexArrays(1, &VAO);
glDeleteProgram(shaderID);
```

Compile and run your program. After all the syntax errors and bugs have been resolved (unless you are very lucky there will be at least one) you should be presented with a window within which is your red triangle that you have created. Note that the window shown below no longer has the title "Hello Window", can you change your code so that your window has the title "Hello Triangle"?

```{figure} ../images/02_hello_triangle.png
:width: 500

The "hello triangle" example
```

If you are having trouble getting to this stage, the code for producing this triangle is given [here](../code/Lab02_Basic_shapes/main.cpp).

---

## More colours

After basking in the glory of your achievements for a few minutes the initial excitement may begin to wane, and your natural curiosity will cause you to wonder whether we can use more than one colour. Well of course, all we need to do is tell OpenGL what colours we want to use for each vertex.

Create an array of float that contains the RGB colour data for each vertex.

```cpp
// Define vertex colours
static const GLfloat colours[] = {
    // R   G     B
    1.0f, 0.0f, 0.0f,    // red
    0.0f, 1.0f, 0.0f,    // green
    0.0f, 0.0f, 1.0f,    // blue
};
```

Here we have assigned the colour red to the first (bottom-left) vertex, green to the second (bottom-right) vertex and blue to the third (top) vertex.

Like the vertex buffer, we need to create and bind a buffer for the colours.

```cpp
// Create colour buffer
GLuint colourBuffer;
glGenBuffers(1, &colourBuffer);
glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
```

And where we draw the triangle, we also need to bind the colour information to the VAO so it can be sent to the shaders.

```cpp
// Send colours to the shaders
glEnableVertexAttribArray(1);
glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
glVertexAttribPointer(
                      1,           // attribute
                      3,           // size
                      GL_FLOAT,    // type
                      GL_FALSE,    // normalise
                      0,           // stride
                      (void*)0     // offset
                      );
```

If you were to compile and run your program, you might be a little disappointed as your triangle is still red. Well of course, we haven't told our shaders how to handle colours! Since our colours are associated with the vertices we need to modify the vertex shader to include the colours.

```glsl
#version 330 core

// Input vertex data
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;

// Output data
out vec3 vertexColour;

void main()
{
    // Output vertex position
    gl_Position = vec4(position, 1.0);
    
    // Output vertex colour
    vertexColour = colour;
}
```

Here our vertex shader is a little more sophisticated than before. We've added another attribute with `location = 1` for the colour data which is a 3-element vector. Also, since we need to pass the colour data to the fragment shader we need to output it from the vertex shader (the vertex data is outputted automatically in `gl_Position`).

The changes to the fragment shader are just the addition of the colour input.

```cpp
#version 330 core

// Input data
in vec3 vertexColour;

// Output data
out vec4 colour;

void main() 
{
    colour = vec4(vertexColour, 1.0);    // rgba
}
```

Compile and run your program and if everything has gone to plan you should be presented with your new triangle in all its colourful snazzy goodness. Notice how the pixels in between the three vertex pixels have been shaded a colour which are combinations of the three vertex colours red, green and blue. OpenGL has interpolated the colours across the triangle.

```{figure} ../images/02_hello_snazzy_triangle.png
:width: 500

Our snazzy triangle.
```

---

## Adding another triangle

What could be better than one triangle? Well two triangles of course. Fortunately since we have done all of the grunt work in setting up the buffers for a single triangle adding another is a simple matter of defining the vertex co-ordinates and vertex colours for the additional triangle. Modify the `vertices[]` and `colours[]` arrays to the following.

```cpp
// Define vertices
static const GLfloat vertices[] = {
    -0.9f, -0.5f, 0.0f,    // triangle 1
    -0.1f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
     0.1f, -0.5f, 0.0f,    // triangle 2
     0.9f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f
};

// Define colours
static const GLfloat colours[] = {
    1.0f, 0.0f, 0.0f,    // red
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,    // blue
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
};
```

Here the `vertices[]` array now defines six vertices for two triangles placed side-by-side. The `colours[]` array defines the first three vertices red and the second three set of vertices blue.

We also need to instruct OpenGL to draw two triangles instead of one. To do this we change the number of vertices we want to draw from `3` to the number of vertices we have. Since each vertex has 3 co-ordinates (x, y, z) and each co-ordinate is a single `GLfloat` then we can calculate the number of vertices we have by dividing `sizeof(vertices)` by `3 * sizeof(GLfloat)`.

```cpp
// Draw the triangles
glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (3 * sizeof(GLfloat)));
```

Compiling and running the executable results in the following.

```{figure} ../images/02_two_triangles.png
:width: 500

Two triangles
```

---

## Exercises

Now that you've got to the stage where you can draw triangles to the screen and alter the colours lets see if you can do the following.

1. Draw the original triangle but alter the vertex shader to achieve the following results:
    
    (a) the triangle is shifted by 0.5 to the right; <br>
    (b) the triangle is drawn upside-down;<br>
    (c) the triangle x and y co-ordinates are swapped.

`````{grid}
````{grid-item}
```{figure} ../images/02_Ex1a.png
```
````

````{grid-item}
```{figure} ../images/02_Ex1b.png
```
````

````{grid-item}
```{figure} ../images/02_Ex1c.png
```
````
`````

2. Use two triangles to draw a green rectangle where the lower-left corner has co-ordinates (-0.5, -0.5, 0.0) and the upper-right corner has co-ordinates (0.5, 0.5, 0.0).

```{figure} ../images/02_Ex2.png
:width: 500
```

3. Use six different coloured triangles to draw a hexagon.

```{figure} ../images/02_Ex3.png
:width: 500
```

## Source code

The source code for this lab, including the exercise solutions, can be downloaded using the links below.

- Hello triangle: [main.cpp](../code/Lab02_Basic_shapes/main.cpp), [simpleVertexShader.vert](../code/Lab02_Basic_shapes/simpleVertexShader.vert), [simpleFragmentShader.frag](../code/Lab02_Basic_shapes/simpleFragmentShader.frag)
- Colour shaders: [colourVertexShader](../code/Lab02_Basic_shapes/colourVertexShader.vert), [colourFragmentShader.frag](../code/Lab02_Basic_shapes/colourFragmentShader.frag)
- Exercise solutions: [main.cpp](../code/Lab02_Basic_shapes/Lab02_solutions.cpp), [ex1VertexShader.vert](../code/Lab02_Basic_shapes/ex1VertexShader.vert)

---

## Video walkthrough

The video below walks you through these lab materials.

<iframe width="560" height="315" src="https://www.youtube.com/embed/tsokaomZfZY?si=zAKqb8nZZ5BGdfW0" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>