#pragma once 
#include <iostream> 
#include <cmath> 
#include <vector> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Voxel 
{
// The voxel object which stores the world coordinates and texture of the voxel 
public: 
    glm::vec3 coordinates; // world coordinates of the voxel  
    glm::vec3 color; // color of the voxel 
    int id; 
    glm::vec3 colorCopy; // copy of the voxel stored when outline color is assigned to the color 
    glm::vec3 outlineColor; // the outline color of the voxel, 

    bool isInterior; // determines whether block should be culled, if interior cull. 



    Voxel(glm::vec3 coordinates, glm::vec3 color,  int id) : coordinates(coordinates),  color(color), id(id), colorCopy(color) {
        outlineColor = glm::vec3(pow(color.x, 6), pow(color.y, 6), pow(color.z, 6)); 
    }; 

}; 
