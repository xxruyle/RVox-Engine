#pragma once 
#include "shader/shader.h" 
#include "mesh/texture_mesh.h" 


class DebugTools
{
public: 

    DebugTools()
    {
        getOutlineVertices(); 
    }

    void getOutlineVertices();  
    void drawVoxelOutline(Shader& shader, glm::vec3 position);  

private: 


    std::vector<TextureMesh> meshes;  

    // vertices for cube and texcoords  
    std::vector<glm::vec3> backFace = {
        // Back face
        glm::vec3(-0.5f, -0.5f, 0.5f),  
        glm::vec3(0.5f,  0.5f, 0.5f),  
        glm::vec3(0.5f, -0.5f, 0.5f),  
        glm::vec3(-0.5f,  0.5f, 0.5f) 
    }; 

    std::vector<glm::vec2> backTEX = {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
    }; 

    std::vector<unsigned int> backIndices = {
        2, 1, 0, 
        1, 3, 0 
    }; 

    std::vector<glm::vec3> frontFace = {
        // Front face
        glm::vec3(-0.5f, -0.5f,  -0.5f),
        glm::vec3(0.5f, -0.5f,  -0.5f),
        glm::vec3(0.5f,  0.5f,  -0.5f),
        glm::vec3(-0.5f,  0.5f,  -0.5f),
    }; 

    std::vector<glm::vec2> frontTEX = {
        glm::vec2(0.0f, 0.0f),  
        glm::vec2(1.0f, 0.0f),  
        glm::vec2(1.0f, 1.0f),  
        glm::vec2(0.0f, 1.0f),  
    }; 

    std::vector<unsigned int> frontIndices = {
        0, 2, 1, 
        0, 3, 2 
    }; 

    std::vector<glm::vec3> leftFace = { 
        // Left face
        glm::vec3(0.5f,  0.5f,  0.5f),
        glm::vec3(0.5f,  0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f,  0.5f),
    };

    std::vector<glm::vec2> leftTEX = {
        glm::vec2(1.0f, 0.0f), 
        glm::vec2(1.0f, 1.0f), 
        glm::vec2(0.0f, 1.0f), 
        glm::vec2(0.0f, 0.0f), 
    }; 

    std::vector<unsigned int> leftIndices = {
        3, 1, 0, 
        3, 2, 1, 
    }; 

    std::vector<glm::vec3> rightFace = {
        // Right face
        glm::vec3(-0.5f,  0.5f,  0.5f), 
        glm::vec3(-0.5f, -0.5f, -0.5f), 
        glm::vec3(-0.5f,  0.5f, -0.5f),      
        glm::vec3(-0.5f, -0.5f,  0.5f)  
    };

    std::vector<glm::vec2> rightTEX = {
        glm::vec2(1.0f, 0.0f), 
        glm::vec2(0.0f, 1.0f), 
        glm::vec2(1.0f, 1.0f), 
        glm::vec2(0.0f, 0.0f), 
    }; 

    std::vector<unsigned int> rightIndices = {
        0, 2, 1,
        0, 1, 3 
    }; 


    std::vector<glm::vec3> bottomFace = {
        // Bottom face
        glm::vec3(-0.5f, -0.5f, -0.5f), 
        glm::vec3(0.5f, -0.5f, -0.5f), 
        glm::vec3(0.5f, -0.5f,  0.5f), 
        glm::vec3(-0.5f, -0.5f,  0.5f), 
    };

    std::vector<glm::vec2> bottomTEX = {
        glm::vec2(0.0f, 1.0f),  
        glm::vec2(1.0f, 1.0f),  
        glm::vec2(1.0f, 0.0f),  
        glm::vec2(0.0f, 0.0f),  
    }; 

    std::vector<unsigned int> bottomIndices = {
        0, 1, 2, 
        0, 2, 3 
    }; 

    std::vector<glm::vec3> topFace = {
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3(0.5f,  0.5f,  0.5f),
        glm::vec3(0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f)
    };

    std::vector<glm::vec2> topTEX = { 
        glm::vec2(0.0f, 1.0f),  
        glm::vec2(1.0f, 0.0f),  
        glm::vec2(1.0f, 1.0f),  
        glm::vec2(0.0f, 0.0f),  
    }; 

    std::vector<unsigned int> topIndices = {
        1, 2, 0,
        3, 1, 0
    }; 
}; 