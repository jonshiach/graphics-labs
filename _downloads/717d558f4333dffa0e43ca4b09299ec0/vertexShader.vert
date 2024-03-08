#version 330 core

// Input vertex data
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

// Output data
out vec2 UV;
out vec3 FragmentPosition;
out vec3 Normal;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Output vertex postion
    gl_Position = projection * view * model * vec4(position, 1.0);
    
    // Output (u,v) co-ordinates
    UV = uv;
    
    // Output view space fragment position and normal
    FragmentPosition = vec3(view * model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * normal;
}
