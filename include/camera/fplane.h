#pragma once 
#include <iostream>
#include <cmath>


#include <algorithm>
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class FPlane
{ // plane for camera frustum  
public: 
    glm::vec3 p0; 
    glm::vec3 p1; 
    glm::vec3 p2;

    glm::vec3 u; 
    glm::vec3 v; 

    glm::vec3 normal; 
    double D; 
    FPlane(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2) : p0(p0), p1(p1), p2(p2) {
        u = p2 - p0; 
        v = p1 - p0; 
        normal = glm::normalize(glm::cross(v, u));  
        D = glm::dot(-normal, p0); 
    };    


    int distance(glm::vec3 point)
    {
        return glm::dot(normal, point) + D; 
    }

};