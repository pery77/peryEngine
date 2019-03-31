#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform float     threshold = 0.5;
uniform float     power     = 1.0;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec4 source = texture(texture0, vec2(fragTexCoord.s , 1 - fragTexCoord.t));
	vec4 brightColor;

	float brightness = dot(source.rgb, vec3(0.2126, 0.7152, 0.0722));
    
	if(brightness > threshold)
        brightColor = vec4(source.rgb, 1.0);
    else
        brightColor = vec4(0.0, 0.0, 0.0, 0.0);


	finalColor = brightColor * power;

}