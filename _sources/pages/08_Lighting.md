(lighting-section)=

# Lighting

In this lab we will be looking at adding a basic lighting model to our application. Lighting modelling is in itself a huge topic within the field of computer graphics and modern games and movies can look very lifelike thanks to some very clever (and complicated) techniques. Lighting models come in two main types: local illumination and global illumination:

- **local illumination** - the colour and brightness of individual points on a surface are determined by the light emanating from one or more light sources.
- **global illumination** - the colour and brightness of individual points on a surface are determine both by the light emanating from light sources **in addition to** light that is reflected off of other objects in the scene.

```{figure} ../images/08_local_global_illumination.svg
:width: 500
```

Here we will be applying a local illumination model since they are easier to apply than global illumination and quicker to computer. The downside is that they don't produce a rendering as realistic than with global illumination.

Download and build the project files for this lab.

1. Go to <a href="https://github.com/jonshiach/Lab08_Lighting" target="_blank">https://github.com/jonshiach/Lab08_Lighting</a> and follow the instructions to download and build the project files.
2. Open the project file `Lab08_Lighting.sln` (or `Lab08_Lighting.xcodeproj` on macOS) set the **Lab08_Lighting** project as the startup project.
    - Visual Studio: right-click on the **Lab08_Lighting** project and select **Set as Startup Project**.
    - Xcode: Click on the target select dropdown (to the right of the name of the project at the top of the window) and select **Lab08_Lighting** as the target.

3. Build the project by pressing CTRL + B (or ⌘B on Xcode) which should build the project without errors. Run the executable by pressing F5 (or ⌘R on Xcode).

Compile and run the project and you will see the window below showing a wireframe representation of the Utah teapot[^1].

[^1]: The Utah teapot is a standard test model for computer graphics first created in 1975 by Martin Newell whilst at the University of Utah. It has become a bit of an in-joke in the computer graphics community and has appeared in Pixar's *Toy Story* and in *The Simpsons* episode *Treehouse of Horror VI*.

```{figure} ../images/08_teapot_wireframe.png
:width: 500
```

The teapot has been rendered as a wireframe model since, in the absence of light and shadow, we wouldn't be able to tell that it was in fact a 3D model. We can turn of the wireframe rendering by commenting out the line `glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);`. Do this and you should see the following.

```{figure} ../images/08_teapot_solid.png
:width: 500
```

## The Model class

If you take a look at the source code in the `Lab08_Lighting/source` folder you will notice that in addition to the classes introduced in previous labs (Texture, Shader and Camera) we have an addition class called Model which is defined in the `model.hpp` and `model.cpp` files. The Model class has been written so that we can load the vertex and texture co-ordinates from external files rather than having to define these in our code. Take a look at the `main.cpp` file where the following Model class methods have been called:

- `Model teapot("../objects/teapot.obj)` - this is the constructor for the Model class and creates an object called `teapot`, loads the vertex co-ordinates, texture co-ordinates and vertex normals from an .obj file (see below) and creates the VAO and relevant buffers.
- `teapot.addTexture("../objects/crate", "diffuse");` - this method loads the crate texture (not used here but shown to give you an example).
- `teapot.draw(shaderID)` - this method binds the objects buffers and instructs OpenGL to draw the model.
- `teapt.deleteBuffers()` - this method deletes all of the buffers created by the constructor.

### Wavefront (.obj) files

The Model class includes a private member function called `loadObj()` written by contributors of <a href = "https://www.opengl-tutorial.org" target="_blank">opengl-tutorial.org</a> which loads in a <a href="https://en.wikipedia.org/wiki/Wavefront_.obj_file" target="_blank">**wavefront (.obj)**</a> file. A wavefront file is one of the many different types of file that is used to describe 3D models in computer graphics. In the `Lab08_Lighting/objects/` folder you will see some `.obj` files. Open the `cube.obj` file using a text editor and you will see the following.

```text
# Blender 4.0.2
# www.blender.org
mtllib cube.mtl
o Cube
v 1.000000 1.000000 -1.000000
v 1.000000 -1.000000 -1.000000
v 1.000000 1.000000 1.000000
v 1.000000 -1.000000 1.000000
v -1.000000 1.000000 -1.000000
v -1.000000 -1.000000 -1.000000
v -1.000000 1.000000 1.000000
v -1.000000 -1.000000 1.000000
vn -0.0000 1.0000 -0.0000
vn -0.0000 -0.0000 1.0000
vn -1.0000 -0.0000 -0.0000
vn -0.0000 -1.0000 -0.0000
vn 1.0000 -0.0000 -0.0000
vn -0.0000 -0.0000 -1.0000
vt 0.875000 0.500000
vt 0.625000 0.750000
vt 0.625000 0.500000
vt 0.375000 0.990255
vt 0.375000 0.750000
vt 0.625000 0.008121
vt 0.375000 0.250000
vt 0.375000 0.008121
vt 0.375000 0.500000
vt 0.125000 0.750000
vt 0.125000 0.500000
vt 0.625000 0.250000
vt 0.875000 0.750000
vt 0.625000 0.988631
s 0
usemtl Material
f 5/1/1 3/2/1 1/3/1
f 3/2/2 8/4/2 4/5/2
f 7/6/3 6/7/3 8/8/3
f 2/9/4 8/10/4 6/11/4
f 1/3/5 4/5/5 2/9/5
f 5/12/6 2/9/6 6/7/6
f 5/1/1 7/13/1 3/2/1
f 3/2/2 7/14/2 8/4/2
f 7/6/3 5/12/3 6/7/3
f 2/9/4 4/5/4 8/10/4
f 1/3/5 3/2/5 4/5/5
f 5/12/6 1/3/6 2/9/6
```

