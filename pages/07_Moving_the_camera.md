(moving-the-camera-section)=

# Moving the camera

In [Lab 6](3D-worlds-section) we saw how we use transformations to build a 3D world, align the world to the camera position and project the view space onto the screen space. We also created a Camera class that performs these calculations, the next step is to modify the Camera class to by able to move and direct the camera around the 3D world.

Download and build the project files for this lab.

1. Go to <a href="https://github.com/jonshiach/Lab07_Moving_the_camera" target="_blank">https://github.com/jonshiach/Lab07_Moving_the_camera</a> and follow the instructions to download and build the project files.
2. Open the project file `Lab07_Moving_the_camera.sln` (or `Lab07_Moving_the_camera.xcodeproj` on macOS) set the **Lab07_Moving_the_camera** project as the startup project.
    - Visual Studio: right-click on the **Lab07_Moving_the_camera** project and select **Set as Startup Project**.
    - Xcode: Click on the target select dropdown (to the right of the name of the project at the top of the window) and select **Lab07_Moving_the_camera** as the target.

3. Build the project by pressing CTRL + B (or ⌘B on Xcode) which should build the project without errors. Run the executable by pressing F5 (or ⌘R on Xcode).

Compile and run the project and you will see the multiple cube example we had at the end of the last lab.

## Using keyboard input to move the camera

The first thing we are going to do is to get keyboard input from the user and use it to move the camera. If you take a look at `main.cpp` we have used the function `glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE)` which captures keyboard inputs for the `window` object and the do/while render loop checks for whether the escape key has been pressed to terminate the application. We are going to do similar for other keys.

We are going to modify the `calculateMatrices()` method from the Camera class so that it can take inputs from the keyboard. To do this edit the `camera.hpp` file so that the method declaration takes in an input of a GLFW window object.

```cpp
void calculateMatrices(GLFWwindow* window);
```

Of course don't forget make a similar change the method in the `Camera.cpp` file. If you attempt to compile the program a compiler error will be thrown up because we also need to pass the `window` object into the method, so in the `main.cpp` file make this change

```cpp
// Get the view and projection matrices from the camera library
camera.calculateMatrices(window);
```

Now that the `calculateMatrices()` method can take in keyboard inputs we need to capture the key presses and make appropriate changes to the `position` attribute. Define three Camera class vectors `front`, `right` and `up` in the `camera.hpp` file

```cpp
 glm::vec3 front;
 glm::vec3 right;
 glm::vec3 up;
 ```

and instantiate them in the class constructor in `camera.cpp`.

```cpp
front = glm::vec3(0.0f, 0.0f, -1.0f);
right = glm::vec3(1.0f, 0.0f, 0.0f);
up = glm::vec3(0.0f, 1.0f, 0.0f);
```

Here we have defined the defaults so the camera is pointing down the $z$-axis. To move the camera we simply need to add these vectors to the `position` vector, for example to move the camera forward and back we add or subtract the `front` vector.

```{figure} ../images/07_camera_movement.svg
:width: 500
```

No we can add the following code to the Camera class constructor before the `view` matrix is calculated.

```cpp
// Keyboard inputs
if (glfwGetKey(window, GLFW_KEY_W))
{
    position += front; // move forward
}
if (glfwGetKey(window, GLFW_KEY_S))
{
    position -= front; // move backwards
}
if (glfwGetKey(window, GLFW_KEY_A))
{
    position -= right; // move left
}
if (glfwGetKey(window, GLFW_KEY_D))
{
    position += right; // move right
}
```

The `glfwGetKey(window, GLFW_KEY_W)` returns a true value if the W key has been pressed. An if statement is then used to add the `front` vector to the camera `position` vector thereby moving the camera forward. We've done similar to move the camera backwards, to the left and right using the classic WSAD key combinations.

Since we now have the ability to change the `positon` vector we should also update the `target` vector so that the camera direction does change (we do this below using mouse inputs). Just before we calculate the `view` matrix add the following code.

```cpp
// Update target
target = position + front;
```

Compile and run the program and you should be able to move the camera around the 3D world using the keyboard. 

<center>
<video controls muted="true" loop="true" width="500">
    <source src="../_static/07_keyboard_1.mp4" type="video/mp4">
</video>
</center>

### Camera movement speed

So it works but the camera movement is too quick resulting in controls are too sensitive. To overcome this issue we can multiply the camera direction vector by number which we can choose to adjust the speed of the camera. Edit `camera.hpp` to add a `speed` attribute to the Camera class

```cpp
// Speed attributes
float speed = 5.0f;
```

Here we have specified that we want our camera to move at a speed of 5 units per second. Speed is distance divided time so we also need to know how much time has elapsed between the rendering of each frame. This is easily done using the `glfwGetTime()` function we have used to [animate objects](animating-objects-section). In the `main.cpp` before the `main()` function add the following declarations

