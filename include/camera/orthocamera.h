#pragma once 
#include <iostream>
#include <cmath>

#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class OrthoCamera
{
private: 
    float SCR_WIDTH = 1300; 
    float SCR_HEIGHT = 1000; 
    glm::vec3 worldUp = glm::vec3(0.0, 1.0, 0.0);  

public:
    glm::vec3 position; 
    glm::vec3 direction; 
    float yaw; 
    float pitch; 
    float fov; 

    glm::vec3 front; 
    glm::vec3 right; 
    glm::vec3 up; 
    glm::vec3 forward; 

    OrthoCamera(glm::vec3 position, float yaw, float pitch, float fov) : position(position), yaw(yaw), pitch(pitch), fov(fov) {
        updateDirectionVectors();
    }; 

    void updateDirectionVectors(); 

    glm::mat4 getViewMatrix(); 

    glm::mat4 getOrthoMatrix(); 

}; 