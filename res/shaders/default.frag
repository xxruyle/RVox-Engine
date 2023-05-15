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

struct DirLight {
	vec3 color; 
	vec3 direction; 
	vec3 ambient; 
	vec3 diffuse; 
	vec3 specular; 
};

struct PointLight {
	vec3 position; 
	float constant; 
	float linear;
	float quadratic; 

	vec3 ambient; 
	vec3 diffuse; 
	vec3 specular; 
};

struct SpotLight {
	vec3 position; 
	vec3 direction; 

	vec3 ambient; 
	vec3 diffuse; 
	vec3 specular; 

	float cutOff; 
	float outerCutOff; 
}; 

#define NR_POINT_LIGHTS 4
out vec4 FragColor;

in vec3 TexCoord;

in vec3 Normal; 
in vec3 FragPos; 

// texture sample for cube map
// uniform samplerCube cubeMap; 

uniform vec3 voxelColor; 

// uniforms for lighting 
uniform vec3 viewPos; 

uniform DirLight dirLight; 
uniform PointLight pointLights[NR_POINT_LIGHTS]; 
uniform SpotLight spotLight; 

uniform Material material; 
uniform Light light; 


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir); 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  
vec3 CalcSpotLight(SpotLight spotlight, vec3 normal, vec3 fragpos, vec3 viewDir); 



void main()
{

/* 	// ambient 
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

/* 	vec3 result = specular + ambient + diffuse;  
	FragColor = vec4(result, 1.0);   */

	// FragColor = texture(cubeMap, TexCoord) * vec4(result, 1.0); // for textures 
	// FragColor = vec4(voxelColor, 0.0) * vec4(result, 1.0); */

    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: Point lights
     for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // phase 3: Spot light
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);     
    
    FragColor = vec4(result, 1.0); //vec4 sun color  
	
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight spotlight, vec3 normal, vec3 fragpos, vec3 viewDir) 
{
	vec3 lightDir = normalize(spotlight.position - fragpos); 


	float diff = max(dot(normal, lightDir), 0.0);


	vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);


	float theta = dot(lightDir, normalize(-spotlight.direction));   
	float epsilon = spotlight.cutOff - spotlight.outerCutOff; 
	float intensity = clamp((theta - spotlight.outerCutOff)/epsilon, 0.0, 1.0); 

	vec3 ambient  = spotlight.ambient  * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse  = spotlight.diffuse  * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = spotlight.specular * spec * vec3(texture(material.specular, TexCoord));

	diffuse *= intensity; 
	specular *= intensity;

	return (ambient + diffuse + specular);
}