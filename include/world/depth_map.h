#pragma once  
#include <iostream> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include "shader/shader.h" 


class DepthMap 
{
public: 
    unsigned int depthMapFBO; 
    unsigned int depthMap;  

    unsigned int quadVAO = 0; 
    unsigned int quadVBO;  


    glm::mat4 lightSpaceMatrix; 
    DepthMap(); 
    void Render(Shader& shader, glm::vec3 lightPos, glm::vec3 target);   
    void bindTexture();
    void DebugRender(Shader& shader); 
    void ConfigureShaderAndMatrices(Shader& shader, glm::vec3 lightPos, glm::vec3 target); 

    void Unbind(); 


private: 

    const unsigned int SHADOW_WIDTH = 1300, SHADOW_HEIGHT = 1000; // the screen space resolution  
    float nearPlane = 0.1f, farPlane = 100.0f;     


}; 