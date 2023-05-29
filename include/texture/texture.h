#pragma once 
#include <glad/glad.h> // include glad before glfw3
//#include <GLEW/glew.h>  // include glew before glfw3
#include <GLFW/glfw3.h> 
#include <iostream> 
#include <vector> 
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
        // Texture(GLenum textureUnit); // initialize the texture object  
        void Init(GLenum textureUnit); 
        void Bind(GLenum textureType, GLenum textureUnit); // bind the texture 
        void Unbind(); 
        void Generate(const char* file_string, int width, int height, GLenum format, bool flip); // load and generate the texture 
        void GenerateCubeMap(std::vector<std::string> textures_faces, int width, int height, GLenum format, bool flip); 
        void Delete(); 
}; 