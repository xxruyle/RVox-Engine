#pragma once 
#include <glad/glad.h> // include glad before glfw3
//#include <GLEW/glew.h>  // include glew before glfw3
#include <GLFW/glfw3.h> 
#include <string> 
#include <fstream> 
#include <sstream> 
#include <cerrno> 


std::string get_file_contents(const char* filename); 

class Shader
{
    public: 
        GLuint ID; 
        Shader(const char* vertexFile, const char* fragmentFile);  
        
        void Activate(); 
        void Delete(); 

        void setBool(const std::string &name, bool value) const; 
        void setInt(const std::string &name, int value) const;  
        void setFloat(const std::string &name, float value) const;

};



