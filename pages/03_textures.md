(textures-section)=

# Textures

Texture mapping is a technique for applying a 2D image known as a **texture** onto a 3D surface. Applying a texture adds detail and complexity to the appearance of 3D objects without the need for modelling intricate geometry.

```{figure} ../images/03_texture_mapping.svg
:width: 600

Mapping a texture to a polygon.
```

The texture is a 2D <a href="https://en.wikipedia.org/wiki/Bitmap" target="_blank">**bitmap**</a>, usually of an image, where each pixel within the texture, known as a **textel**, is referenced using the **texture coordinates** given as (u,v) where u and v are in the range 0 to 1, i.e., (0,0) corresponds to the textel in the bottom left corner and (1,1) corresponds to the textel in the top-right corner. When a fragment is created by the shader the corresponding texture co-ordinates are calculated and the sample colour of the textel is used for the fragment. Fortunately we do not need to write a texture mapper functions since these are in OpenGL.

Lets apply a texture map to our triangle from [Lab 2](hello-triangle-section). First download and build the project files for this lab.

1. Go to <a href="https://github.com/jonshiach/Lab03_Textures" target="_blank">https://github.com/jonshiach/Lab03_Textures</a> and follow the instructions to download and build the project files.
2. Open the project file `Lab03_Textures.sln` (or `Lab03_Textures.xcodeproj` on macOS) set the **Lab03_Textures** project as the startup project.
    - Visual Studio: right-click on the **Lab03_Textures** project and select **Set as Startup Project**.
    - Xcode: Click on the target select dropdown (to the right of the name of the project at the top of the window) and select **Lab03_Textures** as the target.

3. Build the project by pressing CTRL + B (or ⌘B on Xcode) which should build the project without errors. Run the executable by pressing F5 (or ⌘R on Xcode).

If all has gone to plan you should see the red triangle example from Lab 2.

```{figure} ../images/03_red_triangle.png
:width: 500
```

## Texture triangle

### Loading a bitmap image

The first thing we need to do is to load a bitmap image and generate the OpenGL texture. To do this we are going to use a function called `loadBMP_custom()` written by contributors of <a href = "https://www.opengl-tutorial.org" target="_blank">opengl-tutorial.org</a> and contained in the `texture.cpp` file in the `source/` folder. Add the following to the `main.cpp` just after we have compiled the shader program.

```cpp
// Create OpenGL texture
GLuint texture1 = loadBMP_custom("crate.bmp");
```

This creates an OpenGL texture which can be referenced by the integer `texture1`. The bitmap was are using here is `crate.bmp` which is stored in the `source/` folder and shows a side of a wooden crate.

```{figure} ../images/03_crate.bmp
:width: 300

The crate texture.
```

(uniforms-section)=
### Uniforms

A <a href="https://www.khronos.org/opengl/wiki/Uniform_(GLSL)" target="_blank">**uniform**</a> is a global shader that remains constant during the execution of the rendering pass and has the same value for all vertices and fragments. Uniforms provide a way to pass data to the shaders so we will use one for passing the texture information to the fragment shader.

To declare a uniform for our texture add the following code to your `main.cpp` file (this can go anywhere before we render the object but it makes sense to put these after we have loaded our texture).

```cpp
// Get a handle for the texture sampler uniform
GLuint texture1ID = glGetUniformLocation(shaderID, "texture1Sampler");
```

This has stored the memory location of the uniform `texture1Sampler` for the shader program into the integer `texture1ID`.

### Vertex shader

Recall that the [vertex shader](vertex-shader-section) deals with the vertex co-ordinates and is used by OpenGL to calculate the co-ordinates of the fragment. So, in addition to passing the (x, y, z) co-ordinates of the vertices we must also pass the (u, v) co-ordinates of the textels that correspond to the vertices. 

Create a new file in the `source/` folder called `textureVertexShader.vert` and edit it so that it looks like the following.

```cpp
#version 330 core

// Input vertex data
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;

// Output data
out vec2 uv;

void main()
{
    // Output vertex postion
    gl_Position = vec4(vertexPosition, 1);
    
    // Output (u,v) co-ordinates
    uv = vertexUV;
}
```

You may notice some changes from our [simple vertex shader](vertex-shader-section). We have a second input `vertexUV` which is a 2-element vector which are the texture coordinates which are passed to the fragment shader as the output `uv` (remember that the `gl_Position` vector is passed to the fragment shader by default).

### Fragment shader

The fragment shader is where we need to retrieve the sample colour from the texture. Create a new file in the `source/` folder called `textureFragmentShader.frag` and edit it so that it looks like the following.