```cpp
// Timers
float currentTime = 0.0f;
float lastTime = 0.0f;
float deltaTime = 0.0f;
```

Here we have declared the variables `currentTime` and `lastTime` which will be used to record the times that the current and previous frames are rendered and `deltaTime` which is difference between these two.

At the beginning of the render loop at the following code to update these timers.

```cpp
// Update timers
currentTime = glfwGetTime();
deltaTime = currentTime - lastTime;
lastTime = currentTime;
```

We need to pass `deltaTime` to the `calculateMatrices()` Camera class method so change the `camera.hpp` and `camera.cpp` files so that the method declaration look like the following.

```cpp
calculateMatrices(GLFWwindow* window, float deltaTime)
```

Also don't forget to change the method call in `main.cpp`. We can now apply the `speed` attribute to the movement calculations, for example

```cpp
if (glfwGetKey(window, GLFW_KEY_W))
{
  position += front * deltaTime * speed;
}
```

The result of making these changes means that the camera now moves at 5 units per second (since $\tt front$ and $\tt right$ are [unit vectors](unit-vectors-section)) is shown below.

<center>
<video controls muted="true" loop="true" width="500">
    <source src="../_static/07_keyboard_2.mp4" type="video/mp4">
</video>
</center>

## Using the mouse to point the camera

We can now move the camera position using keyboard inputs but we can't yet change the direction of camera, to do this we can use the mouse. First we need to tell GLFW to capture the mouse inputs so add the following code after where we told it to capture the keyboard inputs in the `main.cpp` file.

```cpp
// Tell GLFW to capture our mouse
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
glfwPollEvents();
glfwSetCursorPos(window, 1024/2, 768/2);
```

The `glfwSetInputMode()` and `glfwPollEvents()` functions tell GLFW to expect mouse input and also disable the cursor so it isn't shown on the window. The `gflwSetCursorPos()` initialises the cursor to be in the center of the window using the width and height window size.

We now need to change the Camera class method `calculateMatrices()` to get the mouse cursor positions. In the `camera.cpp` add the following code just before we update the `target` vector.

```cpp
// Get mouse cursor position
double xPos, yPos;
glfwGetCursorPos(window, &xPos, &yPos);
glfwSetCursorPos(window, 1024/2, 768/2);
```

