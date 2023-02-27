#version 330 core
layout (location = 3) in vec3 aPos;

out vec3 TexCoords;
out vec3 FragPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    FragPos = aPos;
    gl_Position = pos.xyww; 
}  