```cpp
#version 330 core

// Interpolated values from the vertex shaders
in vec2 uv;

// Output data
out vec3 colour;

// Values that stay constant for the whole mesh
uniform sampler2D texture1Sampler;

void main ()
{
    // Output colour = colour of the texture at (u, v)
    colour = texture(texture1Sampler, uv).rgb;
}
```

Here we now have an input of the 2-element vector `uv` which has been passed from the vertex shader and also the `texture1Sampler` uniform that we declared in the `main.cpp` file. Since our texture is a 2D image then we use the <a href="https://www.khronos.org/opengl/wiki/Sampler_(GLSL)" target="_blank">`sampler2D`</a> GLSL type to declare the uniform. The colour of the fragment is taken from the texture using the `texture()` function.

Now that we have two brand new shaders we need to instruct the shader program to use these. Change the `LoadShaders()` function to the following.

```cpp
// Compile shader program
GLuint shaderID = LoadShaders("textureVertexShader.vert", "textureFragmentShader.frag");
```

### Texture co-ordinates

For each of the vertices in our triangle we need to define the corresponding (u, v) texture co-ordinates. This is done in a the same way as the triangle vertices, i.e., define and array of co-ordinates and bind the data to an array buffer.

Add the following code to your `main.cpp` file after where we defined the VBO.

```cpp
// Define texture co-ordinates
static const GLfloat uvCoords[] = {
        0.0f,  0.0f,
        1.0f,  0.0f,
        0.5f,  1.0f,
};

// Create texture buffer
GLuint uvBuffer;
glGenBuffers(1, &uvBuffer);
glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(uvCoords), uvCoords, GL_STATIC_DRAW);
```

So here the bottom-left triangle vertex at (-0.5, -0.5, 0) is mapped to the texture co-ordinate (0, 0) at the bottom-left corner of the texture, the bottom-right vertex at (0.5, -0.5, 0) is mapped to (1,0) at the bottom-right corner of the texture and the top vertex at (0, 0.5, 0) is mapped to (0.5, 1) in the middle of the top each fo the texture.

### Send the texture to OpenGL

Now that we have loaded the texture and written our vertex and fragment shaders, the next and final step is to send the texture buffer to OpenGL. This is done in the same way as for the [VBO (Vertex Buffer Object)](vbo-section) but first we need to bind our texture to OpenGL. After we have instructed the application to use our shader program add the following code.

(texture-unit-code)=
```cpp        
// Bind the texture
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, texture1);
glUniform1i(texture1ID, 0);
```

The three functions we've used here are:

- `glActiveTexture()` activates the texture unit `GL_TEXTURE0`.
- `glBindTexture()` sends the texture referenced by `texture1` to the GPU.
- `glUniform1i()` assign the `texture1ID` uniform to the texture unit 0.

Sending the texture to OpenGL is done in the same way as the VBO.

```cpp
// Send the texture buffer to the shaders
glEnableVertexAttribArray(1);
glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
glVertexAttribPointer(
                      1,           // attribute
                      2,           // size
                      GL_FLOAT,    // type
                      GL_FALSE,    // normalise
                      0,           // stride
                      (void*)0     // offset
                      );
```

Note that since the texture has (u, v) co-ordinates instead of (x, y, z) the size attribute is `2` instead of `3`. Compile and run program and, after sending a prayer to the programming gods, you should be presented with a textured triangle.

```{figure} ../images/03_texture_triangle.png
:width: 500
```

If you are having difficulty with this check out the source code and shaders: [main.cpp](../code/Lab03_Textures/texture_triangle.cpp), [textureVertexShader.vert](../code/Lab03_Textures/textureVertexShader.vert), [textureFragmentShader.frag](../code/Lab03_Textures/textureFragmentShader.frag).

## Texture rectangle

Our texture triangle is great and all but doesn't really look like a realistic object. Since the original texture is rectangular, lets create a rectangle out of two triangles with the appropriate texture mapping.

```{figure} ../images/03_rectangle.svg
:width: 500

A rectangle constructed using two triangles.
```

Change the `vertices` and `uvCoords` arrays to the following and run the program.

```cpp
// Define vertex positions
static const GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,    // triangle 1
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,    // triangle 2
     0.5f,  0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
};

// Define texture co-ordinates
static const GLfloat uvCoords[] = {
    // u    v
    0.0f,  0.0f,    // triangle 1
    1.0f,  0.0f,
    1.0f,  1.0f,
    0.0f,  0.0f,    // triangle 2
    1.0f,  1.0f,
    0.0f,  1.0f
};
```

Compile and run the program and you should be presented with the more realistic image below.

```{figure} ../images/03_texture_rectangle.png
:width: 500
```

## Texture wrapping

In our examples above, all of the texture co-ordinates have been in the range from 0 to 1. If we specify co-ordinates outside of this range then OpenGL will repeat the texture across the fragment. This is known as **texture wrapping**.

