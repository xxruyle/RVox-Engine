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
        Texture(); // initialize the texture object 
        void Bind(); // bind the texture 
        void setParameters(); // set the texture wrapping/filtering options
        void Generate(const char* file_string, int width, int height); // load and generate the texture 

}; 