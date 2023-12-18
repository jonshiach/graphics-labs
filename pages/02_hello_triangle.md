# Creating a triangle in OpenGL

In this lab we will be creating our first graphics application in OpenGL.

## Hello window

To start using OpenGL we are going to have to download and compile several libraries and configure the IDE (we will be using Visual Studio on Windows). This can be quite an involved process (see <a href="https://learnopengl.com/Getting-started/Creating-a-window" target="_blank">Learn OpenGL</a> for a detailed tutorial on doing this). However, I have adapted the excellent resource <a href="https://www.opengl-tutorial.org/" target="_blank">opengl-tutorial.org</a> to do this for us.

1. Download the file **GraphicsLabs.zip** from Moodle and extract the contents to your OneDrive area.

2. Copy the **GraphicsLabs/** folder over to a sensible area on your OneDrive folder.

3. Create a folder in the **GraphicsLabs/** and call this something sensible like 'VS_projects' for Visual Studio.

4. Run CMake and select the **GraphicsLabs/** folder for the source code and the folder you created in step 2 for where to build the binaries.

5. Click 'Configure' (near the bottom) and select your IDE (in our case this is Visual Studio) and click on 'Finish'. Click 'Configure' again until the red background disappears.

6. Click 'Generate' which will create your project files in the folder you created in step 2. Open the project file **GraphicsLabs.sln** and right-click on the **lab01_hello_triangle** project and select 'Set as Startup Project.'

7. Build the project by pressing CTRL + B which should build without errors. Run the executable by pressing F5.

If all has gone to plan you should be looking at a boring window with a grey background. Familiarise yourself with the source files. For now, this is quite simple and just contains the main C++ program lab01_hello_triangle.cpp in the **Source Files/** folder and the C++ and header files **shader.cpp** and **shader.hpp** in the **common/** folder.

```{figure} ../images/hello_window.png
:width: 500

The 'hello window' example (boring isn't it)
```

---

## Define our triangle

As you will probably agree, creating a plain grey window isn't the most interesting of applications. What would make it much more exciting is to draw simple shapes in the window. The simplest shape, and one which we use extensively in computer graphics, is a triangle.

The first thing we need to do is create the **Vertex Array Object (VAO)**. Enter the following into your **lab01_hello_triangle.cpp** file before the `do` loop.

```cpp
// Create the Vertex Array Object (VAO)
GLuint vertexArrayID;
glGenVertexArrays(1, &vertexArrayID);
glBindVertexArray(vertexArrayID);
```

```{note}
Here we have defined the integer `vertexArrayID` using `GLunit` instead of plain old `unsigned int`. `GLuint` stands for *OpenGL unsigned integer* and we use this because different architectures stores variables using different memory sizes. In order to ensure our application works across various platforms we use OpenGL types.
```

OpenGL expects the $x$, $y$ and $z$ co-ordinates of all vertices to be between -1.0 and 1.0 where the $x$ and $y$ axes point to the right and up respectively and the $z$ axes points out from the screen (these are known as **Normalised Device Co-ordinates (NDC)** - more on this later). For now we are going to draw a triangle with vertex co-ordinates (-0.5,-0.5,0), (0.5,-0.5,0) and (0,0.5,0) for the bottom-left, bottom-right and top vertices respectively.

```{figure} ../images/opengl_window.svg
:width: 400

OpenGL Normalise Device Co-ordinates are in the range -1.0 to 1.0.
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

We now need to define a **Vertex Buffer Object (VBO)** and copy the vertices information to the vertex buffer.

```cpp
// Create Vertex Buffer Object
GLuint vertexBuffer;
glGenBuffers(1, &vertexBuffer);
glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

---

## Write and compile the shader

Now we have defined our triangle and copied the information over to OpenGL we now need to tell OpenGL how to display the triangle. This is done using a shader program that OpenGL uses to tell it how to display each pixel in our window. At its most basic it consists of a **vertex shader** which deals with the positions of the vertices of the objects (think of our triangle) being displayed and the **fragment shader** which calculates the colour of the pixels that make up the object. The shader programs are written in **GLSL (openGL Shading Language)** which is similar to C.

(vertex-shader-section)=

### Vertex shader

Click on **File > New > File...** (or just press CTRL + N) and select text file. Enter the following program into the new file and save it in the **GraphicsLabs/lab01_hello_triangle/** folder using the filename **simpleVertexShader.vs**.

```glsl
#version 330 core

layout(location = 0) in vec3 position;

void main() {

    gl_Position = vec4(position.x, position.y, position.z, 1.0);

}
```

This is the GLSL program for a simple vertex shader. It takes in a single 3-element vector `vertexPosition` that contains the $(x,y,z)$ co-ordinates of a vertex and forwards a 4-element vector to the fragment shader. You may be wondering why we have this addition element, don't worry about this for now, it will be explained [later on](translation-section)).

### Fragment shader

Create a new file as you did with the vertex shader but with the following code and save it using the filename **simpleFragmentShader.fs**.

```glsl
#version 330 core

// Output data
out vec4 colour;

void main() {

    colour = vec4(1.0, 0.0, 0.0, 1.0);    // RGBA

}
```

The fragment shader outputs a single 4-element vector called colour which defines the colour of the pixel using RGBA which stands for Red-Green-Blue-Alpha. The values are in the range 0 to 1 and so here we have red = 1, blue = 0, green = 0 so our pixel will be rendered in red and alpha = 0 which means it is fully opaque.

### Shader program

We now need to combine the vertex and fragment shaders into a single shader program which is done using the following code.

```cpp
// Compile shader program
GLuint shaderID = LoadShaders("simpleVertexShader.vs", "simpleFragmentShader.fs");
```

This code creates a program object which can be referred to by `shaderID`. The code for the `LoadShaders()` function can be seen in the **shaders.cpp** file in the common folder (we aren't really concerned with this but it's nice to know what's going on).

---

## Draw the triangle

Finally, we can now draw the triangle. Any commands to draw objects in our window go inside the `do` loop.

After clearing the screen, we need to instruct OpenGL to use our shader program so enter the following code.

```cpp
// Use the shader program
glUseProgram(shaderID);
```

Now we need to send the vertex array to the shaders for OpenGL to interpret.

```cpp
// Send vertices to buffer
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

`glEnableVertexAttribArray();` enables a generic vertex array so we can pass our triangle data to OpenGL and `glBindBuffer();` binds our vertex buffer to OpenGL. `glVertexAttribPointer()` tells OpenGL how to interpret the data we are sending it. The input arguments are explained below.

| Argument | Explanation |
|:--|:--|
| Attribute | A number that defines which vertex attribute we want to configure. In the vertex shader we used `location = 0` for the vertex co-ordinates and since we are passing vertex co-ordinates, we set the attribute to 0. |
| Size | How many values does the vertex attribute have. Here we have $(x,y,z)$ co-ordinates so this is 3. |
| Type | Our co-ordinates are floats. |
| Normalise | We have already set out vertex co-ordinates in NDC (i.e., in the range -1 to 1) so we set this to false. |
| Stride | The space between consecutive vertex attributes. Here one vertex immediately follows the next, so this is zero. |
| Offset | Where does the first data point appear in the buffer. For us this is at the beginning, so we set it to 0. |

Now we instruct OpenGL to draw the triangle.

```cpp
// Draw the triangle
glDrawArrays(GL_TRIANGLES, 0, 3);
glDisableVertexAttribArray(0);
```

The `glDrawArrays()` command tells OpenGL that we wish to draw triangles, the `0` specifies that the first vertex starts at the 0 index in the buffer and the `3` specifies the number of vertices we want to draw. Since we no longer need the vertex attribute array, we disable it.

Don't get too excited just yet. As good programmers we should clean up after ourselves and not leave bits of data lying around. After the close of the `do` loop we de-allocate the vertex and buffer objects as well as deleting the shader program.

```cpp
// Cleanup
glDeleteBuffers(1, &vertexBuffer);
glDeleteVertexArrays(1, &VertexArrayID);
glDeleteProgram(shaderID);
```

Compile and run your program. After all the syntax errors and bugs have been resolved (unless you are very lucky there will be at least one) you should be presented with a window within which is your red triangle that you have created. Note that this window no longer has the title 'Hello Window', can you change your code so that your window has a more accurate title?

```{figure} ../images/hello_triangle.png
:width: 500

The 'hello triangle' example
```

If you are having trouble getting to this stage, the code for producing this triangle is given {download}`here<../code/lab01_hello_triangle.cpp>`

---

## More colours

After basking in the glory of your achievements for a few minutes the initial excitement may begin to wane, and your natural curiosity will cause you to wonder whether we can use more than one colour. Well of course, all we need to do is tell OpenGL what colours we want to use for each vertex.

Create an array of float that contains the RGB colour data for each vertex.

```cpp
// Define vertex colours
static const GLfloat colours[] = {
    1.0f, 0.0f, 0.0f,    // red
    0.0f, 1.0f, 0.0f,    // green
    0.0f, 0.0f, 1.0f,    // blue
};
```

Here we have assigned the colour red to the first (bottom-left) vertex, green to the second (bottom-right) vertex and blue to the third (top) vertex.

Like the vertex array buffer, we need to create and bind a colour buffer.

```cpp
// Create colour buffer
GLuint colourBuffer;
glGenBuffers(1, &colourBuffer);
glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
```

And where we draw the triangle, we also need to send the colour buffer to the shaders.

```cpp
// Send colours to buffer
glEnableVertexAttribArray(1);
glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
```

If you were to compile and run your program, you might be a little disappointed as your triangle is still red. Well of course, we haven't told our shaders how to handle colours! Since our colours are associated with the vertices we need to modify the vertex shader to include the colours.

```glsl
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;

// Output data
out vec3 vertexColour;

void main() {

    // Positions
    gl_Position = vec4(position.x, position.y, position.z, 1.0);

    // Colours
    vertexColour = colour;

}
```

Here our vertex shader is a little more sophisticated than before. We've added another attribute with `location = 1` for the colour data which is a 3-element vector. Also, since we need to pass the colour data to the fragment shader we need to output it from the vertex shader (the vertex data is output automatically).

The changes to the fragment shader are just the addition of the colour input.

```cpp
#version 330 core

// Input data
in vec3 vertexColour;

// Output data
out vec4 colour;

void main() {

    colour = vec4(vertexColour, 0);    // rgba

}
```

Compile and run your program and if everything has gone to plan you should be presented with your new triangle in all its colourful snazzy goodness. Notice how the pixels in between the three vertex pixels have been shaded a colour which are combinations of the three vertex colours red, green and blue. OpenGL has interpolated the colours across the triangle.

```{figure} ../images/hello_snazzy_triangle.png
:width: 500

Our snazzy triangle.
```

---

## Adding another triangle

What's better than one triangle, well two triangles of course. Fortunately since we have done all of the grunt work in setting up the buffers for a single triangle adding another is a simple matter of defining the vertex co-ordinates and vertex colours for the additional triangle. Modify the `vertices[]` and `colours[]` arrays to the following.

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

We also need to instruct OpenGL to draw two triangles instead of one. To do this we simply change the number of vertices we want to draw from `3` to `sizeof(vertices)` which is the number of elements in the `vertices` array.

```cpp
// Draw the triangle
glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
```

Compiling and running the executable results in the following.

```{figure} ../images/two_triangles.png
:width: 500

Two triangles
```

---

## Exercises

Now that you've got to the stage where you can draw triangles to the screen and alter the colours lets see if you can do the following.

1. Draw the original triangle but alter the vertex shader to achieve the following results.
    - the triangle is shifted by 0.5 to the right;
    - the triangle is drawn upside-down;
    - the triangle $x$ and $y$ co-ordinates are swapped.

2. Use two triangles to draw a green rectangle where the lower-left corner has co-ordinates (-0.5, -0.5, 0.0) and the upper-right corner has co-ordinates (0.5, 0.5, 0.0).

3. Draw a hexagon constructed from different coloured triangles.
