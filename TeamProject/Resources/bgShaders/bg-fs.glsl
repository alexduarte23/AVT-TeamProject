#version 330 core

in vec4 exColor;

layout(location = 0) out vec4 FragmentColor;
layout(location = 1) out vec4 BrightColor;


void main(void)
{
    FragmentColor = exColor;
    BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