Here we declare two doubles (a floating point number that uses 8 bytes instead of 4 used by the float type) `xPos` and `yPos` and use the function `glfwGetCursorPos()` to get the cursor position before resetting it back to the centre of the window using `glfwSetCursorPos()` (if we didn't do this the cursor could go outside of the window and we would be able to use it anymore).

### Pitch and yaw

The values of `xPos` and `yPos` are the number of pixels across and up the window from the bottom-left hand corner pixel respectively. The inputs to the `glm::lookAt()` function require the camera `front` vector so we need a way of converting from the position of the cursor to a vector. To do this we first calculate the yaw and pitch angles. Consider {numref}`roll-pitch-yaw-figure` which shows $\tt front$, $\tt right$ and $\tt up$ camera vectors.

```{figure} /images/07_euler_angles.svg
:width: 350
:name: roll-pitch-yaw-figure

Yaw, pitch and roll
```

If the user moves the mouse up or down on the window then we want the camera to angle up or down to reflect this, this is the $\tt pitch$ angle. Likewise when the user moves the mouse left or right we want the camera to angle left or right, this is the $\tt yaw$ angle. Add two attributes `pitch` and `yaw` to the Camera class by adding the following code to `camera.hpp`

```cpp
// Camera angles
float pi = 3.1415927f;
float pitch = pi;
float yaw = pi;
```

Both `pitch` and `yaw` are initialised to $\pi$ (which is the equivalent to 180$^\circ$) since the camera is looking down the $z$-axis. We want these angles to update for each frame so in the `calculateMatrices()` Camera class method add the following code after `xPos` and `yPos` have been determined.

```cpp
// Update yaw and pitch angles
yaw += float(xPos - 1024/2);
pitch += float(yPos - 768/2);
```

The $\tt yaw$ and $\tt pitch$ angles increase when the cursor is moved to the right and above the window centre respectively. We now have $\tt yaw$ and $\tt pitch$ angles and need to convert to camera vectors. Using spherical to Cartesian conversion the $\tt front$ vector is

$$ \begin{align*}
  \tt front &= \textsf{normalise}(\sin(\tt yaw) \cos(\tt pitch), \sin(\tt pitch), -\cos(\tt yaw) \cos(\tt pitch)).
\end{align*} $$

You don't necessarily need to know where this comes from but if you are curious click on the dropdown below.

````{dropdown} Calculating the front vector from the yaw and pitch angles

First consider the left-to-right movement, since we use `xPos - 1024/2` to calculate the $\tt yaw$ angle, if we move the cursor to the right then the $\tt yaw$ angle increases and we want to rotate **clockwise** about the $y$-axis ({numref}`yaw-figure`). 

```{figure} /images/03_yaw.svg
:name: yaw-figure
:width: 250

Yaw movement is rotation about the $y$-axis.
```

The [rotation matrix](rotation-section) is (note the negative sign for the $\sin$ function is switched to the bottom left)

$$ \begin{align*}
  R_y &=
  \begin{pmatrix}
    \cos(\tt yaw) & 0 & \sin(\tt yaw) \\
    0 & 1 & 0 \\
    -\sin(\tt yaw) & 0 & \cos(\tt yaw)
  \end{pmatrix}
\end{align*} $$

Similarly, if we move the cursor up then $\tt pitch$ angle increases and we want to rotate **anti-clockwise** about the $x$-axis ({numref}`pitch-figure`).

```{figure} /images/03_pitch.svg
:name: pitch-figure
:width: 200

Pitch movement is rotation about the $x$-axis.
```

The rotation matrix is

$$ \begin{align*}
  R_x &= 
  \begin{pmatrix}
    1 & 0 & 0 \\
    0 & \cos(\tt pitch) & \sin(\tt pitch) \\
    0 & -\sin(\tt pitch) & \cos(\tt pitch)
  \end{pmatrix}
\end{align*} $$

Applying these rotations to the $\tt front$ vector (0,0,-1)

$$ \begin{align*}
  \tt newFront &= (0, 0, -1)
  \begin{pmatrix}
    1 & 0 & 0 \\
    0 & \cos(\tt pitch) & \sin(\tt pitch) \\
    0 & -\sin(\tt pitch) & \cos(\tt pitch)
  \end{pmatrix} 
  \begin{pmatrix}
    \cos(\tt yaw) & 0 & \sin(\tt yaw) \\
    0 & 1 & 0 \\
    -\sin(\tt yaw) & 0 & \cos(\tt yaw)
  \end{pmatrix}\\
  &=
  (0, 0, -1)
  \begin{pmatrix}
    \cos(\tt yaw) & 0 & 0 \sin(\tt yaw) \\
    -\sin(\tt yaw)\sin(\tt pitch) & \cos(\tt pitch) & \cos(\tt yaw)\sin(\tt pitch) \\
    -\sin(\tt yaw)\cos(\tt pitch)  & -\sin(\tt pitch) & \cos(\tt yaw)\cos(\tt pitch)
  \end{pmatrix} \\
  &= (\sin(\tt yaw)\cos(\tt pitch), sin(\tt pitch), -\cos(\tt yaw)\cos(\tt pitch))
\end{align*} $$

which gives the updated $\tt front$ vector from the $\tt yaw$ and $\tt pitch$ angles.
````

The $\tt right$ and $\tt up$ camera vectors are calculated in a similar was to how we calculated the `view` matrix in [Lab 6](view-matrix-section), i.e.,

$$ \begin{align*}
  \tt right &= \textsf{normalise}(\tt front \times worldUp), \\
  \tt up &= \textsf{normalise}(\tt right \times front),
\end{align*} $$

and the $\tt target$ vector is

$$ \tt target = position + front. $$

Add the following code to the `camera.cpp` file after we have updated the $\tt yaw$ and $\tt pitch$ angles.

```cpp
// Update camera vectors
front = glm::normalize(glm::vec3(cos(pitch) * sin(yaw) , sin(pitch), -cos(yaw) * cos(pitch)));
right = glm::normalize(glm::cross(front, worldUp));
up = glm::normalize(glm::cross(right, front));
target = position + front;
```

If you compile and run your program you may notice that the mouse controls are far too sensitive and we need to slow down the speed of rotation. To do this add an attribute to the Camera class in `camera.hpp` called `mouseSpeed` and initialise it to some small number (you may need to experiment with this value).

```cpp
float mouseSpeed = 0.005f;
```

Then change the `yaw` and `pitch` calculations in `camera.cpp` to the following

```cpp
// Update yaw and pitch angles
yaw += mouseSpeed * float(xPos - 1024/2);
pitch += mouseSpeed * float(yPos - 768/2);
```

Running the program now gives a much better result.

<center>
<video controls muted="true" loop="true" width="500">
    <source src="../_static/07_mouse.mp4" type="video/mp4">
</video>
</center>

### Constraining the pitch angle

Whilst playing with the camera direction you may have noticed that when you attempt to look straight up or down upon the objects the camera suddenly flips. This is because of how we calculate the `view` vector. To prevent this we need to constrain the `pitch` angle so it is between $\frac{1}{2}\pi$ and $\frac{3}{2} \pi$ (equivalent to 90$^\circ$ and 270$^\circ$ respectively). To do this we simply add the following if statements after updating the `yaw` and `pitch` angles.

```cpp
// Constrain pitch angle so the screen doesn't flip when looking straight up or down
if (pitch > 1.49f * pi)
    pitch = 1.49f * pi;
if (pitch < 0.51f * pi)
    pitch = 0.51f * pi;
```

## Back face culling

Whilst moving your camera around your 3D world you may notice that we can move through objects and view them from the inside. All surfaces of the cubes are rendered, including those not visible from the camera because they are on the far side. This is a waste of resources as OpenGL is calculating the vertex and fragment shaders for objects that won't be shown in the frame. To overcome this we can cull (omit) any surface of an object that is **back facing** the camera in a method called **back face culling**.

(normal-vector-section)=

A **normal vector** (often just referred to as a **normal**) is a vector that is perpendicular to a surface at a given point ({numref}`normal-vector-figure`).

```{figure} /images/07_normal_vector.svg
:width: 250
:name: normal-vector-figure

The surface normal vector.
```

Since in computer graphics are surfaces are triangles, we can easily calculate a normal vector using a [cross product](cross-product-section). If a triangle has vertices $\tt v0$, $\tt v1$ and $\tt v2$ then the normal vector can be calculated using

$$ \tt normal = (v1 - v0) \times (v2 - v1). $$

A surface is said to be back facing it its normal vector is pointing away from the camera position. If we only render the front facing surfaces then, assuming the surfaces are opaque, we should not notice any difference and we have halved the number of surfaces the shader has to deal with ({numref}`backface-culling-figure`). 

```{figure} /images/07_backface_culling.svg
:width: 300
:name: backface-culling-figure

Back face culling removes surfaces with vectors pointing away from the camera.
```

But how do we know if a surface is back facing? Consider {numref}`back-facing-figure` which shows a back facing surface.

```{figure} /images/07_Back_facing.svg
:width: 400
:name: back-facing-figure

A back facing surface.
```

Recall that the [dot product](dot-product-section) is related to the angle between two vectors, i.e.,

$$ \tt normal \cdot \text{viewVector} = | normal | | \text{viewVector} | \cos(\theta). $$

If we have a back facing surface then $\theta$ is less than 90$^\circ$ and $\cos(\theta)$ is a positive number so

$$\begin{align*}
  \tt normal \cdot \text{viewVector} > 0.
\end{align*}$$

So to apply back face culling the vertex shader just has to calculate the dot product between the $\tt normal$ and $\tt viewVector$ vectors and if it is a positive number it ignores the surface from then on. To apply back face culling in OpenGL all we need to do is add the following code to the `main.cpp` near where we invoke the depth testing.

```cpp
// Use back face culling
glEnable(GL_CULL_FACE);
```

Compile and run your program and use the keyboard and mouse to put the camera inside a cube. You will now see that the back faces haven't been rendered.

---

## Exercises

1. Change the `calculateMatrices()` Camera class method so that the camera position always has a $y$ co-ordinate of 0, i.e., like a first person shooter game where the player cannot fly around the world.
2. Add the ability for the user to perform a jump by pressing the space bar. The jump should last for 1 second and the camera should follow a smooth arc. Hint: the function $y = \tt height \cdot \sin(\pi \cdot \tt time)$ produces values of $y=0$ when $\tt time = 0$ or $\tt time = 1$ and $y = \tt height$ when $\tt time = 0.5$.
3. Write your own class called `MyLib` with static member functions for each of the functions you have used from the glm library (e.g., `lookAt()`) and make use of them to calculate the `model`, `view` and `projection` matrices (you may make use of `glm::mat4` and `glm::vec3` types).

---

## Source code

The source code for this lab, including the exercise solutions, can be downloaded using the links below.

- [main.cpp](../code/Lab07_Moving_the_camera/main.cpp)
- [camera.hpp](../code/Lab07_Moving_the_camera/camera.hpp)
- [camera.cpp](../code/Lab07_Moving_the_camera/camera.cpp)
- [camera_exercises.hpp](../code/Lab07_Moving_the_camera/camera_exercises.hpp) (solution to exercise 1 and 2)
- [camera_exercises.cpp](../code/Lab07_Moving_the_camera/camera_exercises.cpp)

Note the solution to exercise 3 has been omitted since it is a requirement for the higher mark band on the coursework assignment.

## Video walkthrough

The video below walks you through these lab materials.

<iframe width="560" height="315" src="https://www.youtube.com/embed/JJUgf9GCN_4?si=0MDnzyWHUxB0R22f" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>