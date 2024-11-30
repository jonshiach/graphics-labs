#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 uv;
out vec3 fragmentPosition;
out vec3 Normal;

// Uniforms
uniform mat4 mvp;
uniform mat4 mv;

void main()
{
    // Output vertex position
    gl_Position = mvp * vec4(position, 1.0);
    
    // Output texture co-ordinates
    uv = texCoord;
    
    // Output view space fragment position and normal vector
    fragmentPosition = vec3(mv * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(mv))) * normal;
}
