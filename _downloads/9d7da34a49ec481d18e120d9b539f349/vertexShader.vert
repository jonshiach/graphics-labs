#version 330 core

// Input vertex data
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

// Output data
out vec2 uv;

// Values that stay constant for the whole mesh
// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection
uniform mat4 mvp;

void main()
{
    // Output vertex position
    // gl_Position = projection * view * model * vec4(position, 1.0);
    gl_Position = mvp * vec4(position, 1.0);
    
    // Output (u,v) co-ordinates
    uv = vec2(textureCoords);
}
