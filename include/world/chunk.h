#pragma once 
#include <iostream> 
#include <cmath> 
#include <vector> 
#include <string> 
#include <map> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <noise/FastNoiseLite.h>

#include "voxel.h"

class Chunk 
{
public: 
    glm::vec3 position; 
    std::vector<Voxel> voxels; 

    void generate(int randSeed, int startX, int startZ); // generates a chunk based on chunk coordinate in world  
    void generateSolidChunk(int randSeed, int startX, int startZ); 

    void printVoxelSize() 
    {
        std::cout << voxels.size() << std::endl;  
    }
}; 
