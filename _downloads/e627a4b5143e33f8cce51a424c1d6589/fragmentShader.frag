#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 FragmentPosition;
in vec3 Normal;

// Output data
out vec3 colour;

// Uniforms
uniform sampler2D Texture;
uniform vec3 lightColour;
uniform vec3 ambientColour;
uniform vec3 lightPosition;

// Constants
float ka = 0.2;
float kd = 0.7;
float ks = 1.0;
int Ns = 20;
float kc = 1.0;
float kl = 0.1;
float kq = 0.05;

void main ()
{
    // Object colour (not using texture mapping yet)
    //vec3 objectColour = texture(Texture, uv).xyz;
    vec3 objectColour = vec3(0.0f, 0.0f, 1.0f);
    
    // Ambient light
    vec3 ambient = ka * ambientColour * objectColour;
    
    // Diffuse light
    vec3 normal = normalize(Normal);
    vec3 light = normalize(lightPosition - FragmentPosition);
    float cosTheta = max(dot(normal, light), 0);
    vec3 diffuse = kd * objectColour * lightColour * cosTheta;
    
    // Specular light
    vec3 eye = normalize(-FragmentPosition);
    vec3 reflection = -light + 2 * dot(light, normal) * normal;
    float cosAlpha = max(dot(eye, reflection), 0);
    vec3 specular = ks * lightColour * pow(cosAlpha, Ns);
    
    // Attenuation
    float distance = length(lightPosition - FragmentPosition);
    float attenuation = 1.0 / (kc + kl * distance + kq * distance * distance);
    
    // Fragment colour
    colour = (ambient + diffuse + specular) * attenuation;
}