The vertex and face data is given in lines with the following abbreviations:

- `v` - the (x, y, z) co-ordinates of a vertex
- `vn` - the (nx, ny, nz) normal vector for the vertex
- `vt` - the (u, v) texture co-ordinates
- `f` - indices of the vertices of a face. Each face is defined by 3 vertices so we have 3 sets of 3 values. The face vertices are of the form `v/vt/vn` so `3/2/1` refers to a vertex where the co-ordinates are given by the 3rd `v` line, the texture co-ordinates are given by the 2nd `vt` line and the normal vector is given by the 1st `vn` line.

```{note}
The `loadObj()` private member function in the Model class is quite simplistic and we need to make sure our .obj file is in the correct form. There are some model loading libraries available such as <a href="http://www.assimp.org" target="_blank">assimp</a> (open ASSet IMPorter library) that can handle most common object formats but use of this requires compiling source code and configuring the IDE which is a bit too fiddly for what we are doing here.
```

---

## Phong's lighting model

Phong's lighting model is a local illumination model that simulates the interaction of light falling on surfaces. The colour and brightness of a point on a surface is based on three components

- **ambient reflection** - a simplified model of light that reflects of all objects in a scene
- **diffuse reflection** - describes the direct illumination of a surface by a light source based on the angle between the light source direction and the normal vector to the surface
- **specular reflection** - models the shiny highlights on a surface caused by a light source based on the angle between the light source direction, the normal vector and the view direction

The colour of a pixel on the surface is calculated as a sum of these components, i.e.,

$$ \tt colour  = ambient + diffuse + specular.$$

### Ambient reflection

Ambient reflection is light that is scatters off of all surfaces in a scene. To model this we use a massive cheat, we assume that all surfaces of the object are lit equally with the same amount of intensity. The equation to do this is

$$ \textsf{ambient} = k_a I_a I_o, $$

where $k_a$ is the **ambient reflection constant** that determines the amount of ambient lighting used and $I_a$ and $I_o$ are the colours of the ambient light and object respectively. Lets create a light source and calculate the ambient lighting. We need to create a vector for the light colour and send it to the shaders using a uniform. Add the following code to the `main.cpp` file just before the render loop.

```cpp
// Define lighting properties
glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
```

We have used a 3-element vector to containing the RGB values for our light source so in this case it is pure white (a bit boring I know so feel free to experiment with different colours). In the render loop add the following code to send the `lightColour` to the shaders.

```cpp
// Send light source properties to the shader
glUniform3fv(glGetUniformLocation(shaderID, "lightColour"), 1, &lightColour[0]);
```

Then edit `fragmentShader.frag` so that it takes in the `lightColour` uniform and calculates the ambient reflection.

```cpp
#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Output data
out vec3 fragmentColour;

// Uniforms
uniform sampler2D diffuse1;
uniform vec3 lightColour;

// Constants
float ka = 0.2;

void main ()
{
    // Object colour
    vec3 objectColour = vec3(texture(diffuse1, UV));

    // Ambient reflection
    vec3 ambient = ka * lightColour * objectColour;
    
    // Fragment colour
    fragmentColour = ambient;
}
```

Here we have used an $k_a$ value of 0.5. Changing this value will make the colour of the teapot lighter or darker.

`````{grid}
````{grid-item}
```{figure} ../images/08_teapot_ambient_0.2.png
$k_a=0.2$
```
````

````{grid-item}
```{figure} ../images/08_teapot_ambient_0.5.png
$k_a=0.5$
```
````

````{grid-item}
```{figure} ../images/08_teapot_ambient_0.8.png
$k_a=0.8$
```
````

`````

### Diffuse reflection

When a light ray hits a surface it is reflected in a direction such that the angle between the reflected ray and the surface normal is the same as the angle between the light ray and the surface normal (known as the angle of incidence). Consider {numref}`diffuse-reflection-figure` that shows parallel light rays hitting a rough surface. The normal vectors vary across the surface so the light rays are scattered in multiple directions. This is known as **diffuse reflection**.

```{figure} ../images/08_diffuse_reflection.svg
:width: 500
:name: diffuse-reflection-figure

Light rays hitting a rough surface are scattered in all directions.
```

