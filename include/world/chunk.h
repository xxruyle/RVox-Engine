#pragma once 
#include <iostream> 
#include <cmath> 
#include <vector> 
#include <string> 
#include <unordered_map> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <noise/FastNoiseLite.h>

#include "voxel.h"

#define GLM_ENABLE_EXPERIMENTAL // enables hashing of glm::vec3 
#include "glm/gtx/hash.hpp"


class Chunk 
{
public: 
    glm::vec3 position; 
    std::vector<Voxel> voxels; 
    std::unordered_map<glm::vec3, Voxel> voxelMap; 

    void generate(int randSeed, int startX, int startZ); // generates a chunk based on chunk coordinate in world  
    void generateSolidChunk(int randSeed, int startX, int startZ); 

    void generateDebugChunk(); 

    void checkInteriorVoxels(); // checks each block for the chunk to see and assigns them to be interior accordingly. 


    void printVoxelSize() 
    {
        std::cout << voxels.size() << std::endl;  
    }

private: 
    void getVoxelNeighbors(); // gets the neighbors of the voxels 
}; 
