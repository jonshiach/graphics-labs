#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/io.hpp>

#include "shader.hpp"
#include "texture.hpp"

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
    GLFWwindow* window = glfwCreateWindow( 1024, 768, "3D Worlds", NULL, NULL);
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
    
    // Use the shader program
    glUseProgram(shaderID);
    
    // Create OpenGL textures
    GLuint texture1 = loadBMP_custom("crate.bmp");
    
    // Get the handles for the shader uniforms
    GLuint texture1ID = glGetUniformLocation(shaderID, "texture1Sampler");
    GLuint modelID = glGetUniformLocation(shaderID, "model");
    GLuint viewID = glGetUniformLocation(shaderID, "view");
    GLuint projectionID = glGetUniformLocation(shaderID, "projection");
    
    // Create the Vertex Array Object (VAO)
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // Define vertex positions
    static const GLfloat vertices[] = {
        -1.0f, -1.0f,  1.0f,    // front
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,

         1.0f, -1.0f,  1.0f,    // right
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,    // back
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,    // left
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,    // base
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f,  1.0f,    // top
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f
    };

    // Define texture co-ordinates
    static const GLfloat uvCoords[] = {
        // u    v
        0.0f, 0.0f,    // base
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        
        0.0f, 0.0f,    // front
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        
        0.0f, 0.0f,    // right
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        
        0.0f, 0.0f,    // back
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        
        0.0f, 0.0f,    // left
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        
        0.0f, 0.0f,    // top
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
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
    
    do {
        // Clear the window
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Bind the textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(texture1ID, 0);
        
        // Send the VBO to the shaders
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
        // Send the uv buffer to the shaders
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
      
        // Calculate the model matrix
        float time = glfwGetTime();
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 model = translate * rotate * scale;
        
        // Calculate view matrix
        glm::vec3 position = glm::vec3(1.0f, 1.0f, 0.0f);
        glm::vec3 target = glm::vec3(0.0f, 0.0f, -4.0f);
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec3 cameraFront = glm::normalize(target - position);
        glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
        glm::vec3 cameraUp = glm::cross(cameraRight, cameraFront);

        glm::mat4 align = glm::mat4(1.0f);
        translate[3][0] = -position[0], translate[3][1] = -position[1], translate[3][2] = -position[2];
        align[0][0] = cameraRight[0], align[0][1] = cameraUp[0], align[0][2] = -cameraFront[0];
        align[1][0] = cameraRight[1], align[1][1] = cameraUp[1], align[1][2] = -cameraFront[1];
        align[2][0] = cameraRight[2], align[2][1] = cameraUp[2], align[2][2] = -cameraFront[2];

        glm::mat4 view = align * translate;
             
        // Calculate projection matrix (orthographic projection)
        float left, right, near, far, top, bottom;
        left = -2.0f, right = 2.0f;
        bottom = -2.0f, top = 2.0f;
        near = 0.0f, far = 10.0f;

        glm::mat4 projection = glm::mat4(1.0f);
        projection[0][0] = 2.0f / (right - left);
        projection[1][1] = 2.0f / (top - bottom);
        projection[2][2] = 2.0f / (near - far);
        projection[3][0] = - (right + left) / (right - left);
        projection[3][1] = - (top + bottom) / (top - bottom);
        projection[3][2] = (near + far) / (near - far);
       
        // Send the model, view and projection matrices to the shader
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
        
        // Draw the triangles
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (sizeof(float) * 3));
        
        // Disable VAOs
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
