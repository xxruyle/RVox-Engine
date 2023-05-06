#version 330 core 
in vec2 TexCoord;
out vec4 FragColor;

// texture samplers
uniform sampler2D crosshairSprite;


void main()
{
	FragColor = texture(crosshairSprite, TexCoord);
	if (FragColor.rgb == vec3(0,0,0)) discard; // to make the black parts transparent 
}