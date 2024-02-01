#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;

// Output data
out vec3 vertexColour;

void main() {

    // Vertex position
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    
//    gl_Position = vec4(position.x + 0.5, position.y, position.z, 1.0); // Ex 1(a)
//    gl_Position = vec4(position.x, -position.y, position.z, 1.0); // Ex 1(b)
//    gl_Position = vec4(position.y, position.x, position.z, 1.0); // Ex 1(c)
    
    // Vertex colour
    vertexColour = colour;

}
