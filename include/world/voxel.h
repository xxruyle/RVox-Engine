#pragma once 
#include <unordered_map> 
#include <glad/glad.h>  
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>

#include <noise/FastNoiseLite.h> 


struct Voxel {
    std::unordered_map<char, glm::vec3> voxelColors = {
        {1, glm::vec3(85.0f/255.0f, 128.0f/255.0f, 89.0f/255.0f)}, // grass   
        // {1, glm::vec3(124.0f/255.0f, 252.0f/255.0f, 0.0f/255.0f)}, // grass 
        {2, glm::vec3(130.0f/255.0f, 136.0f/255.0f, 134.0f/255.0f)}, // stone
        {3, glm::vec3(236.0f/255.0f, 255.0f/255.0f, 253.0f/255.0f)}, // snow 
        {4, glm::vec3(247.0f/255.0f, 196.0f/255.0f, 105.0f/255.0f)}, // dirt     
        {5, glm::vec3(179.0f/255.0f, 19.0f/255.0f, 18.0f/255.0f)}, // ore      
    }; 

    glm::vec3 grassGradients[6] = {
        glm::vec3(85.0f/255.0f, 128.0f/255.0f, 89.0f/255.0f),     
        glm::vec3(85.0f/255.0f, 132.0f/255.0f, 89.0f/255.0f),    
        glm::vec3(85.0f/255.0f, 136.0f/255.0f, 89.0f/255.0f),      
        glm::vec3(86.0f/255.0f, 140.0f/255.0f, 89.0f/255.0f),     
        glm::vec3(86.0f/255.0f, 144.0f/255.0f, 88.0f/255.0f),  
        glm::vec3(86.0f/255.0f, 148.0f/255.0f, 88.0f/255.0f),      
    }; 

    glm::vec3 calculateColor(char voxelType, int x, int z) { 
        if (voxelType == 1) {
            return calculateGrassNoise(x, z); 
        } else {
            return voxelColors[voxelType]; 
        }
    }


    glm::vec3 calculateGrassNoise(int x, int z)          
    {   
        FastNoiseLite noise;  
        noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);   
        noise.SetFractalType(FastNoiseLite::FractalType_FBm);    
        noise.SetFractalOctaves(8);       
        noise.SetFrequency(.009);          
        noise.SetSeed(1337);    

        float noiseColor = noise.GetNoise((float)(x), (float)(z));      

        if (noiseColor > 0.4) {
            return grassGradients[5];   
        } else if (noiseColor > 0.2) { 
            return grassGradients[4]; 
        } else if (noiseColor > 0) {
            return grassGradients[3]; 
        } else if (noiseColor > -0.2) { 
            return grassGradients[2]; 
        } else if (noiseColor > -0.4) {
            return grassGradients[1]; 
        } else {
            return grassGradients[0]; 
        }
    }
};