To model diffuse reflection we assume that light is reflected equally in all directions ({numref}`diffuse-figure`).

```{figure} ../images/08_diffuse.svg
:width: 400
:name: diffuse-figure

Diffuse reflection scatters light equally in all directions.
```

The amount of light that is reflected to the viewer is modelled using the angle $\theta$ between the $\tt light$ and $\tt normal$ vectors. If $\theta$ is small then the light source is directly in front of the surface so most of the light will be reflected to the viewer. Whereas if $\theta$ is close to 90$^\circ$ then the light source is nearly in line with the surface and little of the light will be reflected to the viewer. We model this using the cosine of $\theta$ since $\cos(0) = 1$ and $\cos(90)=0$. Diffuse reflection is calculated using

$$ \textsf{diffuse} = k_d I_o I_d \cos(\theta),$$

where $k_d$ is the **diffuse reflection constant** that determines the amount of diffuse lighting seen by the viewer and $I_d$ is the colour of the diffuse light source. Recall that the angle between two vectors is related by [dot product](dot-product-section) so if the $\tt light$ and $\tt normal$ vectors are unit vectors then $\cos(\theta) = \tt light \cdot normal$. If $\theta > 90^\circ$ then light source is behind the surface and no light should be reflected to the viewer. When $\theta$ is between 90$^\circ$ and 180$^\circ$, $\cos(\theta)$ is negative so limit the value of $\cos(\theta )$ between 0 and 1.

Lets define position for a light source, add the following to `main.cpp` just after where we defined the `lightColour` vector

```cpp
glm::vec3 lightPosition = glm::vec3(2.0f, 2.0f, 2.0f);
```

All calculations performed in the fragment shader is done in the view space so we need to transform `lightPosition` to the view space. We could do this in the vertex shader but since the light position is the same for all fragments it is better do this in the `main.cpp` file rather than recalculating it for each fragment in the shaders. Add the following code just after we sent `lightColour` to the shader.

```cpp
glm::vec3 viewSpaceLightPosition = glm::vec3(view * glm::vec4(lightPosition, 1.0f));
glUniform3fv(glGetUniformLocation(shaderID, "lightPosition"), 1, &viewSpaceLightPosition[0]);
```

The vertex shader calculates the position of the fragment in the screen space and since our lighting calculations are done in the view space we need to get our vertex shader to calculate the co-ordinates of the fragment in the view space and the normal vector in the view space. For the fragment co-ordinates this is simply a matter of multiplying `position` by the `view` and `model` matrices but for the normal vector we use

$$ \begin{align*}
    \texttt{viewSpaceNormal} = ((\tt view \cdot model)^{-1})^\mathsf{T} \cdot \tt normal.
\end{align*} $$

Recall that $A^\mathsf{T}$ is the [transpose](transpose-section) and $A^{-1}$ is the [inverse](inverse-matrix-section) of the matrix $A$. You don't need to know why we use this equation but if you are curious click on the download link below.

````{dropdown} Derivation of the view space normal transformation

Consider the diagram in {numref}`view-space-normal-1-figure` that shows the normal and tangent vectors to an edge in the object space. If the combined model and view transformations preserves the scaling of the edge such the equal scaling is used in the $x$, $y$ and $z$ axes then the normal and tangent vectors are perpendicular in the view space.

```{figure} ../images/08_view_space_normal_1.svg
:width: 200
:name: view-space-normal-1-figure

Normal and tangent vectors in the object space.
```

If the model and view transformations do not preserve the scaling then the the view space normal vector is no longer perpendicular to the tangent vector ({numref}`view-space-normal-2-figure`).

```{figure} ../images/08_view_space_normal_2.svg
:width: 300
:name: view-space-normal-2-figure

Normal and tangent vectors in the view space.
```

