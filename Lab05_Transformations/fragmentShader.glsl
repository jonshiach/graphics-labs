#version 330 core

in vec2 uv;

out vec3 colour;

uniform sampler2D textureMap;

void main()
{
    colour = vec3(texture(textureMap, uv));
}
