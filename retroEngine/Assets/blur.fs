#version 330 core

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;  
uniform float horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
uniform float force = 1;

void main()
{       

    vec2 tex_offset = 1.0 / textureSize(texture0, 0); // gets size of single texel
    vec3 result = texture(texture0, fragTexCoord).rgb * weight[0]; // current fragment's contribution

    if(horizontal == 0.0)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(texture0, fragTexCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(texture0, fragTexCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(texture0, fragTexCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(texture0, fragTexCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }

    finalColor = vec4(result, 1.0) * force;

}