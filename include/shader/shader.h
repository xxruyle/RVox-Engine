#pragma once 
#include <glad/glad.h> // include glad before glfw3
//#include <GLEW/glew.h>  // include glew before glfw3
#include <GLFW/glfw3.h> 
#include <string> 
#include <fstream> 
#include <sstream> 
#include <cerrno> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



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
        void setMat(const std::string &name, int amount, GLboolean isTransposed, glm::mat4& trans) const; 
        void setVec3(const std::string &name, float x, float y, float z) const; 
        void setVec4(const std::string &name, const glm::vec4& value) const; 

};



