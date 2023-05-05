#version 330 core
out vec4 FragColor;

in vec3 TexCoord;

in vec3 Normal; 
in vec3 FragPos; 

// texture sample for cube map
uniform samplerCube cubeMap; 

uniform vec3 voxelColor; 


// uniforms for lighting 
uniform vec3 lightPos; 

uniform vec3 objectColor; 
uniform vec3 lightColor; 
uniform vec3 viewPos; 



void main()
{
	// ambient 
	float ambientStrength = 0.5; 
	vec3 ambient = ambientStrength * lightColor; 

	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); 
	float diff = max(dot(norm, lightDir), 0.0); // dot product of lightDir unit vector and normal
	vec3 diffuse = diff * lightColor; 


	// specular 
	float specularStrength = 0.5; 

	vec3 viewDir = normalize(viewPos - FragPos); 
	vec3 reflectDir = reflect(-lightDir, norm); 

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 13); // the last parameter is the 'shininess' make sure it is odd so that the both sides bug doesn't happen 
	vec3 specular = specularStrength * spec * lightColor; 

	// putting it all together  
	vec3 result = (specular + ambient + diffuse) * objectColor; 

	// FragColor = texture(cubeMap, TexCoord) * vec4(result, 1.0); // for textures 
	FragColor = vec4(voxelColor, 0.0) * vec4(result, 1.0);
}