Let $\tt M$ be the first 3 rows and columns of the $\tt model \cdot view$ matrix then $\tt viewSpaceTangent = M * tangent$ (here I've used $\tt *$ to denote column major matrix multiplication so that it is consistent with our code). We need to derive a $3\times 3$ transformation matrix $\tt A$ such that the view space normal and tangents vectors are perpendicular, i.e.,

$$\tt (A * normal) \cdot (M * tangent) = 0.$$

We can replace the dot product by a matrix multiplication by transposing $\tt(A * normal)$

$$\tt (A * normal)^\mathsf{T} * (M * tangent )= 0.$$

A property of matrix multiplication is that the transpose of a multiplication is equal to the multiplication of the transposes so we can write this as

$$\tt normal^\textsf{T} * A^\mathsf{T} * M * tangent = 0.$$

If $\tt A^\mathsf{T} * M = I$ then $\tt viewSpaceNormal \cdot viewSpaceTangent = 0$ so are perpendicular. We need $\tt A$ so rearranging gives

$$ \begin{align*}
    \tt A^\mathsf{T} * M &= \tt I \\
    \tt A^\mathsf{T} &= \tt M^{-1} \\
    \tt A &= \tt (M^{-1})^\mathsf{T}.
\end{align*} $$

Since $\tt M = view * model$ then the matrix that transforms a model space normal vector to the view space is $\tt A = ((view * model)^{-1})^\mathsf{T}$

````

Edit `vertexShader.vert` so that is looks like the following.

```cpp
#version 330 core

// Input vertex data
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

// Output data
out vec2 UV;
out vec3 FragmentPosition;
out vec3 Normal;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Output vertex postion
    gl_Position = projection * view * model * vec4(position, 1.0);
    
    // Output (u,v) co-ordinates
    UV = uv;
    
    // Output view space fragment position and normal
    FragmentPosition = vec3(view * model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * normal;
}
```

In `fragmentShader.frag` add a `vec3` uniform for the `lightPosition` and add a float constant with the value `kd = 0.7`. In the `main()` function add the following to calculate diffuse reflection.

```cpp
// Diffuse reflection
vec3 normal = normalize(Normal);
vec3 light = normalize(lightPosition - fragmentPosition);
float cosTheta = max(dot(normal, light), 0);
vec3 diffuse = kd * objectColour * lightColour * cosTheta;
```

Finally add the diffuse reflection to the ambient reflection to calculate the fragment colour.

```cpp
// Fragment colour
colour = ambient + diffuse;
```

The result of applying ambient and diffuse reflection is shown in {numref}`teapot-diffuse-figure`.

```{figure} ../images/08_teapot_diffuse.png
:width: 500
:name: teapot-diffuse-figure

Ambient and diffuse reflection: $k_a = 0.2$, $k_d = 0.7$.
```

Use the keyboard and mouse to view the teapot from different angles. You should notice that the side of the teapot facing away from the light source is darker.

### Specular reflection

Consider {numref}`specular-reflection-figure` that shows parallel light rays hitting a smooth surface. The normal vectors will be similar across the surface so the reflected rays will point mostly in the same directions. This is known as **specular reflection**.

```{figure} ../images/08_specular_reflection.svg
:width: 500
:name: specular-reflection-figure

Light rays hitting a smooth surface are reflected in the same direction.
```

For a perfectly smooth surface the reflected ray will point in the direction of the $\tt reflection$ vector so in order to see the light the viewer would need to be positioned in the direction of the $\tt reflection$ vector. Since most surfaces are not perfectly smooth we add a bit of scattering to the model the amount of specular reflection seen by the viewer. This is determined by the angle $\alpha$ between the $\tt reflection$ vector and the $\tt eye$ vector, the vector pointing from the surface to the viewer. The closer the viewer is to the reflection vector, the smaller the value of $\alpha$ will be and the more of the reflected light will be seen.

```{figure} ../images/08_specular.svg
:width: 400
:name: specular-figure

Specular reflection scatters light mainly towards the reflection vector.
```

We model the scattering of the reflected light rays using $\cos(\alpha)$ raised to an integer power

$$ \textsf{specular} = k_s I_p \cos(\alpha)^{N_s},$$

where $N_s$ is the **specular exponent** that determines the size of the specular highlights. The angle $\alpha$ is calculated using a dot product between the $\tt eye$ and $\tt reflection$ vectors. The $\tt eye$ vector is the vector from the fragment position to the camera which is at (0,0,0) and the reflection vector is calculated using

$$\tt reflection = - light + 2 (light \cdot normal) \, normal.$$

In the fragment shader define a specular constant with a value of `ks = 1.0`, a specular exponent with a value of `Ns = 20.0` and add the following code to the `main()` function to include specular reflection in the fragment colour.

```cpp
// Specular reflection
vec3 eye = normalize(-fragmentPosition);
vec3 reflection = -light + 2 * dot(light, normal) * normal;
float cosAlpha = max(dot(eye, reflection), 0);
vec3 specular = ks * lightColour * pow(cosAlpha, Ns);
```

Don't forget to add the specular reflection to the fragment colour.

```cpp
// Fragment colour
colour = ambient + diffuse + specular;
```

The result of applying ambient, diffuse and specular reflection is shown in {numref}`teapot-specular-figure`.

```{figure} ../images/08_teapot_specular.png
:width: 500
:name: teapot-specular-figure

Ambient, diffuse and reflection: $k_a = 0.2$, $k_d = 0.7$, $k_s = 1.0$, $N_s = 20$.
```

### Attenuation

**Attenuation** is the gradual decrease in light intensity as the distance between the light source and a surface increases. We can use attenuation to model light from low intensity light source, for example, a candle or torch which will only illuminate an area close to the source. Theoretically attenuation should follow the inverse square law where the light intensity is inversely proportional to the square of the distance between the light source and the surface. However, in practice this tends to result in a scene that is too dark so we calculate attenuation using the following

$$ \textsf{attenuation} = \frac{1}{c + l \cdot \textsf{distance} + q \cdot \textsf{distance}^2}, $$

where $c$, $l$ and $q$ are constant, linear and quadratic constant values. The values of these constants are set to model the type of light source. The graph in {numref}`attenuation-figure` shows a typical attenuation profile where the light intensity rapidly decreases when the distance is small levelling off as the distance gets larger.

```{figure} ../images/08_attenuation.svg
:width: 500
:name: attenuation-figure

Attenuation can be modelled by an inverse quadratic function.
```

To model attenuation edit the fragment shader so that constant values are set to `constant = 1.0`, `linear = 0.1` and `quadratic = 0.02` and the attenuation is calculated and applied to the the reflection components using the following.

```cpp
// Attenuation
float distance = length(lightPosition - fragmentPosition);
float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);

// Fragment colour
colour = (ambient + diffuse + specular) * attenuation;
```

To demonstrate the affects of applying attenuation we are going to need some more objects that are further away from the light source. In your `main.cpp` file before the render loop define an array of position vectors

```cpp
// Specify world space object positions
glm::vec3 positions[] = {
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

and then replace the `model` matrix and drawing command with the following (it should be fairly obvious what we are doing here).

```cpp
// Loop through objects
for (unsigned int i = 0; i < 10; i++)
{
    // Calculate model matrix
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), positions[i]);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), 30.0f * i, glm::vec3(1.0f));
    glm::mat4 model = translate * rotate * scale;
    
    // Send the model matrix to the shader
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, &model[0][0]);
    
    // Draw the model
    teapot.draw(shaderID);
}
```

It would also be useful to render the light source. After you've drawn the teapots add the following code (a light source object and the shader for drawing the light source have been defined beforehand).

```cpp
// Draw light source
// Active light source shader
glUseProgram(lightShaderID);

