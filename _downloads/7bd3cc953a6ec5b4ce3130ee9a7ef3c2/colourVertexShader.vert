#version 330 core

// Input vertex data
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;

// Output data
out vec3 vertexColour;

void main()
{
    // Output vertex position
    gl_Position = vec4(position, 1.0);
    
    // Output vertex colour
    vertexColour = colour;
}
