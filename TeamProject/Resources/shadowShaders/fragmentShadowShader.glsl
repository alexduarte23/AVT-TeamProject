#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in vec3 exColor;

in vec3 FragPos;
in vec4 FragPosLightSpace;
in vec4 FragPosLightSpace2;
in vec4 FragPosLightSpace3;
in vec4 FragPosLightSpace4;
in vec4 FragPosLightSpace5;
in vec4 FragPosLightSpace6;


layout(location = 0) out vec4 FragmentColor;
layout(location = 1) out vec4 BrightColor;

uniform vec3 AmbientColor;

uniform vec3 campfirePos;
uniform vec3 campfireColor;

uniform vec3 envPos;
uniform vec3 envColor;

uniform vec3 EyePosition;

uniform sampler2D campfireSM1;
uniform sampler2D campfireSM2;
uniform sampler2D campfireSM3;
uniform sampler2D campfireSM4;
uniform sampler2D campfireSM5;

uniform sampler2D envSM;


float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, sampler2D currShadowMap)
{
	// Adjustment to perspective projection
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    //Turn from [-1, 1] to [0, 1]
    projCoords = projCoords * 0.5 + 0.5;

    //Get the closest depth position
    float closestDepth = texture(currShadowMap, projCoords.xy).r; 

    //Get the depth of the current fragment
    float currentDepth = projCoords.z;

    //Shadow bias to avoid artifacts
    float bias = 0.0006;
    
    float shadow = 0.0;

    //Avoid having shadow when outside light projection 
    if(projCoords.z > 1.0)
        return shadow;
    
    
    //Check if in shadow intersection, and avoid doing smoothing to mitigate smoothing artifacts in the shadow joins
    vec3 v = FragPos - campfirePos;
    float dist = 0.01*length(v);
    if((v.x-v.z < dist && v.x-v.z > -dist) || ( v.x+v.z < dist && v.x+v.z > -dist )){
        shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0; 
        return shadow;
    }
 

	//Apply smoothing to the shadow
    vec2 texelSize = 1.0 / textureSize(currShadowMap, 0);
    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            float pcfDepth = texture(currShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 25.0;
    
    return shadow;

}

int shadowSelect(vec3 lightPos, vec3 fPos){
    vec3 v = fPos - lightPos;
    if(v.y < v.x && v.y < -v.x && v.y < v.z && v.y < -v.z && v.y < 0){
        //5 - down
        return 5;
    } else
    if(v.x > v.z && v.x > -v.z && v.x > 0){
        //1 - xplus
        return 1;
    } else
    if(v.x < v.z && v.x < -v.z && v.x < 0){
        //2 - xminus
        return 2;
    } else
    if(v.z > v.x && v.z > -v.x && v.z>0){
        //3 - zplus
        return 3;
    } else
    if(v.z < v.x && v.z < -v.x && v.z<0){
        //4 - zminus
        return 4;
    } else
    return 0;
}

void main(void)
{

	vec3 objectColor = exColor;

    float shininess = 10;

	vec3 ambient = AmbientColor;

    //Campfire Calculation
	vec3 norm = normalize(exNormal);
	vec3 lightDir = normalize(campfirePos - FragPos);
    vec3 viewDir = normalize(EyePosition - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * campfireColor;

    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = campfireColor * spec;

    //Point light factors
    float constant = 0.0;
    float linear = 0.5;
    float quadratic = 1;

    float dist = length(FragPos - campfirePos);
    float strength = 10/(constant + linear*dist + quadratic*(dist*dist));

    specular = specular * strength;
    diffuse = diffuse * strength;

    float shadow = 0.0;
    int select = shadowSelect(campfirePos, FragPos);
    if(select==1){
        shadow = ShadowCalculation(FragPosLightSpace, lightDir, campfireSM1);
    }
    if(select==2){
        shadow = ShadowCalculation(FragPosLightSpace2, lightDir, campfireSM2); 
    }
    if(select==3){
        shadow = ShadowCalculation(FragPosLightSpace3, lightDir, campfireSM3); 
    }
    if(select==4){
        shadow = ShadowCalculation(FragPosLightSpace4, lightDir, campfireSM4); 
    }
    if(select==5){
        shadow = ShadowCalculation(FragPosLightSpace5, lightDir, campfireSM5); 
    }
    
    vec3 lighting = (ambient + (1 - shadow) * (diffuse + specular)) * objectColor;  
    
    //Environment Light Calculation

	lightDir = normalize(envPos);
    halfwayDir = normalize(lightDir + viewDir);

	diff = max(dot(norm, lightDir), 0.0);
	diffuse = diff * envColor;

    spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    specular = envColor * spec;

    shadow = 0.0;
    shadow = ShadowCalculation(FragPosLightSpace6, lightDir, envSM);
    
    lighting = lighting + (1 - shadow) * (diffuse + specular) * objectColor; 


    FragmentColor = vec4(lighting, 1.0);

    float brightness = dot(FragmentColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0) //1.0
		BrightColor = vec4(FragmentColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
