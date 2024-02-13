# Moving the camera

In [Lab 6](3D-worlds-section) we saw how we use transformations to build a 3D world, align the world to the camera position and project the view space onto the screen space. We also created a Camera class that performs these calculations, the next step is to modify the Camera class to by able to move and direct the camera around the 3D world.

Download and build the project files for this lab.

1. Go to <a href="https://github.com/jonshiach/Lab07_Moving_the_camera" target="_blank">https://github.com/jonshiach/Lab07_Moving_the_camera</a> and follow the instructions to download and build the project files.
2. Open the project file `Lab07_Moving_the_camera.sln` (or `Lab07_Moving_the_camera.xcodeproj` on macOS) set the **Lab07_Moving_the_camera** project as the startup project.
    - Visual Studio: right-click on the **Lab07_Moving_the_camera** project and select **Set as Startup Project**.
    - Xcode: Click on the target select dropdown (to the right of the name of the project at the top of the window) and select **Lab07_Moving_the_camera** as the target.

3. Build the project by pressing CTRL + B (or ⌘B on Xcode) which should build the project without errors. Run the executable by pressing F5 (or ⌘R on Xcode).

Compile and run the project and you will see the multiple cube examples we did at the end of the last lab. 

## Using keyboard input to move the camera

The first thing we are going to do is to get keyboard input from the user and use it to move the camera. If you take a look at `main.cpp` we have used the function `glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE)` which captures keyboard inputs for the `window` object and the do/while render loop checks for whether the escape key has been pressed to terminate the application. We are going to do similar for other keys. 

We are going to modify the `calculateMatrices()` method from the Camera class so that it can take inputs from the keyboard. To do this edit the `camera.hpp` file so that the method declaration looks like the following

```cpp
void calculateMatrices(GLFWwindow* window);
```

and of course don't forget make a similar change the method in the `Camera.cpp` file. If you attempt to compile the program a compiler error will be thrown up because we also need to pass the `window` object into the method, so in the `main.cpp` file make this change

```cpp
// Get the view and projection matrices from the camera library
camera.calculateMatrices(window);
```

Now that the `calculateMatrices()` method can take in keyboard inputs we need to capture the key presses and make appropriate changes to the `position` attribute. In the `Camera.cpp` file, add the following code before the `view` is calculated.

```cpp
// Keyboard inputs
if (glfwGetKey(window, GLFW_KEY_W))
{
  position += front;
}
if (glfwGetKey(window, GLFW_KEY_S))
{
  position -= front;
}
if (glfwGetKey(window, GLFW_KEY_A))
{
  position -= right;
}
if (glfwGetKey(window, GLFW_KEY_D))
{
  position += right;
}
```

The `glfwGetKey(window, GLFW_KEY_W)` returns a true value if the W key has been pressed. An if statement is then used to add the `front` vector to the camera `position` vector thereby moving the camera forward. We've done similar to move the camera backwards, to the left and right using the classic WSAD key combinations.

Compile and run the program and you should be able to move the camera around the 3D world using the keyboard. 

<center>
<video controls muted="true" loop="true" width="500">
    <source src="../07_keyboard_1.mp4" type="video/mp4">
</video>
</center>

### Camera movement speed

So it works but the camera movement is too quick meaning that the controls are too sensitive. To overcome this issue we can multiply the camera direction vector by number which we can choose to adjust the speed of the camera. Edit `camera.hpp` to add a `speed` attribute to the Camera class

```cpp
// Speed attributes
float speed = 5.0f;
```

Here we have specified that we want our camera to move at a speed of 5 units per second. Speed is distance divided time so we also need to know how much time has elapsed between the rendering of each frame. This is easily done using the `glfwGetTime()` function we have used to animate objects. In the `main.cpp` before the `main()` function add the following declarations

```cpp
// Timers
float currentFrame = 0.0f;
float lastFrame = 0.0f;
float deltaTime = 0.0f;
```

Here we have declared the variables `currentFrame` and `lastFrame` which will be used to record the times that the current and previous frames are rendered and `deltaTime` which is difference between these two.

At the beginning of the render loop at the following code to update these timers.

```cpp
// Update timers
currentFrame = glfwGetTime();
deltaTime = currentFrame - lastFrame;
lastFrame = currentFrame;
```

We need to pass `deltaTime` to the `calculateMatrices()` Camera class method so change the `camera.hpp` and `camera.cpp` files so that the method declaration look like the following.

```cpp
calculateMatrices(GLFWwindow* window, float deltaTime)
```

Also don't forget to change the method call in `main.cpp`. We can now apply the `speed` attribute to the movement calculates, for example

```cpp
if (glfwGetKey(window, GLFW_KEY_W))
{
  position += front * deltaTime * speed;
}
```

The result of making these changes is shown below.

<center>
<video controls muted="true" loop="true" width="500">
    <source src="../07_keyboard_2.mp4" type="video/mp4">
</video>
</center>

### Mouse input