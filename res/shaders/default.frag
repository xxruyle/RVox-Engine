#version 330 core
struct Material {
	// vec3 ambient; 
	// vec3 diffuse; 
	samplerCube diffuse; 
	samplerCube specular; 
	// vec3 specular; 
	float shininess; 
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
in vec3 Color; 

// uniform vec3 voxelColor;


// uniforms for lighting 
uniform vec3 viewPos; 

uniform DirLight dirLight; 
uniform PointLight pointLights[NR_POINT_LIGHTS]; 
uniform SpotLight spotLight; 

uniform Material material; 


// fog values 
const float fogDensity = 0.0025; 
const float fogGradient = 2.0; 
uniform float renderDistance = 600; 


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir); 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  
vec3 CalcSpotLight(SpotLight spotlight, vec3 normal, vec3 fragpos, vec3 viewDir); 
float CalcFog(vec3 cameraPos, float renderDistance); 



void main()
{
	vec4 skyColor = vec4(255.0f/255.0f, 193.0f/255.0f, 142.0f/255.0f, 1.0f);  

    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: Point lights
/*     for(int i = 0; i < NR_POINT_LIGHTS; i++)
    	result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);     */
    // phase 3: Spot light
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir); 
	float visibility = CalcFog(viewPos, renderDistance); 
    // FragColor = vec4(Color, 0.0) * vec4(result, 1.0); //vec4 sun color  
	FragColor = vec4(75.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f, 0.0) * vec4(result, 1.0);
	FragColor = mix(skyColor, FragColor, visibility); 
	
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
/*     vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord)); */

    vec3 ambient  = light.ambient;  
    vec3 diffuse  = light.diffuse  * diff; 
    // vec3 specular = light.specular * spec; 

    // return (ambient + diffuse + specular);
	return (ambient + diffuse); 
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
    // vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoord));
    // vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoord));
    // vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	vec3 ambient  = light.ambient;
	vec3 diffuse  = light.diffuse  * diff;
	// vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    // specular *= attenuation;
    // return (ambient + diffuse + specular);
	return (ambient + diffuse); 
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

	// vec3 ambient  = spotlight.ambient  * vec3(texture(material.diffuse, TexCoord));
	// vec3 diffuse  = spotlight.diffuse  * diff * vec3(texture(material.diffuse, TexCoord));
    // vec3 specular = spotlight.specular * spec * vec3(texture(material.specular, TexCoord));

	vec3 ambient  = spotlight.ambient;
	vec3 diffuse  = spotlight.diffuse  * diff;
    vec3 specular = spotlight.specular * spec; 

	diffuse *= intensity; 
	specular *= intensity;

	// return (ambient + diffuse + specular);
	return (ambient + diffuse); 
}

float CalcFog(vec3 cameraPos, float renderDistance)    
{
	float distanceFromCamera =  length(FragPos - cameraPos);       

	float visibility = exp(-pow((distanceFromCamera*fogDensity), fogGradient));   
	visibility = clamp(visibility, 0.0, 1.0); 
	return visibility; 
}