```{figure} ../images/03_texture_repeat.png
:width: 300

The default texture wrapping in OpenGL is to repeat the texture. 
```

Repeating the texture in this way is useful for when we have a small texture that we want to repeat over a large polygon, for example, think of a brick wall where the basic patterns repeats itself.

OpenGL offers other options for texture wrapping;

- `GL_REPEAT` - the texture repeats over the fragment (default);
- `GL_MIRRORED_REPEAT` - same as `GL_REPEAT` but the texture is mirrored with each repeat;
- `GL_CLAMP_TO_EDGE` - clamps the texture co-ordinates to between 0 and 1, co-ordinates outside of this range are clamped to the edge so that the textels on the edge are stretched to the edge of the fragment;
- `GL_CLAMP_TO_BORDER` - co-ordinates outside of the range (0,0) to (1,1) are given a used defined border colour.

`````{grid}
````{grid-item}
```{figure} ../images/03_texture_mirrored_repeat.png
:width: 300

`GL_MIRRORED_REPEAT` 
```
````

````{grid-item}
```{figure} ../images/03_texture_clamp_to_edge.png
:width: 300

`GL_CLAMP_TO_EDGE`
```
````

````{grid-item}
```{figure} ../images/03_texture_clamp_to_border.png
:width: 300

`GL_CLAMP_TO_BORDER`
```
````
`````

The texture wrapping options can be set for each axis of the texture using the `glTexParameteri()` function. Open the file `texture.cpp` and scroll down a bit and you will see the following code.

```cpp
// Choose wrapping method (comment/uncomment as appropriate)
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
```

Here we can choose what type of texture wrapping we want to use by commenting or uncommenting the code. Note that we can specify the wrapping in the horizontal (`S`) or vertical (`T`) directions (some people, including as it seems the contributors to OpenGL, use (s,t) for the vertex co-ordinates instead of (u,v)).

## Texture filtering

**Texture filtering** is method of determining the colour of the fragment, known as the **colour sample**, from the texture. OpenGL maps the co-ordinates of the fragment to the texture co-ordinates and in most cases this will not align exactly to a textel centre, so what does OpenGL do? OpenGL provides two main options `GL_NEAREST` and `GL_LINEAR`.

`GL_NEAREST` is the default in OpenGL uses the colour of the nearest textel to the texture co-ordinates as the colour sample. This is illustrated in the diagram below where the texture co-ordinates represented by the black circle is mapped in a region on the texture with four neighbouring textels with the textel centres represented by the crosses. The texture co-ordinates are closed to the centre of the textel in the top-left so the colour of that textel is used for the colour sample.

```{figure} ../images/03_nearest_filtering.svg
:width: 400
```

However, the texture co-ordinate is quite close to three other neighbouring textels so shouldn't the colours of these be taken into account? Another method is to calculate the sample colour using <a href="https://en.wikipedia.org/wiki/Bilinear_interpolation" target="_blank">(bi)linear</a> interpolation where the distance of a textels centre from the texture co-ordinate determines how much that textel contributes to the sample colour, i.e., the closer the textel the more of the textel colour is contained in the colour sample. This can be applied using the `GL_NEAREST` option.

```{figure} ../images/03_linear_filtering.svg
:width: 400
```

The effects of these two filtering methods are shown in {numref}`nearest-filtering-figure` and {numref}`linear-filtering-figure` where a low resolution texture has been used on a large object. Using the `GL_NEAREST` results in a blocky, or **aliased** image whereas using `GL_LINEAR` results in a smoother image albeit a bit blurred (nothing we can do about this).

`````{grid}
````{grid-item}
```{figure} ../images/03_nearest_filtering.png
:width: 200
:name: nearest-filtering-figure

`GL_NEAREST`
```
````

````{grid-item}
```{figure} ../images/03_linear_filtering.png
:width: 200
:name: linear-filtering-figure

`GL_LINEAR`
```
````
`````

When the fragment is larger then the texture we are up scaling the texture which can result in the blocky appearance seen in {numref}`nearest-filtering-figure` so `GL_LINEAR` would be the preferred filtering method. Texture filtering can be set for **magnifying** or **minifying** (i.e., when up scaling or down scaling) using `GL_TEXTURE_MAG_FILTER` and `GL_TEXTURE_MIN_FILTER`, for example

```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
```

## Mipmaps

Another issue that may occur is when the fragment is a lot smaller than the texture which can happen when an object that is far away from the viewer. In these cases OpenGL will struggle to get the colour sample from a high resolution texture since a fragment covers a larger part of the texture.

