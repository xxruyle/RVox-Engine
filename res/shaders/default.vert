#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;  
layout (location = 2) in vec3 aColor; 
layout (location = 3) in vec3 aoValue;    

out vec3 TexCoord;
out vec3 FragPos; 
out vec3 Normal; 
out vec3 Color; 
out vec3 AOValue;    
// out vec4 FragPosLightSpace; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// uniform mat4 lightSpaceMatrix; 


void main()
{
	TexCoord = aPos; 
	FragPos = vec3(model * vec4(aPos, 1.0)); 
	Normal = transpose(inverse(mat3(model))) * aNormal;      
	// FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0); 
	Color = aColor; 
	AOValue = aoValue; 
	gl_Position = projection * view  * vec4(FragPos, 1.0);    // regular 
	// gl_Position = projection * view * model * vec4(aPos, 1.0); // shadowMap 
}