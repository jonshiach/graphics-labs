#version 330 core

// Inputs
in vec2 UV;
in vec3 fragPos;
in vec3 Normal;

// Outputs
out vec3 colour;

// Structs
struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 att;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 att;
    float cosPhi;
};

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Uniforms
uniform sampler2D textureMap;
uniform float Ns;
uniform int numLights;
uniform int numSpotLights;
uniform Light lightSources[10];
uniform SpotLight spotLightSources[10];
uniform DirLight dirLight;

// Function prototypes
vec3 calculatePointLight(Light src, vec3 fragPos, vec3 normal, vec3 camera);
vec3 calculateSpotLight(SpotLight src, vec3 fragPos, vec3 normal, vec3 camera);
vec3 calculateDirLight(DirLight dirLight, vec3 normal, vec3 camera);

void main ()
{
    // Calculate normal and camera vectors (these are the same for all light sources)
    vec3 normal = normalize(Normal);
    vec3 camera = normalize(- fragPos);

    // Loop through the light sources
    colour = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < numLights; i++)
    {
        colour += calculatePointLight(lightSources[i], fragPos, normal, camera);
    }
    
    // Loop through the directional light sources
    for (int i = 0; i < numSpotLights; i++)
    {
        colour += calculateSpotLight(spotLightSources[i], fragPos, normal, camera);
    }
    
    // Calculate the directional light
    colour += calculateDirLight(dirLight, normal, camera);
}

// Calculate point light
vec3 calculatePointLight(Light src, vec3 fragPos, vec3 normal, vec3 camera)
{
    // Object colour
    vec3 objectColour = vec3(texture(textureMap, UV));
    
    // Ambient reflection
    vec3 ambient = src.ambient * objectColour;
    
    // Diffuse reflection
    vec3 light      = normalize(src.position - fragPos);
    float cosTheta  = max(dot(normal, light), 0);
    vec3 diffuse    = src.diffuse * objectColour * cosTheta;
    
    // Specular reflection
    vec3 reflection = - light + 2 * dot(light, normal) * normal;
    float cosAlpha  = max(dot(camera, reflection), 0);
    vec3 specular   = src.specular * pow(cosAlpha, Ns);
    
    // Attenuation
    float distance    = length(src.position - fragPos);
    float attenuation = 1.0 / (src.att[0] + src.att[1] * distance +
                               src.att[2] * distance * distance);
    
    // Fragment colour
    return (ambient + diffuse + specular) * attenuation;
}

// Calculate spotlight
vec3 calculateSpotLight(SpotLight src, vec3 fragPos, vec3 normal, vec3 camera)
{
    // Object colour
    vec3 objectColour = vec3(texture(textureMap, UV));
    
    // Ambient reflection
    vec3 ambient = src.ambient * objectColour;
    
    // Diffuse reflection
    vec3 light     = normalize(src.position - fragPos);
    float cosTheta = max(dot(normal, light), 0);
    vec3 diffuse   = src.diffuse * objectColour * cosTheta;
    
    // Specular reflection
    vec3 reflection = - light + 2 * dot(light, normal) * normal;
    float cosAlpha  = max(dot(camera, reflection), 0);
    vec3 specular   = src.specular * pow(cosAlpha, Ns);
    
    // Attenuation
    float distance    = length(src.position - fragPos);
    float attenuation = 1.0 / (src.att[0] + src.att[1] * distance +
                               src.att[2] * distance * distance);
    
    // Directional light intensity
    vec3 direction  = normalize(src.direction);
    cosTheta        = dot(- light, direction);
//    float i  ntensity = 0.0;
//    if (cosTheta > src.cosPhi)
//        intensity = 1.0;
    float delta     = radians(2.0);
    float intensity = clamp((cosTheta - src.cosPhi) / delta, 0.0, 1.0);
    
    // Return fragment colour
    return (ambient + diffuse + specular) * attenuation * intensity;
}

// Calculate directional light
vec3 calculateDirLight(DirLight src, vec3 normal, vec3 camera)
{
    // Object colour
    vec3 objectColour = vec3(texture(textureMap, UV));
    
    // Ambient reflection
    vec3 ambient = src.ambient * objectColour;
    
    // Diffuse reflection
    vec3 light     = normalize(- src.direction);
    float cosTheta = max(dot(normal, light), 0);
    vec3 diffuse   = src.diffuse * objectColour * cosTheta;
    
    // Specular reflection
    vec3 reflection = - light + 2 * dot(light, normal) * normal;
    float cosAlpha  = max(dot(camera, reflection), 0);
    vec3 specular   = src.specular * pow(cosAlpha, Ns);
    
    // Return fragment colour
    return ambient + diffuse + specular;
}
