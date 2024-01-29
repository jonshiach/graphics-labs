// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include shader class
#include <common/shader.hpp>

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow( 1024, 768, "Hello Triangle", NULL, NULL);
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
	
	// Grey background
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	
	// Create Vertex Array Object (VAO)
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	// Define vertex co-ordinates
	static const GLfloat vertices[] = {
		-0.5f, -0.5f,  0.0f,
	 	 0.5f, -0.5f,  0.0f,
		 0.0f,  0.5f,  0.0f
	};
	
	// Create vertex buffer
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// Compile shader program
	GLuint shaderID = LoadShaders("simpleVertexShader.vs", "simpleFragmentShader.fs");
	
	do {
		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );
		
		// Use the shader program
		glUseProgram(shaderID);
		
		// Send vertices to buffer
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
					0,			// attribute
					3,			// size
					GL_FLOAT,	// type
					GL_FALSE,	// normalise?
					0,			// stride
					(void*)0	// offset
					);
	
		// Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);	// 3 indices starting at 0
		glDisableVertexAttribArray(0);		// finished with buffer so delete it
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0 );
	
	// Cleanup
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(shaderID);
	
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	
	return 0;
}

