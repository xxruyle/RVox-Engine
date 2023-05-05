#pragma once 
#include <iostream> 
#include <cmath> 
#include <functional> 
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
#include "texture/texture.h"



class World
{
public: 
    std::vector<glm::vec3> positions;  
    std::vector<Voxel> voxelVec;  

    std::map<std::string, Texture> voxelTextures;

    void generateLand(int randSeed, bool printHeights); 
    void generateSingle(); 
    void generatePlane(); 
    void printCubes(); 

}; 