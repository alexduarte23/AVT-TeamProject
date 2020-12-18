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

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir)
{
	// Adjustment to perspective projection
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    //Turn from [-1, 1] to [0, 1]
    projCoords = projCoords * 0.5 + 0.5;

    //Get the closest depth position
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    //Get the depth of the current fragment
    float currentDepth = projCoords.z;

    //Create a shadow bias value to avoid shadow acne, based on the light direction and the fragment no
    //float bias = max(0.05 * (1.0 - dot(exNormal, lightDir)), 0.002); 
    float bias = 0.002;
	//float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0; 
    
	//PCF TEST
	float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }

    if(projCoords.z > 1.0)
        shadow = 0.0;
    shadow /= 9.0;
    
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

	float shadow = ShadowCalculation(FragPosLightSpace, lightDir);       
    vec3 lighting = (ambient + (0.8 - shadow) * diffuse) * objectColor;    
    
    FragmentColor = vec4(lighting, 1.0);
}