To solve this issue OpenGL uses <a href="https://www.khronos.org/opengl/wiki/Texture#Mip_maps" target="_blank">**mipmaps**</a>[^1] which are a series of textures, each one half the size of the previous one. OpenGL will use a mipmap texture most suitable based on the distance of the fragment from the viewer. This way the fragment does not span a large part of the texture and it also cuts down on memory.

[^1]: "mip" is short for the latin phrase *"multum in parvo"* or "much in little".

```{figure} ../images/03_mipmaps.svg
:width: 500

Mipmaps
```

The good news is that we do not need to create lots of new different size textures because OpenGL has a function `glGenerateMipmap()` to do this for us. One issue we may encounter is that when we switch between two mipmaps, e.g., when the viewer is moving towards or away from an object, there can be a notable change in appearance of the object. This is known as **texture popping** and is caused by switching between two mipmaps. To overcome this OpenGL gives the option to sample the texture from a linear interpolation between the two nearest mipmaps. So we have two main texture filtering options and two mipmap options (nearest or linear interpolation) giving four main mipmap options:

- `GL_NEAREST_MIPMAP_NEAREST` - uses nearest texture filtering on the nearest mipmap;
- `GL_LINEAR_MIPMAP_NEAREST` - uses linear texture filtering on the nearest mipmap;
- `GL_NEAREST_MIPMAP_LINEAR` - uses nearest texture filtering on a linear interpolation between two mipmaps;
- `GL_LINEAR_MIPMAP_LINEAR` - uses linear texture filtering on a linear interpolation between two mipmaps.

Like with the texture filtering methods we can use different options for magnifying and minifying the texture. Take a look at the `texture.cpp` file in the `source/` folder where the following code is used to specify the options.

```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glGenerateMipmap(GL_TEXTURE_2D);
```

## Multiple textures

In our texture mapping [example above](texture-unit-code) we used the `glActiveTexture()` command to activate the **texture unit** `GL_TEXTURE0`. OpenGL allows us to use up to 16 textures in a single fragment shader so lets add another texture to our rectangle. We have already created and bound a buffer with the texture co-ordinates so to add another all we need to load the texture, create a uniform, bind the texture and change the fragment shader to use the new texture.

- Load the new texture
  
```cpp
GLuint texture2 = loadBMP_custom("smiley.bmp");
```

- Create the uniform

```cpp
GLuint texture2ID = glGetUniformLocation(shaderID, "texture2Sampler");
```

- Bind the texture

```cpp
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, texture2);
glUniform1i(texture2ID, 1);
```

- Update the fragment shader

```cpp
#version 330 core

// Interpolated values from the vertex shaders
in vec2 uv;

// Output data
out vec3 colour;

// Values that stay constant for the whole mesh
uniform sampler2D texture1Sampler;
uniform sampler2D texture2Sampler;

void main ()
{
    // Output colour = colour of the texture at (u, v)
    colour = mix(texture(texture1Sampler, uv).rgb, 
                 texture(texture2Sampler, uv).rgb, 0.3);
}
```

Here we have used the `mix()` GLSL function to combine the colour samples from the two textures. The last number `0.3` is used to interpolate between the two textures so 30% of the fragment colour comes from the second texture and 70% comes from the first texture.

```{figure} ../images/03_two_textures.png
:width: 500
```

---

## Exercises

1. Apply the smiley face texture to the rectangle so that it displays a 3 by 4 grid of smiley faces.

```{figure} ../images/03_smiley_grid.png
:width: 500
```

2. Modify the fragment shader so that the smiley face looks in the other direction. Hint: the individual elements of a `vec3` vector can be accessed using `<vector name>.x`, `<vector name>.y` etc.

```{figure} ../images/03_smiley_reversed.png
:width: 500
```

3. Apply a texture of your choice to the rectangle (e.g., a selfie). You will need to ensure that the source image is a bitmap so an online converter such as <a href="https://online-converting.com/image/convert2bmp/" target="_blank">https://online-converting.com/image/convert2bmp/</a> are useful for converting from various image formats.

```{figure} ../images/03_kratos_texture.png
:width: 500
```

## Source code

The source code for this lab, including the exercise solutions, can be downloaded using the links below.

- [main.cpp](../code/Lab03_Textures/main.cpp)
- [textureVertexShader.frag](../code/Lab03_Textures/textureVertexShader.vert)
- [textureFragmentShader.frag](../code/Lab03_Textures/textureFragmentShader.frag)
- [Lab03ExFragmentShader.frag](../code/Lab03_Textures/Lab03ExTextureFragmentShader.frag) (fragment shader for the exercise solutions)

---

## Video walkthrough

The video below walks you through these lab materials.

<iframe width="560" height="315" src="https://www.youtube.com/embed/ZSHJsjKycyw?si=Z04BrW1IOjAzZuTB" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>