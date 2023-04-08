#include <iostream> 
#include <cmath> 
#include "shader/shader.h"

std::string get_file_contents(const char* filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in)
    {
      std::string contents;
      in.seekg(0, std::ios::end);
      contents.resize(in.tellg());
      in.seekg(0, std::ios::beg);
      in.read(&contents[0], contents.size());
      in.close();
      return(contents);
    }
    throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) 
{
    std::string vertexCode = get_file_contents(vertexFile); 
    std::string fragmentCode = get_file_contents(fragmentFile);     
    const char* vertexSource = vertexCode.c_str(); 
    const char* fragmentSource = fragmentCode.c_str();      
    // vertex shader 
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // creating a shader with type GL_VERTEX_SHADER since we are dealing with vertexes  
    glShaderSource(vertexShader, 1, &vertexSource, NULL); // attach shader source code. (shader object to compile, the number of strings were passing as source code, the source code of the vertex shader, NULL)
    glCompileShader(vertexShader); // compiling the shader      
    // fragment shader 
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL); // attach fragmentShaderSource to fragmentShader object 
    glCompileShader(fragmentShader); // compile fragmentShade     
    // Create shader program
    ID = glCreateProgram(); 
    glAttachShader(ID, vertexShader); // attach vertexShader to shaderProgram
    glAttachShader(ID, fragmentShader); // attach fragmentShader to shaderprogram 
    glLinkProgram(ID); // linking all the shaders together in the shader program      
    // deleting the shaders that we have already linked
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); 
}

void Shader::Activate() 
{
    glUseProgram(ID); 
    // updating the uniform color 
    float timeValue = glfwGetTime(); 
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f; 
    int vertexColorLocation = glGetUniformLocation(ID, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

}

void Shader::Delete() 
{
    glDeleteProgram(ID); 
}

