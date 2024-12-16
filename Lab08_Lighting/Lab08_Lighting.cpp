#include <iostream>
#include <cmath>

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

// Light struct
struct Light
{
    glm::vec3 position;
    glm::vec3 colour;
    glm::vec3 direction;
    float constant;
    float linear;
    float quadratic;
    float cosPhi;
    unsigned int type;
};

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
    unsigned int shaderID, lightShaderID;
//    shaderID      = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");
    shaderID      = LoadShaders("vertexShader.glsl", "multipleLightsFragmentShader.glsl");
    lightShaderID = LoadShaders("lightVertexShader.glsl", "lightFragmentShader.glsl");
    
    // Activate shader
    glUseProgram(shaderID);
    
    // Load models
    Model teapot("../assets/teapot.obj");
    Model sphere("../assets/sphere.obj");
    
    // Load the textures
    teapot.addTexture("../assets/blue.bmp", "diffuse");
    
//    // Use wireframe rendering (comment out to turn off)
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Define teapot object lighting properties
    teapot.ka = 0.2f;
    teapot.kd = 0.7f;
    teapot.ks = 1.0f;
    teapot.Ns = 20.0f;
    
    // Define light source properties
    glm::vec3 lightPosition = glm::vec3(2.0f, 2.0f, 2.0f);
    glm::vec3 lightColour   = glm::vec3(1.0f, 1.0f, 1.0f);
    float constant  = 1.0f;
    float linear    = 0.1f;
    float quadratic = 0.02f;
    
    // Create vector of light sources
    std::vector<Light> lightSources;
    
    // Add first point light source
    Light light;
    light.position  = glm::vec3(2.0f, 2.0f, 2.0f);
    light.colour    = glm::vec3(1.0f, 1.0f, 1.0f);
    light.constant  = 1.0f;
    light.linear    = 0.1f;
    light.quadratic = 0.02f;
    light.type      = 1;
    lightSources.push_back(light);
    
    // Add second point light source
    light.position  = glm::vec3(1.0f, 1.0f, -8.0f);
    light.colour    = glm::vec3(1.0f, 1.0f, 1.0f);
    light.constant  = 1.0f;
    light.linear    = 0.1f;
    light.quadratic = 0.02f;
    light.type      = 1;
    lightSources.push_back(light);
    
    // Add spotlight
    light.position  = glm::vec3(0.0f, 3.0f, 0.0f);
    light.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    light.colour    = glm::vec3(1.0f, 1.0f, 0.0f);
    light.constant  = 1.0f;
    light.linear    = 0.1f;
    light.quadratic = 0.02f;
    light.cosPhi    = std::cos(Maths::radians(45.0f));
    light.type      = 2;
    lightSources.push_back(light);
    
    // Add directional light
    light.direction = glm::vec3(1.0f, -1.0f, 0.0f);
    light.colour    = glm::vec3(1.0f, 0.0f, 0.0f);
    light.type      = 3;
    lightSources.push_back(light);
    
    // Teapot positions
    glm::vec3 teapotPositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -10.0f),
        glm::vec3(-3.0f, -2.0f, -3.0f),
        glm::vec3(-4.0f, -2.0f, -8.0f),
        glm::vec3( 2.0f,  2.0f, -6.0f),
        glm::vec3(-4.0f,  3.0f, -8.0f),
        glm::vec3( 0.0f, -2.0f, -5.0f),
        glm::vec3( 4.0f,  2.0f, -4.0f),
        glm::vec3( 2.0f,  0.0f, -2.0f),
        glm::vec3(-1.0f,  1.0f, -2.0f)
    };

    // Teapot rotation angles
    float teapotAngles[10];
    for (unsigned int i = 0 ; i < 10 ; i++)
        teapotAngles[i] = Maths::radians(20.0f * i);
    
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
        
        // Activate shader
        glUseProgram(shaderID);
        
        // Calculate view and projection matrices
        camera.target = camera.eye + camera.front;
        camera.calculateMatrices();

        // Calculate the model matrix
        glm::mat4 translate;
        glm::mat4 scale;
        glm::mat4 rotate;
        glm::mat4 model = translate * rotate * scale;

        // Calculate the MVP matrix
        glm::mat4 MVP = camera.projection * camera.view * model;
        
        // Send MVP matrix to the vertex shader
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
        
        // Send MV matrix to the vertex shader
        glm::mat4 MV = camera.view * model;
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "MV"), 1, GL_FALSE, &MV[0][0]);
        
