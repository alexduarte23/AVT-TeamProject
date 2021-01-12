#version 330 core

in vec4 ex_color;
in vec2 texCoord;

uniform sampler2D in_texture;

layout(location = 0) out vec4 FragmentColor;
layout(location = 1) out vec4 BrightColor;

void main(void)
{
	vec4 texel = texture(in_texture, texCoord);
	if(texel.a * ex_color.a == 0.0)
		discard;

	FragmentColor = texel * ex_color;

	float brightness = dot(FragmentColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0) //1.0
		BrightColor = vec4(FragmentColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}