// Calculate model matrix
glm::mat4 translate = glm::translate(glm::mat4(1.0f), lightPosition);
glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
glm::mat4 model = translate * scale;

// Send model, view, projection matrices and light colour to light shader
glUniformMatrix4fv(glGetUniformLocation(lightShaderID, "model"), 1, GL_FALSE, &model[0][0]);
glUniformMatrix4fv(glGetUniformLocation(lightShaderID, "view"), 1, GL_FALSE, &view[0][0]);
glUniformMatrix4fv(glGetUniformLocation(lightShaderID, "projection"), 1, GL_FALSE, &projection[0][0]);
glUniform3fv(glGetUniformLocation(lightShaderID, "lightColour"), 1, &lightColour[0]);

// Draw light source
sphere.draw(lightShaderID);
```

Moving the camera to a different position allows us to see the affects of attenuation ({numref}`teapot-attenuation-figure`). Note how the teapots further away from the light source are darker as the light intensity has been reduced.

```{figure} ../images/08_teapot_attenuation.png
:width: 500
:name: teapot-attenuation-figure

The affects of applying attenuation.
```

If you are having difficulty with this check out the source code [Lab08_single_light.cpp](../code/Lab08_Lighting/Lab08_single_light.cpp) and the shaders [vertexShader.vert](../code/Lab08_Lighting/vertexShader.vert) and [fragmentShader.frag](../code/Lab08_Lighting/fragmentShader.frag).


## Multiple light sources

In theory to add multiple light sources to a scene is simply a matter of calculating the ambient, diffuse and specular reflection for each light source and the fragment colour is the sum of all of reflections from all of the light sources. We have seen for a single light source we have to define the colour, the position in the world space and the three constant, linear and quadratic attenuation constants. Given that we would like to do this for multiple light sources we need data structure for each light source.

A data structure in C++ and GLSL is defined in the same way using the <a href="https://cplusplus.com/doc/tutorial/structures/" target="_blank">struct</a> declaration.

```cpp
// Structs
struct Light
{
    vec3 colour;
    vec3 position;
    float linear, constant, quadratic;
}
```

This defines a data structure called `Light` that contains the information required to calculate the lighting model for a single light source. We are going to pass this light information for multiple light sources using a uniform with the following code.

```cpp
#define maxLights 100;
uniform Light lights[maxLights];
uniform int numLights;
```

This defines a 100 element array of `Light` structs (hopefully we will not need more than 100 light sources but if you then simply increase the number of `maxLights`) and the actual number of lights we have is passed in using the `numLights` uniform. Then all we need to do is loop through each of the light sources, calculate the ambient, diffuse and specular reflection and add it to the fragment colour. The fragment shader for multiple light sources is given below. Create a new file called `multipleLightsFragmentShader.frag` in the `source/` directory and paste this code.

```cpp
#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 fragmentPosition;
in vec3 Normal;

// Output data
out vec3 fragmentColour;

// Structs
struct Light
{
    vec3 colour;
    vec3 position;
    float linear, constant, quadratic;
};

// Uniforms
#define maxLights 100
uniform sampler2D diffuse1;
uniform Light lights[maxLights];
uniform int numLights;

uniform float ka;
uniform float kd;
uniform float ks;
uniform float Ns;

