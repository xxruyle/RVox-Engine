#version 330 core
struct Material {
	// vec3 ambient; 
	// vec3 diffuse; 
	samplerCube diffuse; 
	samplerCube specular; 
	// vec3 specular; 
	float shininess; 
}; 

struct Light {

	vec3 direction; 
	vec3 position;  

	float cutOff; 

	vec3 ambient; 
	vec3 diffuse; 
	vec3 specular; 

	float constant; 
	float linear; 
	float quadratic; 

	float outerCutOff;
	float innerCutoff;
}; 

out vec4 FragColor;

in vec3 TexCoord;

in vec3 Normal; 
in vec3 FragPos; 

// texture sample for cube map
// uniform samplerCube cubeMap; 

uniform vec3 voxelColor; 


// uniforms for lighting 
uniform vec3 viewPos; 

// bools to set which type of lighting 
uniform bool spotlight;  // basically just bools
uniform bool sunlight;  // basically just bools
uniform bool dirAttenutation;  // basically just bools

uniform Material material; 
uniform Light light; 


void main()
{

	// ambient 
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));    

	// diffuse 
	vec3 norm = normalize(Normal);

	// vec3 lightDir = normalize(light.position - FragPos); //  for light caster directional lighting (e.g spotlights)
	vec3 lightDir = normalize(-light.direction); // sunlight  

	float diff = max(dot(norm, lightDir), 0.0); // dot product of lightDir unit vector and normal
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));     


	// specular 
	vec3 viewDir = normalize(viewPos - FragPos); 
	vec3 reflectDir = reflect(-lightDir, norm); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // the last parameter is the 'shininess' make sure it is odd so that the both sides bug doesn't happen 
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));    


	if (spotlight) // spotlight (soft edges with intensity and epsilon)
	{
		float theta = dot(lightDir, normalize(-light.direction));   
		float epsilon = light.cutOff - light.outerCutOff; 
		float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0); 
		diffuse *= intensity; 
		specular *= intensity; 
	}

	// for light caster attenuation   
/*  	float distance = length(light.position - FragPos); 
	float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * (distance*distance)); 
	ambient *= attenuation;   
	diffuse  *= attenuation;  
	specular *= attenuation;    */

	vec3 result = specular + ambient + diffuse;  
	FragColor = vec4(result, 1.0);  

	// FragColor = texture(cubeMap, TexCoord) * vec4(result, 1.0); // for textures 
	// FragColor = vec4(voxelColor, 0.0) * vec4(result, 1.0);
}