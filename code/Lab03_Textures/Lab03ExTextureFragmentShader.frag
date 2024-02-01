#version 330 core

// Interpolated values from the vertex shaders
in vec2 uv;

// Output data
out vec3 colour;

// Values that stay constant for the whole mesh
uniform sampler2D texture1Sampler;
uniform sampler2D texture2Sampler;
uniform sampler2D texture3Sampler;

void main ()
{
    // Output colour = colour of the texture at (u, v)
    // Single texture
    colour = texture(texture1Sampler, uv).rgb;
    
    // Two textures
//    colour = mix(texture(texture1Sampler, uv).rgb,
//                 texture(texture2Sampler, uv).rgb, 0.3);
    
    // Exercise 1 - Reverse u co-ordinates of texture2 (smiley face)
//    colour = texture(texture2Sampler, vec2(1.0 - uv.x, uv.y)).rgb;
    
    // Exercise 3 - Kratos selfie texture
//    colour = texture(texture3Sampler, uv).rgb;
}