void main ()
{
    // Object colour, normal and eye vector (these are the same for all light sources)
    vec3 objectColour = vec3(texture(diffuse1, UV));
    vec3 normal = normalize(Normal);
    vec3 eye = normalize(-fragmentPosition);
    
    // Loop through light sources
    fragmentColour = vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < numLights; i++)
    {
        // Ambient reflection
        vec3 ambient = ka * lights[i].colour * objectColour;
        
        // Diffuse reflection
        vec3 light = normalize(lights[i].position - fragmentPosition);
        float cosTheta = max(dot(normal, light), 0);
        vec3 diffuse = kd * objectColour * lights[i].colour * cosTheta;
        
        // Specular reflection
        vec3 reflection = -light + 2 * dot(light, normal) * normal;
        float cosAlpha = max(dot(eye, reflection), 0);
        vec3 specular = ks * lights[i].colour * pow(cosAlpha, Ns);
        
        // Attenuation
        float distance = length(lights[i].position - fragmentPosition);
        float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * distance * distance);
        
        // Add current light source to the fragment colour
        fragmentColour += (ambient + diffuse + specular) * attenuation;
    }
}
```

We also need to make changes to the `main.cpp` file. Add the `Light` data structure before the `main()` function declaration

```cpp
// Light structs
struct Light
{
    glm::vec3 position;
    glm::vec3 colour;
    float constant, linear, quadratic;
};
```

Since we are using a different file for the fragment shader we need to tell OpenGL to use our new fragment shader which it compiles the shader program.

```cpp
// Compile shader programs
GLuint shaderID = LoadShaders("vertexShader.vert", "multipleLightsFragmentShader.frag");
```

We want to define the lighting properties for multiple lights sources so we are going to store our `Light` structures in a <a href="https://en.cppreference.com/w/cpp/container/vector" target="_blank">vector</a>. Where we defined `lightColour` and `lightPosition` for our single light example replace the code with the following.

```cpp
// Create vector of Light structs
std::vector<Light> lights;

// First light source
Light light;
light.colour = glm::vec3(1.0f, 1.0f, 1.0f);
light.position = glm::vec3(2.0f, 2.0f, 2.0f);
light.constant = 1.0f;
light.linear = 0.1f;
light.quadratic = 0.02f;
lights.push_back(light);

// Second light source
light.colour = glm::vec3(1.0f, 1.0f, 1.0f);
light.position = glm::vec3(2.0f, 2.0f, -8.0f);
light.constant = 1.0f;
light.linear = 0.1f;
light.quadratic = 0.02f;
lights.push_back(light);
```

This code creates two light sources, defines the values of the data structures and stores then in the vector `lights`. The `lights.push_back(light)` adds the current `light` to the end of the `lights` vector. Now we need to send the light and material values to the shader using uniforms, replace the existing code with the code below.

```cpp
// Send light source properties to the shader
glUniform1i(glGetUniformLocation(shaderID, "numLights"), static_cast<unsigned int>(lights.size()));
for (unsigned int i = 0; i < lights.size(); i++)
{
    std::string number = std::to_string(i);
    glUniform3fv(glGetUniformLocation(shaderID, ("lights[" + number + "].colour").c_str()), 1, &lights[i].colour[0]);
    glm::vec3 viewSpaceLightPosition = glm::vec3(view * glm::vec4(lights[i].position, 1.0f));
    glUniform3fv(glGetUniformLocation(shaderID, ("lights[" + number + "].position").c_str()), 1, &viewSpaceLightPosition[0]);
    glUniform1f(glGetUniformLocation(shaderID, ("lights[" + number + "].constant").c_str()), lights[i].constant);
    glUniform1f(glGetUniformLocation(shaderID, ("lights[" + number + "].linear").c_str()), lights[i].linear);
    glUniform1f(glGetUniformLocation(shaderID, ("lights[" + number + "].quadratic").c_str()), lights[i].quadratic);
}

