#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;


float LinearizeDepth(float depth); 

uniform sampler2D depthMap;
uniform float nearD; 
uniform float farD; 

void main()
{             

/* 	float depth = LinearizeDepth(gl_FragCoord.z) / farD; // depth testing  
	FragColor = vec4(vec3(depth), 1.0); // depth buffer    */
    
    float depthValue = texture(depthMap, TexCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0);
}  

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; 
	return (2.0 * nearD * farD) / (farD + nearD - z * (farD - nearD)); 
}