#version 330 core
struct DirLight {
	vec3 color; 
	vec3 direction; 
	vec3 ambient; 
	vec3 diffuse; 
};

out vec4 FragColor;

in vec3 TexCoord;
in vec3 Normal; 
in vec3 FragPos; 
in vec3 Color; 
in vec3 AOValue;    
in vec4 FragPosLightSpace; 

// uniform vec3 voxelColor;

// uniforms for lighting 
uniform vec3 viewPos; 
uniform DirLight dirLight; 

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
vec3 CalcIntensity(DirLight light, vec3 voxelColor); 
float CalcAmbientOcclusion();   
float CalcFog(vec3 cameraPos);  
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
 */


	float occlusion = CalcAmbientOcclusion();    
	vec4 occlusionTint = vec4(occlusion, occlusion, occlusion, 1.0);    

	// regular lighting  output 
	FragColor = vec4(Color, 1.0) * vec4(result, 1.0); //vec4 sun   color           
 	FragColor = mix(skyColor, FragColor, visibility);   
	FragColor = FragColor * vec4(AOValue, 1.0);  


	// calculate shadow
/*     float shadow = ShadowCalculation(FragPosLightSpace);       

    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse)) * Color;       
    FragColor = vec4(lighting, 1.0);  */
	
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.4);   
    // specular shading
	
    // combine results
    vec3 ambient  = light.ambient;  
    vec3 diffuse  = light.diffuse * diff; 

	return (ambient + diffuse); 
}

vec3 CalcIntensity(DirLight light, vec3 voxelColor)  
{
	vec3 sunPosition = normalize(-light.direction); 
	float sunlightIntensity = max(sunPosition.y * 0.96 + 0.6, 0.0);   

	return voxelColor * sunlightIntensity; 
}


float CalcFog(vec3 cameraPos)     
{
	float distanceFromCamera =  length(FragPos - cameraPos);       

	float visibility = exp(-pow((distanceFromCamera*fogDensity), fogGradient));   
	visibility = clamp(visibility, 0.0, 1.0); 
	return visibility; 
}


float CalcAmbientOcclusion()  
{
	if (AOValue.x == 0)
	{
		return 0.1;   
	}
	else if (AOValue.x == 2)  
	{
		return 0.4; 
	} 
	else if (AOValue.x == 3)  
	{
		return 0.8; 
	}
	else 
	{
		return 1.0; 
	}

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