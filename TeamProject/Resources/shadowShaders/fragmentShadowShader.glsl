#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in vec3 exColor;

in vec3 FragPos;
in vec4  FragPosLightSpace;

layout(location = 0) out vec4 FragmentColor;
layout(location = 1) out vec4 BrightColor;

uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform vec3 EyePosition;

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
    float bias = 0.001;
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

	vec3 ambientColor = vec3(1.0, 1.0, 1.0);

    float shininess = 100;

	float ambientStrength = 0.05;
	vec3 ambient = ambientStrength * ambientColor;

	vec3 norm = normalize(exNormal);
	vec3 lightDir = normalize(LightPosition - FragPos);
    vec3 viewDir = normalize(EyePosition - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * LightColor;

    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = LightColor * spec;

    //Point light factors
    float constant = 0.0;
    float linear = 0.5;
    float quadratic = 1;

    float dist = length(FragPos - LightPosition);
    float strength = 10/(constant + linear*dist + quadratic*(dist*dist));

	float shadow = ShadowCalculation(FragPosLightSpace, lightDir);       
    vec3 lighting = (ambient + (0.8 - shadow) * (diffuse + specular))* strength * objectColor;  
    
    FragmentColor = vec4(lighting, 1.0);

    float brightness = dot(FragmentColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0) //1.0
		BrightColor = vec4(FragmentColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
