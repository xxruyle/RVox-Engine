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
    void sunLightInit(Shader& shader, Camera& camera);  
    void setLightSource(Shader& shader, glm::vec3 position, int i);  
    void spotLightInit(Shader& shader, Camera& camera);   
}; 