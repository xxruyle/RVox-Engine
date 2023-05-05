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
private: 
    const int EDGE_SIZE = 64; // 64x64 chunk lengthxlength  
    const int AREA = EDGE_SIZE * EDGE_SIZE;  // area of a chunk 

public: 
    glm::vec3 position; 
    std::vector<Voxel> voxels; 

    void set(int localX, int localY, int localZ, Voxel voxel); // sets the current voxel given certain position   

    void get(int localX, int localY, int localZ); // gets the previously set voxel at a current position 


}; 
