#version 330 core
out vec4 FragColor;

in vec3 TexCoord;

// texture sample for cube map
uniform samplerCube cubeMap; 

// light source color
uniform vec3 objectColor; 
uniform vec3 lightColor; 


void main()
{
	FragColor = texture(cubeMap, TexCoord) * vec4(lightColor * objectColor, 1.0); 
	// FragColor = vec4(lightColor * objectColor, 1.0);
}