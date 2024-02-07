#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "texture.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/io.hpp>

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
    GLFWwindow* window = glfwCreateWindow( 1024, 768, "3D World", NULL, NULL);
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
    GLuint texture1 = loadBMP_custom("crate.bmp");
    
    // Get a handle for the texture sampler uniforms
    GLuint texture1ID = glGetUniformLocation(shaderID, "texture1Sampler");
    
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
        
        0.0f, 0.0f,    // right
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
        
        0.0f, 0.0f,    // left
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        
        0.0f, 0.0f,    // base
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
        0.0f, 1.0f,
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
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    
    do {
        // Clear the window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader program
        glUseProgram(shaderID);
        
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
        
        // Calculate Model matrix
        float time = glfwGetTime();
        glm::vec3 modelCentre = glm::vec3(0.0f, 0.0, -1.0f);
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), modelCentre);
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));
//        glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotate = glm::mat4(1.0f);
        glm::mat4 model = translate * rotate * scale;
        
//        // Send the model matrix to the vertex shader
//        GLuint modelID = glGetUniformLocation(shaderID, "model");
//        glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
//
//        // Calculate view matrix
//        glm::vec3 camera = glm::vec3(1.0f, 1.0f, 0.0f);
//        glm::vec3 target = modelCentre;
//        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
//
//        glm::vec3 cameraForward = glm::normalize(camera - target);
//        glm::vec3 cameraRight = glm::normalize(glm::cross(worldUp, cameraForward));
//        glm::vec3 cameraUp = glm::cross(cameraForward, cameraRight);
//
//        glm::mat4 align = glm::mat4(1.0f);
//        translate[3][0] = -camera[0], translate[3][1] = -camera[1], translate[3][2] = -camera[2];
//        align[0][0] = cameraRight[0], align[0][1] = cameraUp[0], align[0][2] = cameraForward[0];
//        align[1][0] = cameraRight[1], align[1][1] = cameraUp[1], align[1][2] = cameraForward[1];
//        align[2][0] = cameraRight[2], align[2][1] = cameraUp[2], align[2][2] = cameraForward[2];
//
//        glm::mat4 view = align * translate;
//
//        // Send the view matrix to the vertex shader
//        GLuint viewID = glGetUniformLocation(shaderID, "view");
//        glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
//
//        // Calculate orthographic projection matrix
//        float left, right, near, far, top, bottom;
//        left = -0.8f, right = 0.8f;
//        bottom = -0.8f, top = 0.8f;
//        near = 0.0f, far = 10.0f;
//
//        glm::mat4 projection = glm::mat4(1.0f);
//        projection[0][0] = 2.0f / (right - left);
//        projection[1][1] = 2.0f / (top - bottom);
//        projection[2][2] = 2.0f / (near - far);
//        projection[3][0] = - (right + left) / (right - left);
//        projection[3][1] = - (top + bottom) / (top - bottom);
//        projection[3][2] = (near + far) / (near - far);
//
//        // ...or use glm::ortho()
//        glm::mat4 projection = glm::ortho(left, right, bottom, top, near, far);
//
//        // Calculate perspective projection matrix
//        float right, top, near, far, fov, aspect;
//        near = 0.2f;
//        far = 10.0f;
//        fov = glm::radians(45.0f);
//        aspect = 1024.0f / 768.0f;
//        top = near * glm::tan(fov / 2);
//        right = aspect * top;
//
//        glm::mat4 projection = glm::mat4(1.0f);
//        projection[0][0] = near / right;
//        projection[1][1] = near / top;
//        projection[2][2] = - (far + near) / (far - near);
//        projection[2][3] = -1.0f;
//        projection[3][2] = - 2 * far * near / (far - near);;
//        projection[3][3] = 0.0f;
//
//        // ...or use glm::perspective()
//        glm::mat4 projection = glm::perspective(fov, aspect, near, far);
//
//        // Send our projection matrix to the vertex shader
//        GLuint projectionID = glGetUniformLocation(shaderID, "projection");
//        glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
        
        // Calculate the MVP matrix
        glm::mat4 view = glm::lookAt(glm::vec3(1.0f, 1.0f, 0.0f),       // camera
                                     glm::vec3(0.0f, 0.0f, -1.0f),      // target
                                     glm::vec3(0.0, 1.0f, 0.0f));       // worldUp
        glm::mat4 projection = glm::perspective(glm::radians(40.0f),    // field of view
                                                1024.0f / 768.0f,       // aspect (width / height)
                                                0.2f,                   // near
                                                10.0f);                 // far
        glm::mat4 mvp = projection * view * model;
        
        // Send the mvp matrix to the vertex shader
        GLuint mvpID = glGetUniformLocation(shaderID, "mvp");
        glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
        
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
        
        // Draw the triangle
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
