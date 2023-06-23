#pragma once 
#include <unordered_map> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>

struct Voxel {
    std::unordered_map<char, glm::vec3> voxelColors = {
        {1, glm::vec3(85.0f/255.0f, 128.0f/255.0f, 89.0f/255.0f)}, // grass   
        // {1, glm::vec3(124.0f/255.0f, 252.0f/255.0f, 0.0f/255.0f)}, // grass 
        {2, glm::vec3(130.0f/255.0f, 136.0f/255.0f, 134.0f/255.0f)}, // stone
        {3, glm::vec3(236.0f/255.0f, 255.0f/255.0f, 253.0f/255.0f)}, // snow 
        {4, glm::vec3(247.0f/255.0f, 196.0f/255.0f, 105.0f/255.0f)}, // dirt     
    }; 

/*     glm::vec3 calculateColor(int height)  
    {
        
    } */
};
