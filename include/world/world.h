#pragma once 
#include <iostream> 
#include <cmath> 
#include <functional> 
#include <vector> 
#include <string> 
#include <unordered_map> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <noise/FastNoiseLite.h>


#define GLM_ENABLE_EXPERIMENTAL // enables hashing of glm::vec3 
#include "glm/gtx/hash.hpp"


class World
{
public: 
    int wSizeX = 1; 
    int wSizeZ = 1; 

    std::unordered_map<glm::vec3, int> worldMap; 
    

}; 