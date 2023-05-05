#pragma once 
#include <iostream> 
#include <cmath> 
#include <vector> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture/texture.h"




class Voxel 
{
// The voxel object which stores the world coordinates and texture of the voxel 
public: 
    glm::vec3 coordinates;  
    glm::vec3 color;

    Voxel(glm::vec3 coordinates, glm::vec3 color) : coordinates(coordinates), color(color) {}; 
}; 
