#pragma once 
#include <iostream> 
#include <vector> 
#include <string> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "texture/texture.h" 
#include "shader/shader.h" 
#include "camera/camera.h"

class Light 
{

public: 
    void spotLightInit(Shader& shader, Camera& camera);   

    void sunLightInit(Shader& shader, Camera& camera); // enables sunlight;  

    void sourceLightInit(Shader& shader, Camera& camera, glm::vec3 lightPosition); // enables light source and attenuation 

}; 