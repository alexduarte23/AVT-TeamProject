#version 330 core

in vec4 ex_color;
in vec2 texCoord;

uniform sampler2D in_texture;
uniform sampler2D in_dissolveMap;

layout(location = 0) out vec4 FragmentColor;
layout(location = 1) out vec4 BrightColor;

void main(void)
{
	vec4 texel = texture(in_texture, texCoord);
	texel.a = clamp(texel.a * 3, 0, 1);
	//if (texel.a > 0.1) texel.a = clamp(texel.a * 2.5, 0, 1);


	vec4 color = vec4(vec3(3.5, 2.7, 0.9), texel.a);
	
	float dissolveVal = texture(in_dissolveMap, texCoord).r;
	float threshold = ex_color.w;

	if (dissolveVal > threshold)
		discard;
	else if (dissolveVal > threshold-0.1)
		color.a *= (threshold - dissolveVal) / 0.1;

	if(color.a == 0.0)
		discard;

	FragmentColor = color;

	float brightness = dot(FragmentColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0) //1.0
		BrightColor = vec4(FragmentColor.rgb/2, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}