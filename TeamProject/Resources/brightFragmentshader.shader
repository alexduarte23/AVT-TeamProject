#version 330 core

in vec2 exTexcoord; 
out vec4 FragmentColor;

uniform sampler2D TexFramebuffer;

void main(void)
{
	vec4 color = texture(TexFramebuffer, exTexcoord);

	//Luma
    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 0.4)
        FragmentColor = vec4(color.rgb, 1.0);
    else
        FragmentColor = vec4(0.0, 0.0, 0.0, 1.0);
}