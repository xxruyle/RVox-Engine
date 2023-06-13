#pragma once 
#include <iostream> 
#include <cmath> 
#include <vector> 
#include <string> 
#include <future>  
#include <mutex> 
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
    bool startedGeneration = false; 
    bool startedMesh = false; 
    bool renderable = false; // only renderable if all neighboring chunks are renderable 
    glm::vec3 position; 
    char voxelArray[262144];  // the flat voxel array 32*32*256


    std::vector<voxelVertex> vertices; // for the chunk mesh    
    std::vector<unsigned int> indices;

    // futures for multithreading compeletion checks   
    std::future<void> generateFuture; 
    std::future<void> meshFuture; 

    std::mutex vLock; 


    void generate(int randSeed);  
    // void generateAndMesh(int randSeed, int startX, int startZ);  
    void threadedMesh(); 



    void generateSolidChunk(int randSeed, int startX, int startZ); 
    void generateDebugChunk(int randSeed, int startX, int startZ);   

    char getVoxel(glm::vec3 coordinate);   
    int getVoxelIndex(glm::vec3 coordinate);  // returns an int index to access the flat voxel array 
    bool isSolid(char voxelValue); 
    bool isAir(char voxelValue); 


    // pointers to neighboring chunks 
    Chunk* mLeft;  
    Chunk* mRight;  
    Chunk* mUp;  
    Chunk* mBot;  


    void mesh(); // checks each block for the chunk to see and assigns them to be interior accordingly.   
    void draw(Shader& shader, Frustum& frustum); // draw the chunk    

    void finishMeshing(); 
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
}; 
