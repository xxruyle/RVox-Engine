#pragma once 
#include "shader/shader.h" 
#include "mesh/texture_mesh.h" 

struct BoundingBox
{
    float startminX; // the value that it started with 
    float minX;  // the value that it currently is in relation to the world 


    float startmaxX; 
    float maxX; 


    float startminZ;  
    float minZ; 

    float startmaxZ; 
    float maxZ; 


    float startminY; 
    float minY; 

    float startmaxY; 
    float maxY; 

    glm::vec3 bottomFrontLeft;
    glm::vec3 bottomFrontRight;
    glm::vec3 bottomBackLeft;
    glm::vec3 bottomBackRight;
    glm::vec3 topFrontLeft;
    glm::vec3 topFrontRight;
    glm::vec3 topBackLeft;
    glm::vec3 topBackRight; 

    void translateValues(glm::vec3 position)
    {
        minX = position.x + startminX;  
        maxX = position.x + startmaxX;   

        minY = position.y + startminY; 
        maxY = position.y + startmaxY; 

        minZ = position.z + startminZ; 
        maxZ = position.z + startmaxZ; 
    }

    void calculateVertices() 
    {
        bottomFrontLeft   = glm::vec3(minX, minY, minZ);  
        bottomFrontRight  = glm::vec3(maxX, minY, minZ);   
        bottomBackLeft    = glm::vec3(minX, minY, maxZ);  
        bottomBackRight   = glm::vec3(maxX, minY, maxZ);  
        topFrontLeft      = glm::vec3(minX, maxY, minZ);  
        topFrontRight     = glm::vec3(maxX, maxY, minZ);  
        topBackLeft       = glm::vec3(minX, maxY, maxZ);   
        topBackRight      = glm::vec3(maxX, maxY, maxZ);   
    }
}; 


class DebugTools
{
public: 

    DebugTools()
    {
        // getOutlineVertices(); 
    }

    void getOutlineVertices();  
    void getWireFrameVertices(); 
    void getBoundingBoxVertices(BoundingBox& boundingBox);   
    void drawVoxelOutline(Shader& shader, glm::vec3 position, glm::vec3 scale);
    void DrawBoundingBox(Shader& shader, glm::vec3 position);  

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

    std::vector<glm::vec3> backFacewire = {
        // Back face
        glm::vec3(-0.5f, -0.5f, 0.5f),  
        glm::vec3(0.5f,  1.0f, 0.5f),  
        glm::vec3(0.5f, -0.5f, 0.5f),  
        glm::vec3(-0.5f,  1.0f, 0.5f) 
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

    std::vector<glm::vec3> frontFaceWire = {
        // Front face
        glm::vec3(-0.5f, -0.5f,  -0.5f),
        glm::vec3(0.5f, -0.5f,  -0.5f),
        glm::vec3(0.5f,  1.0f,  -0.5f),
        glm::vec3(-0.5f,  1.0f,  -0.5f),
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

    std::vector<glm::vec3> leftFaceWire = { 
        // Left face
        glm::vec3(0.5f,  1.0f,  0.5f),
        glm::vec3(0.5f,  1.0f, -0.5f),
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

    std::vector<glm::vec3> rightFaceWire = {
        // Right face
        glm::vec3(-0.5f,  1.0f,  0.5f), 
        glm::vec3(-0.5f, -0.5f, -0.5f), 
        glm::vec3(-0.5f,  1.0f, -0.5f),      
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

    std::vector<glm::vec3> topFaceWire = {
        glm::vec3(-0.5f,  1.0f, -0.5f),
        glm::vec3(0.5f,  1.0f,  0.5f),
        glm::vec3(0.5f,  1.0f, -0.5f),
        glm::vec3(-0.5f,  1.0f,  0.5f)
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