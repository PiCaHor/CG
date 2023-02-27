#version 330 core

out vec4 FragColor;

struct PointLight {
    vec3 position;
    vec3 color;
};


in vec3 TexCoords;
in vec3 FragPos;

#define NR_POINT_LIGHTS 16

uniform vec3 viewPos;
uniform vec3 normal;
uniform float shininess;
uniform PointLight pointlights[NR_POINT_LIGHTS];
uniform vec3 amb;
uniform vec3 dif;
uniform vec3 spe;
uniform float constant;
uniform float linear;
uniform float quadratic;
	
uniform samplerCube skybox;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 Reinhard(vec3 hdrColor);

void main()
{    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointlights[i], normal, FragPos, viewDir);

    result = Reinhard(result);

    FragColor = vec4(result, 1.0) * texture(skybox, TexCoords);
    //FragColor = texture(skybox, TexCoords);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float distance = length(light.position - fragPos) / 7;
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));    

    vec3 ambient = amb * light.color;
    vec3 diffuse = dif * max(dot(normal, lightDir), 0.0) * light.color;
    vec3 specular = spe * pow(max(dot(normal, halfwayDir), 0.0), 4 * shininess) * light.color;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 Reinhard(vec3 hdrColor)
{
    float gamma = 2.2;
    float exposure = 1.0;

    //vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

    mapped = pow(mapped, vec3(1.0 / gamma));
    return mapped;
}