// Send material (object) properties to the shader
glUniform1f(glGetUniformLocation(shaderID, "ka"), 0.2f);
glUniform1f(glGetUniformLocation(shaderID, "kd"), 0.7f);
glUniform1f(glGetUniformLocation(shaderID, "ks"), 1.0f);
glUniform1f(glGetUniformLocation(shaderID, "Ns"), 20.0f);
```

Here we simply loop through the `lights` vector and send the values for each individual light to the shader (unfortunately we can't send a vector of structs using a uniform, we could use <a href="https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)" target="_blank">GLSL interface blocks</a> but I wanted to keep things simple here).

Finally to draw each light source replace the code to draw the single one with the following.

```cpp
// Draw light sources
glUseProgram(lightShaderID);
for (unsigned int i = 0; i < lights.size(); i++)
{
    // Calculate model matrix
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), lights[i].position);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
    glm::mat4 model = translate * scale;
    
    // Send model, view, projection matrices and light colour to light shader
    glUniformMatrix4fv(glGetUniformLocation(lightShaderID, "model"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(lightShaderID, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(lightShaderID, "projection"), 1, GL_FALSE, &projection[0][0]);
    glUniform3fv(glGetUniformLocation(lightShaderID, "lightColour"), 1, &lights[i].colour[0]);
    
    // Draw light source
    sphere.draw(lightShaderID);
}
```

Hopefully once you've made all of the changes it compiles and runs to show the following

```{figure} ../images/08_teapot_multiple_lights.png
:width: 500
:name: multiple-lights-figure

Teapots lit using 2 light sources.
```

Use the keyboard and mouse to move the camera around the teapots to see the affects of the light sources.

## Directional light

When modelling a light source that is far away, e.g., example the sun, the light rays are parallel. It also does not matter where the object is or where the camera is in relation to the light source. Therefore we are only concerned in the direction of the light.

The lighting calculations are the same as for a point light source seen above with the exception that we do not apply the attenuation. We will use another struct for the directional light source (we will assume we only have one directional light source)

```cpp
struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
```

and import the corresponding uniform. Here our `DirLight` struct contains the light direction and 3 other 3-element vectors for the ambient, diffuse and specular colours. These replace the `ka`, `kd` and `ks` used in the point light source to give us more control over the look of our scene (you can also do this for the point light source if you want). 

 To keep our code organised and readable we should refactor the code and make use of functions. The following function is used to calculate directional lighting

```cpp
vec3 directionalLight(DirLight dirLight, vec3 normal, vec3 eye, vec3 objectColour)
{
    // Ambient reflection
    vec3 ambient = dirLight.ambient * objectColour;
    
    // Diffuse refection
    vec3 light = normalize(-dirLight.direction);
    float cosTheta = max(dot(normal, light), 0);
    vec3 diffuse = dirLight.diffuse * objectColour * cosTheta;
    
    // Specular light
    vec3 reflection = -light + 2 * dot(light, normal) * normal;
    float cosAlpha = max(dot(eye, reflection), 0);
    vec3 specular = dirLight.specular * objectColour * pow(cosAlpha, Ns);
    
    // Return fragment colour
    return ambient + diffuse + specular;
}
```

Doing something similar for the point light source means our `main()` function is now much simpler.

```cpp
void main ()
{
    // Object colour, normal and eye vector (these are the same for all light sources)
    vec3 objectColour = vec3(texture(diffuse1, UV));
    vec3 normal = normalize(Normal);
    vec3 eye = normalize(-fragmentPosition);
    
    // Calculate directional light
    fragmentColour = directionalLight(dirLight, normal, eye, objectColour);
    
    // Loop through the point light sources
    for (int i = 0; i < numLights; i++)
    {
        fragmentColour += pointLight(lights[i], fragmentPosition, normal, eye, objectColour);
    }
}
```

The full code for the fragment shader is shown below. Paste this into your `multipleLightsFragmentShader.frag` file.

```cpp
#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 fragmentPosition;
in vec3 Normal;

// Output data
out vec3 fragmentColour;

// Structs
struct Light
{
    vec3 colour;
    vec3 position;
    float linear, constant, quadratic;
};

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Uniforms
#define maxLights 100
uniform sampler2D diffuse1;
uniform Light lights[maxLights];
uniform int numLights;
uniform DirLight dirLight;

uniform float ka;
uniform float kd;
uniform float ks;
uniform float Ns;

// Function prototypes
vec3 directionalLight(DirLight dirLight, vec3 normal, vec3 eye, vec3 objectColour);
vec3 pointLight(Light ptLight, vec3 fragmentPosition, vec3 normal, vec3 eye, vec3 objectColour);

void main ()
{
    // Object colour, normal and eye vector (these are the same for all light sources)
    vec3 objectColour = vec3(texture(diffuse1, UV));
    vec3 normal = normalize(Normal);
    vec3 eye = normalize(-fragmentPosition);
    
    // Calculate directional light
    fragmentColour = directionalLight(dirLight, normal, eye, objectColour);
    
    // Loop through the point light sources
    for (int i = 0; i < numLights; i++)
    {
        fragmentColour += pointLight(lights[i], fragmentPosition, normal, eye, objectColour);
    }
}

// Calculate directional lighting
vec3 directionalLight(DirLight dirLight, vec3 normal, vec3 eye, vec3 objectColour)
{
    // Ambient reflection
    vec3 ambient = dirLight.ambient * objectColour;
    
    // Diffuse refection
    vec3 light = normalize(-dirLight.direction);
    float cosTheta = max(dot(normal, light), 0);
    vec3 diffuse = dirLight.diffuse * objectColour * cosTheta;
    
    // Specular light
    vec3 reflection = -light + 2 * dot(light, normal) * normal;
    float cosAlpha = max(dot(eye, reflection), 0);
    vec3 specular = dirLight.specular * pow(cosAlpha, Ns);
    
    // Return fragment colour
    return ambient + diffuse + specular;
}

// Calculate point light
vec3 pointLight(Light ptLight, vec3 fragmentPosition, vec3 normal, vec3 eye, vec3 objectColour)
{
    // Ambient reflection
    vec3 ambient = ka * ptLight.colour * objectColour;
    
    // Diffuse reflection
    vec3 light = normalize(ptLight.position - fragmentPosition);
    float cosTheta = max(dot(normal, light), 0);
    vec3 diffuse = kd * objectColour * ptLight.colour * cosTheta;
    
    // Specular reflection
    vec3 reflection = -light + 2 * dot(light, normal) * normal;
    float cosAlpha = max(dot(eye, reflection), 0);
    vec3 specular = ks * ptLight.colour * pow(cosAlpha, Ns);
    
    // Attenuation
    float distance = length(ptLight.position - fragmentPosition);
    float attenuation = 1.0 / (ptLight.constant + ptLight.linear * distance + ptLight.quadratic * distance * distance);
    
    // Return fragment colour
    return (ambient + diffuse + specular) * attenuation;
}
```

Now we need to define a directional light source in the `main.cpp` file and send to to the shader. First declare a struct for the directional light where we declared the `Light` struct. 

```cpp
struct DirLight
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
```

Then define the values for the directional light

```cpp
// Define directional light
DirLight dirLight;
dirLight.direction = glm::vec3(1.0f, -1.0f, 0.0f);
dirLight.ambient = glm::vec3(0.2f, 0.2f, 0.0f);
dirLight.diffuse = glm::vec3(0.7f, 0.7f, 0.0f);
dirLight.specular = glm::vec3(1.0f, 1.0f, 0.0f);
```

Here we define a directional light source with rays coming down from the top left as we look down the $z$-axis. Colour of the light source is yellow (i.e., equal red and green mixed with zero blue) and the ambient, diffuse and specular colours have been scaled similarly to the point light sources above. The directional light values are sent to the shader in the same was as per the point light sources (although we only have one of these). Note that the `direction` vector is defined in the world space so we need to multiply it by `view` before sending it to the shader.

```cpp
// Send directional light to the shader
glm::vec3 lightDirection = glm::mat3(view) * dirLight.direction;
glUniform3fv(glGetUniformLocation(shaderID, "dirLight.direction"), 1, &dirLight.direction[0]);
glUniform3fv(glGetUniformLocation(shaderID, "dirLight.ambient"), 1, &dirLight.ambient[0]);
glUniform3fv(glGetUniformLocation(shaderID, "dirLight.diffuse"), 1, &dirLight.diffuse[0]);
glUniform3fv(glGetUniformLocation(shaderID, "dirLight.specular"), 1, &dirLight.specular[0]);
```

Make these changes and run the code and you should see something similar to this.

```{figure} ../images/08_teapot_directional_light.png
:width: 500
:name: directional-light-figure

Directional and point light sources.
```

Note that we can see that the teapots have been illuminated from a directional light source from the left hand side in addition to the two point light sources. Since the directional light source colour was yellow our blue teapots take on a slightly green appearance.

---
## Exercises

1. Experiment with the positions, colours and material properties of the various light sources to see what effects they have.
2. Change the colour of the second point light source to magenta and rotate its position in a circle around the teapots.

<center>
<video controls muted="true" loop="true" width="500">
    <source src="../08_exercise_2.mp4" type="video/mp4">
</video>
</center>

3. The planet Narkov has a red sun and a single day lasts for 5 of our seconds. Use directional lighting to model the illumination of the sun as it passes through the sky and also beneath the horizon (fortunately Narkovians like tea so using our teapots would not seem unusual). The background colour can also be changed to match the colour of the light source.

<center>
<video controls muted="true" loop="true" width="500">
    <source src="../08_exercise_3.mp4" type="video/mp4">
</video>
</center>

--- 
## Source code

The source code for this lab can be downloaded below

- [Lab08_single_light.cpp](../code/Lab08_Lighting/Lab08_single_light.cpp)
- [vertexShader.vert](../code/Lab08_Lighting/vertexShader.vert)
- [fragmentShader.frag](../code/Lab08_Lighting/fragmentShader.frag)
- [main.cpp](../code/Lab08_Lighting/main.cpp) - multiple point light sources and directional light
- [multipleLightsFragmentShader.frag](../code/Lab08_Lighting/multipleLightsFragmentShader.frag)

---
## Creating an .obj file in Blender

To create an .obj file we can use the popular open source application <a href="https://www.blender.org" target="_blank">Blender</a> (this is installed on the machines in the Dalton building).

1. Create your object in blender and sort out the material textures, UV co-ordinates etc. (lots of tutorials on youtube to help you with this)
2. Click on **File > Export > Wavefront (.obj)**

```{figure} ../images/08_blender_export_obj_1.png
:width: 600
```

3. Make sure **Include Normals**, **Include UVs** and **Triangular Faces** are selected.


```{figure} ../images/08_blender_export_obj_2.png
:width: 600
```

4. Navigate to your chosen folder e.g., `Lab08_Lighting/objects/`, and give it an appropriate name.
