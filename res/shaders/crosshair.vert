#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoord;

uniform mat4 cModel; 
uniform mat4 cProjection; 


void main()
{
	TexCoord = vertex.zw;
	gl_Position = cProjection * cModel * vec4(vertex.xy, 0.0, 1.0);
}