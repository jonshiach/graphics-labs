#version 330 core

// Input vertex data
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

// Output data
out vec2 uv;

// Values that stay constant for the whole mesh
uniform mat4 transformation;

void main()
{
    // Output vertex postion
    gl_Position = transformation * vec4(position, 1.0);
    
    // Output (u,v) co-ordinates
    uv = vec2(textureCoords);
}
