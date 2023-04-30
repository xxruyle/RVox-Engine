#version 330 core 
out vec4 FragColor;

in vec3 LightTexCoord; 
uniform samplerCube lightCubeMap; 

uniform vec3 lightColor; 

void main() 
{
    FragColor = texture(lightCubeMap, LightTexCoord) * vec4(lightColor, 1.0);
}