#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in vec3 exColor;

in vec3 FragPos;
in vec4  FragPosLightSpace;

out vec4 FragmentColor;

uniform vec3 LightPosition;
uniform vec3 LightColor;

uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	float bias = 0.002;
	float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0; 

    return shadow;

}

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

	//vec3 result = (ambient + diffuse) * objectColor;

	float shadow = ShadowCalculation(FragPosLightSpace);       
    vec3 lighting = (ambient + (1.0 - shadow) * diffuse) * objectColor;    
    
    FragmentColor = vec4(lighting, 1.0);
}
