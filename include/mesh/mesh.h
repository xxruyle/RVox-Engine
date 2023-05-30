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




struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal; 
    glm::vec3 Color; 
}; 


class Mesh
{
public: 
    std::vector<Vertex> vertices; 
    std::vector<unsigned int> indices; 


    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);     


    void Draw(Shader& shader);  

private: 
    unsigned int VAO, VBO, EBO; 

    void setupMesh();
    void setupTextureMesh(); 
 

}; 