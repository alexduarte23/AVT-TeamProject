#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in vec3 exColor;
in vec3 FragPos;

layout(location = 0) out vec4 FragmentColor;
layout(location = 1) out vec4 BrightColor;

uniform vec3 LightPosition;
uniform vec3 LightColor;

void main(void)
{

	vec3 objectColor = exColor;

	vec3 lightPos = vec3(3.0, 3.0, 3.0);
	vec3 lightColor = vec3(1.0, 0.3, 0.0);
	vec3 ambientColor = vec3(1.0, 1.0, 1.0);

	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * ambientColor;



	vec3 norm = normalize(exNormal);
	vec3 lightDir = normalize(LightPosition - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * LightColor;

	vec3 result = (ambient + diffuse) * objectColor;
	FragmentColor = vec4(result, 1.0);

	float brightness = dot(FragmentColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0) //1.0
		BrightColor = vec4(FragmentColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}