## Shaders 
```C++
unsigned int shaderName = glCreateShader(GL_VERTEX_SHADER); 
glShaderSource(shaderName, 1, &shaderNameSource, NULL); 
glCompileShader(shaderName); 

unsigned int otherShaderName = glCreateShader(GL_VERTEX_SHADER); 
glShaderSource(otherShaderName, 1, &shaderNameSource, NULL); 
glCompileShader(otherShaderName); 

unsigned int shaderProgram = glCreatePorgram(); 
glAttachShader(shaderProgram, shaderName); 
glAttachShader(shaderProgram, otherShaderName);
glLinkProgram(shaderProgram); 

glDeleteShader(shaderName); 
glDeleteShader(otherShaderName); 
```