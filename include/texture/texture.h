#pragma once 
#include <glad/glad.h> // include glad before glfw3
//#include <GLEW/glew.h>  // include glew before glfw3
#include <GLFW/glfw3.h> 
#include <iostream> 
#include <string> 
#include <fstream> 
#include <sstream> 
#include <cerrno> 




class Texture 
{
    private: 
        void compileErrors(unsigned char* data); 

    public: 
        GLuint ID;  
        Texture(GLenum textureUnit); // initialize the texture object 
        void Bind(GLenum textureUnit); // bind the texture 
        void Unbind(); 
        void setParameters(GLint param); // set the texture wrapping/filtering options
        void Generate(const char* file_string, int width, int height, GLenum format, bool flip); // load and generate the texture 
        void Delete(); 
}; 