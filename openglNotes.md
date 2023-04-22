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


# Notes on General OpenGL Procedure so Far 
## Window 
1) glfwInit(); init glfw 
2) set glfwWindowHints, opengl3.3, openglcore profile 
3) Create window pointer with glfwCreateWindow()
4) Make the the current context the window 
5) Create glfwSetFramebufferSizeCallback function to set glViewPort()  
6) innit gladLoadGL()
7) enable depth buffer, glEnable(GL_DEPTH_TEST)
## VBO and VAO 
1) glGenVertexArrays() VAO 
2) bind VAO, glBindVertexArray() 
3) bind VBO 
4) glVertexAttribPointer()
5) glEnableVertexAttribArray() 
## Shader 
1) get source code for vertex and fragment shader and compile 
2) ID = glCreateProgram(); 
3) attach vertex and fragment shader to shader program
4) link vertex and fragment shader  
5) delete vertex and fragment shader 
6) Use shaderprogram, glUseProgram(ID) 
7) Create getUniformLocation() functions in shader class
## Textures 
1) use stb image to read in textures jpgs and pngs
2) glActiveTexture(GL_TEXTURE0) 
3) glGenTextures(1, &ID); 
4) Bind, glBindTexture(GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP, ID) 
5) Set parameters by, GL REPEAT OR GL Linear 
6) Generate texture by glTexImage2D() and glGenerateMipmap
7) remember to flip when using stbi

## 3D and Camera 
1) create view glm::lookat(), projection glm::perspective(), model matrix glm::translate().
2) setMat4 getUniformlocation for view, projection and model. 
3) Refer to glsl code, transform vec4 -> model -> view -> projection

## GLSL 
```GLSL
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
```