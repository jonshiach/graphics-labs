#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/maths.hpp>
#include <common/camera.hpp>
#include <common/model.hpp>

// Function prototypes
void keyboardInput(GLFWwindow *window);
void mouseInput(GLFWwindow *window);

// Frame timers
float previousTime = 0.0f;  // time of previous iteration of the loop
float deltaTime    = 0.0f;  // time elapsed since the previous frame

// Create camera object
Camera camera(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f));

int main( void )
{
    // =========================================================================
    // Window creation - you shouldn't need to change this code
    // -------------------------------------------------------------------------
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow(1024, 768, "Lab08 Lighting", NULL, NULL);
    
    if( window == NULL ){
        fprintf(stderr, "Failed to open GLFW window.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    // -------------------------------------------------------------------------
    // End of window creation
    // =========================================================================
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    
    // Use back face culling
    glEnable(GL_CULL_FACE);
    
    // Ensure we can capture keyboard inputs
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Capture mouse inputs
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);
    
    // Compile shader program
    unsigned int shaderID;
    shaderID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");
    
    // Activate shader
    glUseProgram(shaderID);
    
    // Load models
    Model teapot("../assets/teapot.obj");
    
    // Load the textures
    teapot.addTexture("../assets/blue.bmp", "diffuse");
    
//    // Use wireframe rendering (comment out to turn off)
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Define object properties
    float ka = 0.2f;
    float kd = 0.7f;
    
    // Define colours
    glm::vec3 white = glm::vec3(1.0f, 1.0f, 1.0f);
    
    // Define light colours
    glm::vec3 ambientLight  = ka * glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuseLight  = kd * white;
    glm::vec3 lightPosition = glm::vec3(2.0f, 2.0f, 2.0f);
    
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Update timer
        float time   = glfwGetTime();
        deltaTime    = time - previousTime;
        previousTime = time;
        
        // Get inputs
        keyboardInput(window);
        mouseInput(window);
        
        // Clear the window
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calculate view and projection matrices
        camera.target = camera.eye + camera.front;
        camera.calculateMatrices();
        
        // Calculate the model matrix
        glm::mat4 translate;
        glm::mat4 scale;
        glm::mat4 rotate;
        glm::mat4 model = translate * rotate * scale;

        // Calculate the MVP matrix
        glm::mat4 mvp = camera.projection * camera.view * model;
        
        // Send MVP matrix to the vertex shader
        unsigned int mvpID = glGetUniformLocation(shaderID, "mvp");
        glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
        
        // Send MV matrix to the vertex shader
        glm::mat4 mv = camera.view * model;
        unsigned int mvID = glGetUniformLocation(shaderID, "mv");
        glUniformMatrix4fv(mvID, 1, GL_FALSE, &mv[0][0]);
        
        // Send light source properties to the shader
        glUniform3fv(glGetUniformLocation(shaderID, "ambientLight"), 1,
                     &ambientLight[0]);
        glm::vec3 viewLightPos = glm::mat3(camera.view) * lightPosition;
        glUniform3fv(glGetUniformLocation(shaderID, "diffuseLight"), 1,
                     &diffuseLight[0]);
        glUniform3fv(glGetUniformLocation(shaderID, "lightPosition"), 1,
                     &viewLightPos[0]);
        
        // Draw teapot
        teapot.draw(shaderID);
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Cleanup
    teapot.deleteBuffers();
    glDeleteProgram(shaderID);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}

void keyboardInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // Move the camera using WSAD keys
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.eye += 5.0f * deltaTime * camera.front;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.eye -= 5.0f * deltaTime * camera.front;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.eye -= 5.0f * deltaTime * camera.right;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.eye += 5.0f * deltaTime * camera.right;
}

void mouseInput(GLFWwindow *window)
{
    // Get mouse cursor position and reset to centre
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);
    
    // Update yaw and pitch angles
    camera.yaw   += 0.005f * float(xPos - 1024 / 2);
    camera.pitch += 0.005f * float(768 / 2 - yPos);
    
    // Calculate camera vectors from the yaw and pitch angles
    camera.calculateCameraVectors();
}

