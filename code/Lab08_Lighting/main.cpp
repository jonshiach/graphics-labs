#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/io.hpp>

#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"

// Create camera object
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));

// Light structs
struct Light
{
    glm::vec3 position;
    glm::vec3 colour;
    float constant, linear, quadratic;
};

struct DirLight
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

// Timers
float currentTime = 0.0f;
float lastTime = 0.0f;
float deltaTime = 0.0f;

int main( void )
{
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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow( 1024, 768, "Lighting", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.");
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

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Use back face culling
    glEnable(GL_CULL_FACE);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Compile shader programs
    GLuint shaderID = LoadShaders("vertexShader.vert", "multipleLightsFragmentShader.frag");
    GLuint lightShaderID = LoadShaders("lightVertexShader.vert", "lightFragmentShader.frag");

    // Load models
    Model teapot("../objects/teapot.obj");
    Model sphere("../objects/sphere.obj");
    
    // Load texture
    teapot.addTexture("../objects/blue.bmp", "diffuse");
    
    // Use wireframe (comment out to turn off)
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
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
    light.position = glm::vec3(1.0f, 1.0f, -8.0f);
    light.constant = 1.0f;
    light.linear = 0.1f;
    light.quadratic = 0.02f;
    lights.push_back(light);
    
    // Define directional light
    DirLight dirLight;
    dirLight.direction = glm::vec3(1.0f, -1.0f, 0.0f);
    dirLight.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    dirLight.diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
    dirLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    
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
    
    do {
        // Update timers
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Background colour
        glm::vec3 bgColour = glm::vec3(0.0f, 0.0f, 0.0f);
        glClearColor(bgColour[0], bgColour[1], bgColour[2], 1.0f);
        
        // Clear the window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Get the view and projection matrices from the camera library
        camera.calculateMatrices(window, deltaTime);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        // Activate shader
        glUseProgram(shaderID);
        
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
        
        // Send directional light to the shader
        glm::vec3 lightDirection = glm::mat3(view) * dirLight.direction;
        glUniform3fv(glGetUniformLocation(shaderID, "dirLight.direction"), 1, &lightDirection[0]);
        glUniform3fv(glGetUniformLocation(shaderID, "dirLight.ambient"), 1, &dirLight.ambient[0]);
        glUniform3fv(glGetUniformLocation(shaderID, "dirLight.diffuse"), 1, &dirLight.diffuse[0]);
        glUniform3fv(glGetUniformLocation(shaderID, "dirLight.specular"), 1, &dirLight.specular[0]);
        
        // Send material (object) properties to the shader
        glUniform1f(glGetUniformLocation(shaderID, "ka"), 0.2f);
        glUniform1f(glGetUniformLocation(shaderID, "kd"), 0.7f);
        glUniform1f(glGetUniformLocation(shaderID, "ks"), 1.0f);
        glUniform1f(glGetUniformLocation(shaderID, "Ns"), 20.0f);
        
        // Send view and projection matrices to the shaders
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, &projection[0][0]);

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

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
