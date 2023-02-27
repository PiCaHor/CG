#version 330 core

// in vec4 ourColor;
in vec2 TexCoord;
in vec3 FragPos; 
in vec3 Normal;  

struct Material{
    vec3 ambient; 
    sampler2D diffuse; 
    sampler2D specular; 
    sampler2D emission; 
    float shininess; 
};

struct lightPoint{
    vec3 pos;  
    vec3 dirToLight; 
    vec3 color; 
    float constant; 
    float linear; 
    float quadratic; 
};

struct lightSpot{
    vec3 pos;  
    vec3 dirToLight; 
    vec3 color; 
    float constant; 
    float linear; 
    float quadratic; 
    float cosPhyInner;
    float cosPhyOuter;
};

struct lightDirctional{
    vec3 pos; 
    vec3 dirToLight; 
    vec3 color; 
}; 

uniform Material material; 
uniform lightPoint lightP0; 
uniform lightPoint lightP1; 
uniform lightPoint lightP2; 
uniform lightPoint lightP3; 
uniform lightSpot lightS; 
uniform lightDirctional lightD; 
// uniform sampler2D ourTexture;
// uniform sampler2D ourFace;
uniform vec3 objColor; 
uniform vec3 ambientColor; 
uniform vec3 cameraPos; 

out vec4 FragColor;

vec3 CalcLightDirectional(lightDirctional light,vec3 uNormal, vec3 dirToCamera)
{
    float diffIntensity = max(0, dot(light.dirToLight,uNormal)); 
    vec3 diffcolor = diffIntensity * light.color * texture(material.diffuse, TexCoord).rgb; 

    vec3 R = normalize(reflect(-light.dirToLight,uNormal)); 
    float specIntensity = pow(max(0, dot(R,dirToCamera)) , material.shininess); 
    vec3 speccolor = specIntensity * light.color * texture(material.specular, TexCoord).rgb; 

    vec3 result = diffcolor + speccolor; 
    return result; 
}

vec3 CalcLightPoint(lightPoint light,vec3 uNormal, vec3 dirToCamera)
{
    float dist = length(light.pos - FragPos);
    float attenuation  = 1 / (light.constant +  light.linear * dist + light.quadratic * dist * dist);  

    float diffIntensity = max(0, dot(normalize(light.pos - FragPos),uNormal)) * attenuation; 
    vec3 diffcolor = diffIntensity * light.color * texture(material.diffuse, TexCoord).rgb; 

    vec3 R = normalize(reflect(-normalize(light.pos - FragPos),uNormal)); 
    float specIntensity = pow(max(0, dot(R,dirToCamera)) , material.shininess) * attenuation; 
    vec3 speccolor = specIntensity * light.color * texture(material.specular, TexCoord).rgb; 

    vec3 result = diffcolor + speccolor; 
    return result; 
}
 
vec3 CalcLightSpot(lightSpot light,vec3 uNormal, vec3 dirToCamera)
{
    float dist = length(light.pos - FragPos);
    float attenuation  = 1 / (light.constant +  light.linear * dist + light.quadratic * dist * dist);  
    float spotRatio; 
    float cosTheta = dot(normalize(FragPos - light.pos),-light.dirToLight); 

    if(cosTheta > light.cosPhyInner)
    {
        spotRatio = 1.0f; 
    }
    else if(cosTheta > light.cosPhyOuter)
    {
        spotRatio = (cosTheta - light.cosPhyOuter) / (light.cosPhyInner - light.cosPhyOuter ); 
    }
    else 
    {
        spotRatio = 0; 
    }

    float diffIntensity = max(0, dot(normalize(light.pos - FragPos),uNormal)) * attenuation * spotRatio; 
    vec3 diffcolor = diffIntensity * light.color * texture(material.diffuse, TexCoord).rgb; 

    vec3 R = normalize(reflect(-normalize(light.pos - FragPos),uNormal)); 
    float specIntensity = pow(max(0, dot(R,dirToCamera)) , material.shininess) * attenuation * spotRatio; 
    vec3 speccolor = specIntensity * light.color * texture(material.specular, TexCoord).rgb; 

    vec3 result = diffcolor + speccolor; 
    return result; 
}

void main()
{   
    vec3 finalResult = vec3(0,0,0); 
    vec3 uNormal = normalize(Normal);
    vec3 dirToCamera = normalize(cameraPos - FragPos);  

    finalResult += CalcLightDirectional(lightD,uNormal,dirToCamera); 
    finalResult += CalcLightPoint(lightP0,uNormal,dirToCamera); 
    finalResult += CalcLightPoint(lightP1,uNormal,dirToCamera); 
    finalResult += CalcLightPoint(lightP2,uNormal,dirToCamera); 
    finalResult += CalcLightPoint(lightP3,uNormal,dirToCamera); 
    finalResult += CalcLightSpot(lightS,uNormal,dirToCamera); 

    FragColor = vec4(finalResult, 1.0); 
}