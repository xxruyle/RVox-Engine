#version 330 core 
in vec2 TexCoord;
out vec4 FragColor;

// texture samplers
uniform sampler2D crosshairSprite;


void main()
{
	FragColor = texture(crosshairSprite, TexCoord);
}