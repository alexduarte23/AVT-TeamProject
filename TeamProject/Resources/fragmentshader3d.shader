#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in vec3 FragPos;

out vec4 FragmentColor;

uniform vec3 LightPosition;
uniform vec3 LightColor;

void main(void)
{
	/**/
	//	vec3 color = vec3(1.0);
	//	vec3 color = (exPosition + vec3(1.0)) * 0.5;
	//	vec3 color = vec3(exTexcoord, 0.0);

	vec3 objectColor = vec3(0.2, 0.4, 0.2);

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
}