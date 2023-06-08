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
    glm::vec3 aoValue;   
}; 


class Chunk
{ // stores local chunk voxel positions and chunk mesh 
public: 
    bool renderable = false; // only renderable if all neighboring chunks are renderable 
    glm::vec3 position; 
    char voxels[33][256][33];  // the voxel array     

    glm::vec3 heighestLocation; 

    std::vector<voxelVertex> vertices; // for the chunk mesh    
    std::vector<unsigned int> indices;
    

    void generateSolidChunk(int randSeed, int startX, int startZ); 
    void generateDebugChunk(int randSeed, int startX, int startZ);   

    void getVoxel(glm::vec3 coordinate);  


    // pointers to neighboring chunks 
    Chunk* mLeft;  
    Chunk* mRight;  
    Chunk* mUp;  
    Chunk* mBot;  

    void mesh(); // checks each block for the chunk to see and assigns them to be interior accordingly.   
    void draw(Shader& shader, Frustum& frustum, float renderDistance); // draw the chunk    
    
private: 
    unsigned int VAO, VBO, EBO;
    void setupMesh(); 
    glm::vec3 backAO(glm::vec3 vPos, unsigned int vertexIndex); // gets the ao value for the particular vertex  
    glm::vec3 frontAO(glm::vec3 vPos, unsigned int vertexIndex);
    glm::vec3 leftAO(glm::vec3 vPos, unsigned int vertexIndex);
    glm::vec3 rightAO(glm::vec3 vPos, unsigned int vertexIndex); 
    glm::vec3 bottomAO(glm::vec3 vPos, unsigned int vertexIndex); 
    glm::vec3 topAO(glm::vec3 vPos, unsigned int vertexIndex);   
    int getAOSides(char side1, char side2, char corner); // takes 3 different voxels[][][] coordinate positions depending on vertex index  
    glm::vec3 getAOValue(int numSides); //takes in num of sides and returns an AO dark color value  

 

std::vector<glm::vec3> backFace = {
    // Back face
    glm::vec3(0.5f,  0.5f, 0.5f),  
    glm::vec3(-0.5f,  0.5f, 0.5f),
    glm::vec3(-0.5f, -0.5f, 0.5f),  
    glm::vec3(0.5f, -0.5f, 0.5f),  
}; 


std::vector<unsigned int> backIndices = {
    0, 1, 2, 
    0, 2, 3
}; 

std::vector<glm::vec3> backNormals = {
    glm::vec3(0.0f,  0.0f, -1.0f), 
    glm::vec3(0.0f,  0.0f, -1.0f), 
    glm::vec3(0.0f,  0.0f, -1.0f), 
    glm::vec3(0.0f,  0.0f, -1.0f), 
}; 

std::vector<glm::vec3> frontFace = {
    // Front face
    glm::vec3(-0.5f,  0.5f,  -0.5f),
    glm::vec3(0.5f,  0.5f,  -0.5f),
    glm::vec3(0.5f, -0.5f,  -0.5f),
    glm::vec3(-0.5f, -0.5f,  -0.5f),
}; 

std::vector<unsigned int> frontIndices = {
    0, 1, 2, 
    0, 2, 3 
}; 

std::vector<glm::vec3> frontNormals = {
    glm::vec3(0.0f,  0.0f, 1.0f),  
    glm::vec3(0.0f,  0.0f, 1.0f),  
    glm::vec3(0.0f,  0.0f, 1.0f),  
    glm::vec3(0.0f,  0.0f, 1.0f),  
};

std::vector<glm::vec3> leftFace = { 
    // Left face
    glm::vec3(0.5f,  0.5f, -0.5f),
    glm::vec3(0.5f,  0.5f,  0.5f),
    glm::vec3(0.5f, -0.5f,  0.5f),
    glm::vec3(0.5f, -0.5f, -0.5f),
};

std::vector<unsigned int> leftIndices = {
    0, 1, 2, 
    0, 2, 3
}; 

std::vector<glm::vec3> leftNormals = {
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
}; 

std::vector<glm::vec3> rightFace = {
    // Right face
    glm::vec3(-0.5f,  0.5f,  0.5f), 
    glm::vec3(-0.5f,  0.5f, -0.5f),       
    glm::vec3(-0.5f, -0.5f, -0.5f), 
    glm::vec3(-0.5f, -0.5f,  0.5f)  
};

std::vector<unsigned int> rightIndices = {
    0, 1, 2, 
    0, 2, 3
}; 

std::vector<glm::vec3> rightNormals = {
    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),

}; 

std::vector<glm::vec3> bottomFace = {
    // Bottom face
    glm::vec3(-0.5f, -0.5f, -0.5f), 
    glm::vec3(0.5f, -0.5f, -0.5f), 
    glm::vec3(0.5f, -0.5f,  0.5f), 
    glm::vec3(-0.5f, -0.5f,  0.5f), 
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

}; 

std::vector<glm::vec3> topFace = {
    // Top face
    glm::vec3(-0.5f,  0.5f,  0.5f),
    glm::vec3(0.5f,  0.5f,  0.5f),
    glm::vec3(0.5f,  0.5f, -0.5f),
    glm::vec3(-0.5f,  0.5f, -0.5f),

};

std::vector<unsigned int> topIndices = {
    0, 1, 2, 
    0, 2, 3, 
}; 

std::vector<glm::vec3> topNormals = {
    glm::vec3(0.0f,  1.0f,  0.0f),
    glm::vec3(0.0f,  1.0f,  0.0f),
    glm::vec3(0.0f,  1.0f,  0.0f),
    glm::vec3(0.0f,  1.0f,  0.0f),
}; 
}; 
