#pragma once 
#include <iostream> 
#include <cmath> 
#include <vector> 
#include <string> 
#include <unordered_map> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include "shader/shader.h" 



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <noise/FastNoiseLite.h>


#include "voxel.h" 
#include "mesh/mesh.h" 

#include "camera/frustum.h"  



#define GLM_ENABLE_EXPERIMENTAL // enables hashing of glm::vec3 
#include "glm/gtx/hash.hpp"

struct voxelVertex {
    glm::vec3 Position; 
    glm::vec3 Normal; 
    glm::vec3 Color;  
}; 


class Chunk
{ // stores local chunk voxel positions and chunk mesh 
public: 
    glm::vec3 position; 
    char voxels[33][256][33];  // the voxel array   
    // glm::vec3 voxelColors[33][256][33]; 


    std::vector<voxelVertex> vertices; // for the chunk mesh    
    std::vector<unsigned int> indices;
    

    void generateSolidChunk(int randSeed, int startX, int startZ); 
    void generateDebugChunk(int randSeed, int startX, int startZ);   
    void changeVoxelColor(glm::vec3 voxelCoord); 
    void addLightSource(); 


    void mesh(); // checks each block for the chunk to see and assigns them to be interior accordingly.   
    void draw(Shader& shader, Frustum& frustum, float renderDistance); // draw the chunk    
    
private: 
    unsigned int VAO, VBO, EBO;
    void setupMesh(); 
 

std::vector<glm::vec3> backFace = {
    // Back face
    glm::vec3(-0.5f, -0.5f, 0.5f),  
    glm::vec3(0.5f,  0.5f, 0.5f),  
    glm::vec3(0.5f, -0.5f, 0.5f),  
    /* glm::vec3(0.5f,  0.5f, -0.5f),  
    glm::vec3(-0.5f, -0.5f, -0.5f),  */
    glm::vec3(-0.5f,  0.5f, 0.5f) 
}; 


std::vector<unsigned int> backIndices = {
/*     0, 1, 2, 
    0, 3, 1  */
    2, 1, 0, 
    1, 3, 0 
}; 

std::vector<glm::vec3> backNormals = {
    glm::vec3(0.0f,  0.0f, -1.0f), 
    glm::vec3(0.0f,  0.0f, -1.0f), 
    glm::vec3(0.0f,  0.0f, -1.0f), 
    glm::vec3(0.0f,  0.0f, -1.0f), 
/*     glm::vec3(0.0f,  0.0f, -1.0f), 
    glm::vec3(0.0f,  0.0f, -1.0f) */
}; 

std::vector<glm::vec3> frontFace = {
    // Front face
    glm::vec3(-0.5f, -0.5f,  -0.5f),
    glm::vec3(0.5f, -0.5f,  -0.5f),
    glm::vec3(0.5f,  0.5f,  -0.5f),
    // glm::vec3(0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f,  -0.5f),
    // glm::vec3(-0.5f, -0.5f,  0.5f)
}; 

std::vector<unsigned int> frontIndices = {
    0, 2, 1, 
    0, 3, 2 
}; 

std::vector<glm::vec3> frontNormals = {
    glm::vec3(0.0f,  0.0f, 1.0f),  
    glm::vec3(0.0f,  0.0f, 1.0f),  
    glm::vec3(0.0f,  0.0f, 1.0f),  
    glm::vec3(0.0f,  0.0f, 1.0f),  
/*     glm::vec3(0.0f,  0.0f, 1.0f),  
    glm::vec3(0.0f,  0.0f, 1.0f)  */
};

std::vector<glm::vec3> leftFace = { 
    // Left face
    glm::vec3(0.5f,  0.5f,  0.5f),
    glm::vec3(0.5f,  0.5f, -0.5f),
    glm::vec3(0.5f, -0.5f, -0.5f),
    // glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3(0.5f, -0.5f,  0.5f),
    // glm::vec3(-0.5f,  0.5f,  0.5f)
};

std::vector<unsigned int> leftIndices = {
    3, 1, 0, 
    3, 2, 1, 
}; 

std::vector<glm::vec3> leftNormals = {
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
/*     glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f) */
}; 

std::vector<glm::vec3> rightFace = {
    // Right face
    glm::vec3(-0.5f,  0.5f,  0.5f), 
    glm::vec3(-0.5f, -0.5f, -0.5f), 
    glm::vec3(-0.5f,  0.5f, -0.5f),      
    // glm::vec3(0.5f, -0.5f, -0.5f), 
    // glm::vec3(0.5f,  0.5f,  0.5f), 
    glm::vec3(-0.5f, -0.5f,  0.5f)  
};

std::vector<unsigned int> rightIndices = {
    0, 2, 1,
    0, 1, 3 

}; 

std::vector<glm::vec3> rightNormals = {
    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),
/*     glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f) */
}; 

std::vector<glm::vec3> bottomFace = {
    // Bottom face
    glm::vec3(-0.5f, -0.5f, -0.5f), 
    glm::vec3(0.5f, -0.5f, -0.5f), 
    glm::vec3(0.5f, -0.5f,  0.5f), 
    // glm::vec3(0.5f, -0.5f,  0.5f), 
    glm::vec3(-0.5f, -0.5f,  0.5f), 
    // glm::vec3(-0.5f, -0.5f, -0.5f) 
};

std::vector<unsigned int> bottomIndices = {
    0, 1, 2, 
    0, 2, 3 
}; 

std::vector<glm::vec3> bottomNormals = {
    glm::vec3(0.0f, -1.0f,  0.0f),
    glm::vec3(0.0f, -1.0f,  0.0f),
    glm::vec3(0.0f, -1.0f,  0.0f),
    glm::vec3(0.0f, -1.0f,  0.0f),
/*     glm::vec3(0.0f, -1.0f,  0.0f),
    glm::vec3(0.0f, -1.0f,  0.0f)  */
}; 

std::vector<glm::vec3> topFace = {
    // Top face
    glm::vec3(-0.5f,  0.5f, -0.5f),
    glm::vec3(0.5f,  0.5f,  0.5f),
    glm::vec3(0.5f,  0.5f, -0.5f),
    // glm::vec3(0.5f,  0.5f,  0.5f),
    // glm::vec3(-0.5f,  0.5f, -0.5f),
    glm::vec3(-0.5f,  0.5f,  0.5f)
};

std::vector<unsigned int> topIndices = {
/*     0, 2, 1, 
    0, 1, 3  */
    1, 2, 0,
    3, 1, 0
}; 

std::vector<glm::vec3> topNormals = {
    glm::vec3(0.0f,  1.0f,  0.0f),
    glm::vec3(0.0f,  1.0f,  0.0f),
    glm::vec3(0.0f,  1.0f,  0.0f),
    glm::vec3(0.0f,  1.0f,  0.0f),
/*     glm::vec3(0.0f,  1.0f,  0.0f),
    glm::vec3(0.0f,  1.0f,  0.0f)  */
}; 

}; 
