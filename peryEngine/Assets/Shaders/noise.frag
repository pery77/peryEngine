#version 330
float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float noise(vec2 p){
	vec2 ip = floor(p);
	vec2 u = fract(p);
	u = u*u*(3.0-2.0*u);
	
	float res = mix(
		mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
		mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
	return res*res;
}

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform float time;
uniform sampler2D texture0;

void main(void)
{
 // vec3 v = vec3(2.0 * fragTexCoord.xy * (2.0 + sin(0.5*time)));  
 // float n = simplexNoise3(v);

  vec2 perspectiveCorrection = vec2(2.0f * (0.5 - fragTexCoord.x  ) , fragTexCoord.y*3);

  vec2 v = vec2(10 * perspectiveCorrection.xy * (2.0 + sin(0.005*time )));  
  vec2 v2 = vec2(20 * perspectiveCorrection.xy * (2.0 + sin(0.005*time*0.5 ))); 
  float n = noise(v);
  float n2 = noise(v2);
  vec2 a = vec2(n,n2);
 // vec2 perspectiveCorrection = vec2(2.0f * (0.5 - fragTexCoord.x ) * fragTexCoord.y, 0.0f);
 // vec4 displacement = texture( texture0, fragTexCoord.xy + perspectiveCorrection );
 	
 vec2 adjusted = vec2(fragTexCoord.x + n,n2);
  
  finalColor =  texture( texture0, adjusted);

//  finalColor = fragColor * vec4(0.5 + 0.5 * vec3(n, n, n), 1.0);
 // finalColor = vec4(n,n2,0,1);
}