#pragma once 
#include <iostream> 
#include <vector> 
#include <cmath> 
#include <string> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include "texture/texture.h" 
#include "shader/shader.h" 
#include "camera/camera.h"

class Light 
{
public: 
    glm::vec3 lightPos; 

    void setupDirLight(Shader& shader, Camera& camera);   
    void setupSunlight(Shader& shader, PLYModel& sun, Player& player);   


private: 
}; 