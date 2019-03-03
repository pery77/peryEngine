#version 330


// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform sampler2D displaceTexture;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

void main()
{
    // Texel color fetching from texture sampler

	vec2 perspectiveCorrection = vec2(2.0f * (0.5 - fragTexCoord.x) * fragTexCoord.y, 0.0f);
	vec4 displacement = texture( displaceTexture, fragTexCoord.xy + perspectiveCorrection );


    //vec4 displacement1 = texture( displace, fragTexCoord );

	vec2 adjusted = fragTexCoord.xy + (displacement.rg - 0.5);
    vec4 texelColor = texture(texture0, adjusted);
    // NOTE: Implement here your fragment shader code
    
    finalColor = texelColor * vec4(.8,.8,1,.9);
}