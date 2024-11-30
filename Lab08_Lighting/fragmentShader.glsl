#version 330 core

in vec2 uv;
in vec3 fragmentPosition;
in vec3 Normal;

out vec3 colour;

// Uniforms
uniform sampler2D textureMap;
uniform vec3 ambientLight;
uniform vec3 diffuseLight;
uniform vec3 lightPosition;

void main()
{
    // Object colour
    vec3 objectColour = texture(textureMap, uv).rgb;
    
    // Ambient reflection
    vec3 ambient = ambientLight * objectColour;
    
    // Diffuse reflection
    vec3 normal = normalize(Normal);
    vec3 light  = normalize(lightPosition - fragmentPosition);
    float cosTheta = max(dot(normal, light), 0);
    vec3 diffuse = diffuseLight * objectColour * cosTheta;
    
    // Fragment colour
    colour = ambient + diffuse;
}
