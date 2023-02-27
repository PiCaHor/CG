#version 330 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal; 

uniform mat4 modelMat; 
uniform mat4 viewMat; 
uniform mat4 projMat; 

// out vec4 ourColor;
out vec2 TexCoord;
out vec3 FragPos; 
out vec3 Normal; 

void main()
{
    gl_Position =  projMat * viewMat * modelMat * vec4(aPos, 1.0);
    Normal = mat3(transpose(inverse(modelMat))) * aNormal; 
    FragPos = (modelMat * vec4(aPos, 1.0)).xyz;
    // ourColor = vec4(aColor, 1.0);
    TexCoord = aTexCoord; 
}