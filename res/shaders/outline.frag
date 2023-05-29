#version 330 core 
out vec4 FragColor;

in vec2 TexCoord;  
uniform sampler2D outlineTexture; 


void main() 
{

/*     if (texture(outlineTexture, TexCoord).a < 0.2) 
        discard;  */



    FragColor = texture(outlineTexture, TexCoord);   
}