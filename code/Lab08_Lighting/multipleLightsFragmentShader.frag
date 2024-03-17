#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 fragmentPosition;
in vec3 Normal;

// Output data
out vec3 fragmentColour;

// Structs
struct Light
{
    vec3 colour;
    vec3 position;
    float linear, constant, quadratic;
};

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Uniforms
#define maxLights 100
uniform sampler2D diffuse1;
uniform Light lights[maxLights];
uniform int numLights;
uniform DirLight dirLight;

uniform float ka;
uniform float kd;
uniform float ks;
uniform float Ns;

// Function prototypes
vec3 directionalLight(DirLight dirLight, vec3 normal, vec3 eye, vec3 objectColour);
vec3 pointLight(Light ptLight, vec3 fragmentPosition, vec3 normal, vec3 eye, vec3 objectColour);

void main ()
{
    // Object colour, normal and eye vector (these are the same for all light sources)
    vec3 objectColour = vec3(texture(diffuse1, UV));
    vec3 normal = normalize(Normal);
    vec3 eye = normalize(-fragmentPosition);
    
    // Calculate directional light
    fragmentColour = directionalLight(dirLight, normal, eye, objectColour);
    
    // Loop through the point light sources
    for (int i = 0; i < numLights; i++)
    {
        fragmentColour += pointLight(lights[i], fragmentPosition, normal, eye, objectColour);
    }
}

// Calculate directional lighting
vec3 directionalLight(DirLight dirLight, vec3 normal, vec3 eye, vec3 objectColour)
{
    // Ambient reflection
    vec3 ambient = dirLight.ambient * objectColour;
    
    // Diffuse refection
    vec3 light = normalize(-dirLight.direction);
    float cosTheta = max(dot(normal, light), 0);
    vec3 diffuse = dirLight.diffuse * objectColour * cosTheta;
    
    // Specular light
    vec3 reflection = -light + 2 * dot(light, normal) * normal;
    float cosAlpha = max(dot(eye, reflection), 0);
    vec3 specular = dirLight.specular * pow(cosAlpha, Ns);
    
    // Return fragment colour
    return ambient + diffuse + specular;
}

// Calculate point light
vec3 pointLight(Light ptLight, vec3 fragmentPosition, vec3 normal, vec3 eye, vec3 objectColour)
{
    // Ambient reflection
    vec3 ambient = ka * ptLight.colour * objectColour;
    
    // Diffuse reflection
    vec3 light = normalize(ptLight.position - fragmentPosition);
    float cosTheta = max(dot(normal, light), 0);
    vec3 diffuse = kd * objectColour * ptLight.colour * cosTheta;
    
    // Specular reflection
    vec3 reflection = -light + 2 * dot(light, normal) * normal;
    float cosAlpha = max(dot(eye, reflection), 0);
    vec3 specular = ks * ptLight.colour * pow(cosAlpha, Ns);
    
    // Attenuation
    float distance = length(ptLight.position - fragmentPosition);
    float attenuation = 1.0 / (ptLight.constant + ptLight.linear * distance + ptLight.quadratic * distance * distance);
    
    // Return fragment colour
    return (ambient + diffuse + specular) * attenuation;
}
