#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "texture.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/io.hpp>

#include "MyLib.hpp"

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
    GLFWwindow* window = glfwCreateWindow( 1024, 768, "Transformations", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
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

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Dark grey background
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    
    // Compile shader program
    GLuint shaderID = LoadShaders("vertexShader.vert", "fragmentShader.frag");
    
    // Create OpenGL textures
    GLuint texture1 = loadBMP_custom("smiley.bmp");
    
    // Get a handle for the texture sampler uniforms
    GLuint texture1ID = glGetUniformLocation(shaderID, "texture1Sampler");
    
    // Create the Vertex Array Object (VAO)
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // Define vertex positions
    static const GLfloat vertices[] = {
        -0.2f, -0.2f, 0.0f,    // triangle 1
         0.2f, -0.2f, 0.0f,
         0.2f,  0.2f, 0.0f,
        -0.2f, -0.2f, 0.0f,    // triangle 2
         0.2f,  0.2f, 0.0f,
        -0.2f,  0.2f, 0.0f
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
    
    // Create Vertex Buffer Object
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Create uv buffer
    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvCoords), uvCoords, GL_STATIC_DRAW);
    
    // Use the shader program
    glUseProgram(shaderID);
    
    // Define the translation matrix
    glm::mat4 translate = glm::mat4(1.0f);
    translate[3][0] = 0.5f, translate[3][1] = 0.3f;
   
    std::cout << "translate = " << glm::transpose(translate) << "\n" <<  std::cout;
  
    // Calculate the transformation matrix
    glm::mat4 transformation = translate;
    
    // Get the handle for the transformation matrix
    GLuint transformationID = glGetUniformLocation(shaderID, "transformation");
    
    // Render loop
    do {
        // Clear the window
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Send the VBO to the shaders
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(
                              0,           // attribute
                              3,           // size
                              GL_FLOAT,    // type
                              GL_FALSE,    // normalise
                              0,           // stride
                              (void*)0     // offset
                              );
        
        // Bind the textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(texture1ID, 0);
        
        // Send the uv buffer to the shaders
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
        
        // Send our transformation matrix to the vertex shader
        glUniformMatrix4fv(transformationID, 1, GL_FALSE, &transformation[0][0]);
        
        // Draw the triangles
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (sizeof(float) * 3));
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &uvBuffer);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderID);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