//        // Send light source properties to the shader
//        glUniform1f (glGetUniformLocation(shaderID, "ka"), teapot.ka);
//        glUniform1f (glGetUniformLocation(shaderID, "kd"), teapot.kd);
//        glUniform3fv(glGetUniformLocation(shaderID, "lightColour"), 1, &lightColour[0]);
//        glm::vec3 viewSpaceLightPosition = glm::vec3(camera.view * glm::vec4(lightPosition, 1.0f));
//        glUniform3fv(glGetUniformLocation(shaderID, "lightPosition"), 1, &viewSpaceLightPosition[0]);
//        glUniform1f (glGetUniformLocation(shaderID, "ks"), teapot.ks);
//        glUniform1f (glGetUniformLocation(shaderID, "Ns"), teapot.Ns);
//        glUniform1f (glGetUniformLocation(shaderID, "constant"), constant);
//        glUniform1f (glGetUniformLocation(shaderID, "linear"), linear);
//        glUniform1f (glGetUniformLocation(shaderID, "quadratic"), quadratic);
        
        // Send multiple light source properties to the shader
        for (unsigned int i = 0; i < static_cast<unsigned int>(lightSources.size()); i++)
        {
            glm::vec3 viewSpaceLightPosition = glm::vec3(camera.view * glm::vec4(lightSources[i].position, 1.0f));
            glm::vec3 viewSpaceLightDirection = glm::vec3(camera.view * glm::vec4(lightSources[i].direction, 0.0f));
            std::string idx = std::to_string(i);
            glUniform3fv(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].colour").c_str()), 1, &lightSources[i].colour[0]);
            glUniform3fv(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].position").c_str()), 1, &viewSpaceLightPosition[0]);
            glUniform1f(glGetUniformLocation (shaderID, ("lightSources[" + idx + "].constant").c_str()), lightSources[i].constant);
            glUniform1f(glGetUniformLocation (shaderID, ("lightSources[" + idx + "].linear").c_str()), lightSources[i].linear);
            glUniform1f(glGetUniformLocation (shaderID, ("lightSources[" + idx + "].quadratic").c_str()), lightSources[i].quadratic);
            glUniform1i(glGetUniformLocation (shaderID, ("lightSources[" + idx + "].type").c_str()), lightSources[i].type);
            glUniform3fv(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].direction").c_str()), 1, &viewSpaceLightDirection[0]);
            glUniform1f(glGetUniformLocation (shaderID, ("lightSources[" + idx + "].cosPhi").c_str()), lightSources[i].cosPhi);
        }

        // Send object lighting properties to the fragment shader
        glUniform1f(glGetUniformLocation(shaderID, "ka"), teapot.ka);
        glUniform1f(glGetUniformLocation(shaderID, "kd"), teapot.kd);
        glUniform1f(glGetUniformLocation(shaderID, "ks"), teapot.ks);
        glUniform1f(glGetUniformLocation(shaderID, "Ns"), teapot.Ns);
        
        // Draw teapot
//        teapot.draw(shaderID);
        
        // Loop through objects
        for (unsigned int i = 0; i < 10; i++)
        {
            // Calculate model matrix
            glm::mat4 translate = Maths::translate(teapotPositions[i]);
            glm::mat4 scale     = Maths::scale(glm::vec3(0.75f));
            glm::mat4 rotate    = Maths::rotate(teapotAngles[i], glm::vec3(1.0f));
            glm::mat4 model     = translate * rotate * scale;
            
            // Send the MVP and MV matrices to the vertex shader
            glm::mat4 MV  = camera.view * model;
            glm::mat4 MVP = camera.projection * MV;
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "MV"), 1, GL_FALSE, &MV[0][0]);
            
            // Draw the model
            teapot.draw(shaderID);
        }
        
        // ---------------------------------------------------------------------
        // Draw light sources
        // Activate light source shader
        glUseProgram(lightShaderID);
//        
//        // Calculate model matrix
//        glm::mat4 translate = Maths::translate(lightPosition);
//        glm::mat4 scale     = Maths::scale(glm::vec3(0.1f));
//        glm::mat4 model     = translate * scale;
//
//        // Send the MVP and MV matrices to the vertex shader
//        glm::mat4 MVP = camera.projection * camera.view * model;
//        glUniformMatrix4fv(glGetUniformLocation(lightShaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
//
//        // Send model, view, projection matrices and light colour to light shader
//        glUniform3fv(glGetUniformLocation(lightShaderID, "lightColour"), 1, &lightColour[0]);
//
//        // Draw light source
//        sphere.draw(lightShaderID);
        
        for (unsigned int i = 0; i < static_cast<unsigned int>(lightSources.size()); i++)
        {
            // Calculate model matrix
            glm::mat4 translate = Maths::translate(lightSources[i].position);
            glm::mat4 scale     = Maths::scale(glm::vec3(0.1f));
            glm::mat4 model     = translate * scale;

            // Send the MVP and MV matrices to the vertex shader
            glm::mat4 MVP = camera.projection * camera.view * model;
            glUniformMatrix4fv(glGetUniformLocation(lightShaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);

            // Send model, view, projection matrices and light colour to light shader
            glUniform3fv(glGetUniformLocation(lightShaderID, "lightColour"), 1, &lightSources[i].colour[0]);

            // Draw light source
            sphere.draw(lightShaderID);
        }
        // ---------------------------------------------------------------------
        
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

