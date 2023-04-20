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

## First GLM Operations Notes 
- This code translates the object and makes it go in a circle while rotating 
```C++ 
// rotating, translating and scaling 
glm::mat4 trans = glm::mat4(1.0f); 
trans = glm::translate(trans, glm::vec3(5.0f*sin((float)glfwGetTime()), 5.0f*cos((float)glfwGetTime()), 0.0f)); 
trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0)); 
trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5)); 
```