#version 330 core

in vec2 exTexcoord;
in vec4 exColor;

out vec4 FragmentColor;

uniform sampler2D inTexture;

void main(void)
{
    vec4 texel = texture(inTexture, exTexcoord);

    FragmentColor = texel * exColor;
}
