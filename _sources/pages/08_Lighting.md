(lighting-section)=

# Lighting

In this lab we will be looking at adding a basic lighting model to our application. Lighting modelling is in itself a huge topic within the field of computer graphics and modern games and movies can look very lifelike thanks to some very clever (and complicated) techniques. Lighting models come in two main types: local illumination and global illumination:

- **local illumination** - the colour and brightness of individual points on a surface are determined by the light emanating from one or more light sources.
- **global illumination** - the colour and brightness of individual points on a surface are determine both by the light emanating from light sources **in addition to** light that is reflected off of other objects in the scene.

Here we will be applying a local illumination model since they are easier to apply than global illumination and quicker to computer. The downside is that they don't produce a rendering as realistic than with global illumination.

Download and build the project files for this lab.

1. Go to <a href="https://github.com/jonshiach/Lab08_Lighting" target="_blank">https://github.com/jonshiach/Lab08_Lighting</a> and follow the instructions to download and build the project files.
2. Open the project file `Lab08_Lighting.sln` (or `Lab08_Lighting.xcodeproj` on macOS) set the **Lab08_Lighting** project as the startup project.
    - Visual Studio: right-click on the **Lab08_Lighting** project and select **Set as Startup Project**.
    - Xcode: Click on the target select dropdown (to the right of the name of the project at the top of the window) and select **Lab08_Lighting** as the target.

3. Build the project by pressing CTRL + B (or ⌘B on Xcode) which should build the project without errors. Run the executable by pressing F5 (or ⌘R on Xcode).

Compile and run the project and you will see the window below showing a wireframe representation of the Utah teapot[^1].

[^1]: The Utah teapot is a standard text model for computer graphics first created in 1975 by Martin Newell whilst at the University of Utah. It has become a bit of an in-joke in the computer graphcis community and has appeared in Pixar's *Toy Story* and in *The Simpsons* episode *Treehouse of Horror VI*.

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

### Creating an .obj file in Blender

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

---

## Phong's lighting model

Phong's lighting model is a local illumination model that simulates the interaction of light falling on surfaces. The colour and brightness of a point on a surface is based on three components

- **ambient reflection** - a simplified model of light that reflects of all objects in a scene
- **diffuse reflection** - describes the direct illumination of a surface by a light source based on the angle between the light source direction and the normal vector to the surface
- **specular reflection** - models the shiny highlights on a surface caused by a light source based on the angle between the light source direction, the normal vector and the view direction

The colour of a pixel on the surface is calculated as a sum of these components, i.e.,

$$ \tt colour  = ambient + diffuse + specular.$$

### Ambient reflection

Ambient reflection is light that is scatters off of all surfaces in a scene. To model this we use a massive cheat, we assume that all surfaces in a scene are lit equally with the same amount of intensity. The equation to do this is

$$ \textsf{ambient} = k_a I_a I_o, $$

where $k_a$ is the **ambient reflection constant** that determines the amount of ambient lighting used and $I_a$ and $I_o$ are the colours of the ambient light and object respectively. Lets create a light source and calculate the ambient lighting. We need to create a vector for the light colour and send it to the shaders using a uniform. Add the following code to the `main.cpp` file just before the render loop.

```cpp
// Define lighting properties
glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 ambientColour = glm::vec3(1.0f, 1.0f, 1.0f);
```

We have used a 3-element vector to containing the RGB values for our light source and ambient colour so in this case it is pure white (a bit boring I know so feel free to experiment with different colours). In the render loop add the following code to send the `lightColour` to the shaders.

```cpp
// Send light source vectors to the shaders
glUniform3fv(glGetUniformLocation(shaderID, "lightColour"), 1, &lightColour[0]);
glUniform3fv(glGetUniformLocation(shaderID, "ambientColour"), 1, &ambientColour[0]);
```

Then edit `fragmentShader.frag` so that it takes in the `lightColour` uniform and calculates the ambient reflection.

```cpp
#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 FragmentPosition;
in vec3 Normal;

// Output data
out vec3 colour;

// Uniforms
uniform sampler2D Texture;
uniform vec3 lightColour;
uniform vec3 ambientColour;

// Constants
float ka = 0.5;

void main ()
{
    // Object colour (not using texture mapping yet)
    //vec3 objectColour = texture(Texture, uv).xyz;
    vec3 objectColour = vec3(0.0f, 0.0f, 1.0f);

    // Ambient
    vec3 ambient = ka * ambientColour * objectColour;
    
    // Fragment colour
    colour = ambient;
}

```

Here we have used an $k_a$ value of 0.5. Changing this value will make the colour of the teapot lighter or darker.

`````{grid}
````{grid-item}
```{figure} ../images/08_ambient_0.2.png
$k_a=0.2$
```
````

````{grid-item}
```{figure} ../images/08_ambient_0.5.png
$k_a=0.5$
```
````

````{grid-item}
```{figure} ../images/08_ambient_0.8.png
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

The vertex shader calculates the position of the fragment in the screen space and since our lighting calculations are done in the view space we need to get our vertex shader to calculate the normal vector and the co-ordinates of the fragment in the view space. Edit `vertexShader.vert` so that is looks like the following.

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

We also need to calculate the view space co-ordinates for the `lightPosition` vector. Since the light position is the same for all fragments it is better do this in our `main.cpp` file rather than recalculating it for each fragment in the shaders.

