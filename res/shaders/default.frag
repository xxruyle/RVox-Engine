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
in vec4 FragPosLightSpace; 

// uniform vec3 voxelColor;

// uniforms for lighting 
uniform vec3 viewPos; 
uniform DirLight dirLight; 
uniform PointLight pointLights[NR_POINT_LIGHTS]; 
uniform SpotLight spotLight; 
uniform Material material; 

uniform sampler2D shadowMap; 
uniform vec3 lightPos; 


// fog values 
// const float fogDensity = 0.0025; // for 1000 block render distance 
// const float fogDensity = 0.005; // for 500 block render distance 
const float fogDensity = .0025;  
float fogGradient = 3.0; 

// frustum values for depth buffer 
float nearD = 0.1; 
float farD = 1000.0; 


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir); 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  
vec3 CalcSpotLight(SpotLight spotlight, vec3 normal, vec3 fragpos, vec3 viewDir); 
float CalcFog(vec3 cameraPos);  
float LinearizeDepth(float depth); 
float ShadowCalculation(vec4 fragPosLightSpace);   


void main()
{
	vec4 skyColor = vec4(255.0f/255.0f, 193.0f/255.0f, 142.0f/255.0f, 1.0f);  

    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

	float visibility = CalcFog(viewPos);   

	// shadow stuff 
/*     vec3 normal = normalize(Normal); 
    vec3 lightColor = vec3(1.0);
    // ambient
    vec3 ambient = 0.15 * lightColor;
    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos);  // lightpos - FragPos  
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // calculate shadow
    float shadow = ShadowCalculation(FragPosLightSpace);   */    

    // vec3 lighting = (ambient + (1.0 - shadow) * (diffuse)) * Color;      
    // FragColor = vec4(lighting, 1.0);


	// regular lighting  output 
	FragColor = mix(skyColor, FragColor, visibility);  
	FragColor = vec4(Color, 0.0) * vec4(result, 1.0); //vec4 sun color      
	
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

float CalcFog(vec3 cameraPos)     
{
	float distanceFromCamera =  length(FragPos - cameraPos);       

	float visibility = exp(-pow((distanceFromCamera*fogDensity), fogGradient));   
	visibility = clamp(visibility, 0.0, 1.0); 
	return visibility; 
}

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; 
	return (2.0 * nearD * farD) / (farD + nearD - z * (farD - nearD)); 
}

float ShadowCalculation(vec4 fragPosLightSpace)   
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow

	vec3 lightDir = normalize(lightPos - FragPos);  

	float bias = max(0.05 * (1.0 - dot(Normal, lightDir)), 0.005);   

	float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  

    return shadow;
}