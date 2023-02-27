#version 330 core

in vec2 UV;
in vec4 fragmentColor;

uniform sampler2D sampler;
uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
//uniform float weight[5] = float[](0.427027, 0.3945946, 0.3216216, 0.254054, 0.216216);
out vec4 color;

void main()
{
    vec4 col = texture(sampler, UV) * fragmentColor;

    float brightness = dot(col.xyz, vec3(0.2126, 0.7152, 0.0722));
    if(brightness < -1.0) // all gauss
        color = vec4(0.0, 0.0, 0.0, 1.0);
    else
    {
        vec2 tex_offset = 1.0 / textureSize(sampler, 0);
        vec3 result = texture(sampler, UV).xyz * fragmentColor.xyz * weight[0]; 
        
        for(int i = 1; i < 5; ++i)
        {
            result += texture(sampler, UV + vec2(tex_offset.x * i, 0.0)).rgb * fragmentColor.xyz * weight[i];
            result += texture(sampler, UV - vec2(tex_offset.x * i, 0.0)).rgb * fragmentColor.xyz * weight[i];
        }
        for(int i = 1; i < 5; ++i)
        {
            result += texture(sampler, UV + vec2(0.0, tex_offset.y * i)).rgb * fragmentColor.xyz * weight[i];
            result += texture(sampler, UV - vec2(0.0, tex_offset.y * i)).rgb * fragmentColor.xyz * weight[i];
        }
        
        color = vec4(result, 1.0);
    }
    if (all(lessThan(color, vec4(0.1, 0.1, 0.1, 1.1))))
            discard;
}