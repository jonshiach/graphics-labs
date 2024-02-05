#version 330 core

// Input data
in vec3 vertexColour;

// Output data
out vec4 colour;

void main()
{
    colour = vec4(vertexColour, 1.0);    // rgba
}
