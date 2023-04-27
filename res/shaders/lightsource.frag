#version 330 core 
out vec4 FragColor;

in vec2 LightTexCoord; 

uniform sampler2D lightTexture; 

void main() 
{
    FragColor = texture(lightTexture, LightTexCoord) * vec4(1.0);
}