```cpp
glm::vec3 viewSpaceLightPosition = glm::vec3(view * glm::vec4(lightPosition, 1.0f));
glUniform3fv(glGetUniformLocation(shaderID, "lightPosition"), 1, &viewSpaceLightPosition[0]);
```

In `fragmentShader.frag` add a float constant with the value `kd = 0.7` and in the `main()` function add the following to calculate diffuse reflection.

```cpp
// Diffuse
vec3 normal = normalize(Normal);
vec3 light = normalize(lightPosition - FragmentPosition);
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

In the fragment shader define a specular constant with a value of `ks = 1.0`, a specular exponent with a value of `Ns = 20` and add the following code to the `main()` function to include specular reflection in the fragment colour.

```cpp
// Specular light
vec3 eye = normalize(-FragmentPosition);
vec3 reflection = -light + 2 * dot(light, normal) * normal;
float cosAlpha = max(dot(eye, reflection), 0);
vec3 specular = ks * lightColour * pow(cosAlpha, Ns);
```

Don't forget to add the specular reflection to the fragment colour.

```cpp
// Fragment colour
colour = ambient + diffuse + specular;
```

The result of applying ambient, diffuse and specular reflection is shown in {numref}`teapot-phong-figure`.

```{figure} ../images/08_teapot_phong.png
:width: 500
:name: teapot-phong-figure

Ambient, diffuse and reflection: $k_a = 0.2$, $k_d = 0.7$, $k_s = 1.0$, $N_s = 20$.
```

### Attenuation

**Attenuation** is the gradual decrease in light intensity as the distance between the light source and a surface increases. We can use attenuation to model light from low intensity light source, for example, a candle or torch which will only illuminate an area close to the source. Theoretically attenuation should follow the inverse square law where the light intensity is inversely proportional to the square of the distance between the light source and the surface. However, in practice this tends to result in a scene that is too dark so we calculate attenuation using the following

$$ \textsf{attenuation} = \frac{1}{k_c + k_l \cdot \textsf{distance} + k_q \cdot \textsf{distance}^2}, $$

where $k_c$, $k_l$ and $k_q$ are constant, linear and quadratic constant values. The values of these constants are set to model the type of light source. The graph in {numref}`attenuation-figure` shows a typical attenuation profile where the light intensity rapidly decreases when the distance is small levelling off as the distance gets larger.

```{figure} ../images/08_attenuation.svg
:width: 500
:name: attenuation-figure

Attenuation can be modelled by an inverse quadratic function.
```

To model attenuation edit the fragment shader so that constant values are set to `kc = 1.0`, `kl = 0.1` and `kq = 0.05` and the attenuation is calculated and applied to the the reflection components using the following.

```cpp
// Attenuation
float distance = length(viewSpaceLightPosition - viewSpaceFragPosition);
float attenuation = 1.0 / (kc + kl * distance + kq * distance * distance);

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
lightSource.draw(lightShaderID);
```

Moving the camera to a different position allows us to see the affects of attenuation ({numref}`teapot-attenuation-figure`). Note how the teapots further away from the light source are darker as the light intensity has been reduced.

```{figure} ../images/08_teapot_attenuation.png
:width: 500
:name: teapot-attenuation-figure

The affects of applying attenuation.
```

If you are having difficulty with this check out the source code [Lab08_Phong_lighting.cpp](../code/Lab08_Lighting/Lab08_Phong_lighting.cpp) and the shaders [vertexShader.vert](../code/Lab08_Lighting/vertexShader.vert) and [fragmentShader.frag](../code/Lab08_Lighting/fragmentShader.frag).

### Summary of Phong's reflection model

Having got to this stage we have covered quite a bit and you have been introduced to lots of vectors and constants so it is a good idea to summarise these here

| Name | Type | Description |
|:--|:--|:--|
| $\tt ambient$ | vec3 | Simplified model of the scattering of light in a scene. All surfaces are illuminated equally.|
| $\tt diffuse$ | vec3 | Reflection of light off a rough surface, light is scattered equally in all directions. |
| $\tt specular$ | vec3 | Reflection of light off a smooth surface, light is scatter mainly in the direction of the reflection vector. |
| $\tt attenuation$ | vec3 | Loss of light intensity over distance. |
| $\tt light$ | vec3 | The view space vector from the surface to the light source. |
| $\tt normal$ | vec3 | The view space surface (unit) normal vector. |
| $\tt reflection$ | vec3 | The view space vector that is the reflection of $\tt light$ of the surface. |
| $\tt eye$ | vec3 |The view space vector from the surface to the camera. |
| $\tt cosTheta$ | float | The cosine of the angle between the $\tt light$ and $\tt normal vectors$. |
| $\tt cosAlpha$ | float | The cosine of the angle between the $\tt reflection$ and $\tt eye$ vectors. |
| $\tt ka, kd, ks$ | float | Constants for ambient, diffuse and reflection that determine the material look of the surface. |
| $\tt Ns$ | int | The specular exponent determining the size of the specular highlights |
| $\tt kc, kl, kq$ | float | Constant, linear and quadratic constants used to calculate the attenuation |
| $\tt colour$ | vec3 | Colour of the fragment calculated as the sum of $\tt ambient$, $\tt diffuse$ and $\tt specular$. |


## Multiple light sources

## Directional light
