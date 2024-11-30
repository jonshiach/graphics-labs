#version 330 core

in vec2 uv;

out vec3 colour;

uniform sampler2D textureMap;

void main()
{
    colour = texture(textureMap, uv).rgb;
}
