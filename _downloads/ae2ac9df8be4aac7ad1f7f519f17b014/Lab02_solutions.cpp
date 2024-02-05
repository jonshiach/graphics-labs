#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"

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
    GLFWwindow* window = glfwCreateWindow( 1024, 768, "Hello Window", NULL, NULL);
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
    
    // Compile shader program - comment/uncomment as appropriate
//    GLuint shaderID = LoadShaders("simpleVertexShader.vert", "simpleFragmentShader.frag");
    GLuint shaderID = LoadShaders("colourVertexShader.vert", "colourFragmentShader.frag");
//    GLuint shaderID = LoadShaders("ex1VertexShader.vert", "colourFragmentShader.frag");
    
    // Dark grey background
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    
    // Create the Vertex Array Object (VAO)
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // Define vertex positions
    /*
    // Single triangle
    static const GLfloat vertices[] = {
        // x     y      z
        -0.5f, -0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
         0.0f,  0.5f,  0.0f
    };
     
    // Define vertex colours
    static const GLfloat colours[] = {
     // R   G     B
     1.0f, 0.0f, 0.0f,    // red
     0.0f, 1.0f, 0.0f,    // green
     0.0f, 0.0f, 1.0f,    // blue
    };
    */
    
    /*
    // Two triangles
    // Define vertices
    static const GLfloat vertices[] = {
        -0.9f, -0.5f, 0.0f,    // triangle 1
        -0.1f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
         0.1f, -0.5f, 0.0f,    // triangle 2
         0.9f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f
    };

    // Define colours
    static const GLfloat colours[] = {
        1.0f, 0.0f, 0.0f,    // red
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,    // blue
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };
    */
    
    /*
    // Exercise 1
    static const GLfloat vertices[] = {
        // x     y      z
        -0.5f, -0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
         0.0f,  0.5f,  0.0f
    };
     
    // Define vertex colours
    static const GLfloat colours[] = {
         // R   G     B
         1.0f, 0.0f, 0.0f,    // red
         1.0f, 0.0f, 0.0f,    // green
         1.0f, 0.0f, 0.0f,    // blue
    };
    */
    
    /*
    // Exercise 2
    static const GLfloat vertices[] = {
        // x     y     z
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    
    // Define vertex colours
    static const GLfloat colours[] = {
        // R   G     B
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    */
    
    // Exercise 3
    static const GLfloat vertices[] = {
        // x     y     z
        -0.25f, -0.50f, 0.0f,   // triangle 1
         0.25f, -0.50f, 0.0f,
         0.00f,  0.00f, 0.0f,
         0.25f, -0.50f, 0.0f,   // triangle 2
         0.50f,  0.00f, 0.0f,
         0.00f,  0.00f, 0.0f,
         0.50f,  0.00f, 0.0f,   // triangle 3
         0.25f,  0.50f, 0.0f,
         0.00f,  0.00f, 0.0f,
         0.25f,  0.50f, 0.0f,   // triangle 4
        -0.25f,  0.50f, 0.0f,
         0.00f,  0.00f, 0.0f,
        -0.25f,  0.50f, 0.0f,   // triangle 5
        -0.50f,  0.00f, 0.0f,
         0.00f,  0.00f, 0.0f,
        -0.50f,  0.00f, 0.0f,   // triangle 6
        -0.25f, -0.50f, 0.0f,
         0.00f,  0.00f, 0.0f
    };
    
    // Define vertex colours
    static const GLfloat colours[] = {
        // R   G     B
        1.0f, 0.0f, 0.0f,   // red
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,   // green
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,   // green
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f,   // yellow
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,   // magenta
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f,   // cyan
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f
    };
    
    
    // Create Vertex Buffer Object (VBO)
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Create colour buffer
    GLuint colourBuffer;
    glGenBuffers(1, &colourBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
    
    // Render loop
	do {
        // Clear the window
		glClear(GL_COLOR_BUFFER_BIT);

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

        // Send colours to the shaders
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
        glVertexAttribPointer(
                              1,           // attribute
                              3,           // size
                              GL_FLOAT,    // type
                              GL_FALSE,    // normalise
                              0,           // stride
                              (void*)0     // offset
                              );
        
        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (3 * sizeof(GLfloat)));
        glDisableVertexAttribArray(0);
        
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
    
    // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );
    
    // Cleanup
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderID);
    
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
