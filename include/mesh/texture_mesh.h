#pragma once 
#include <iostream> 
#include <vector> 
#include <string> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "texture/texture.h" 
#include "shader/shader.h" 






struct vertexTEX {
    glm::vec3 Position; 
    glm::vec2 TexCoord; 
};


class TextureMesh
{
public: 
    Texture texture;  
    std::vector<vertexTEX> vertices; 
    std::vector<unsigned int> indices; 


    TextureMesh(std::vector<vertexTEX> vertices, std::vector<unsigned int> indices, const char* textureFilePath);         


    void Draw(Shader& shader); 

private: 
    unsigned int VAO, VBO, EBO; 

    void setupTextureMesh(); 

}; 