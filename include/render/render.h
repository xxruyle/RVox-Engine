#pragma once 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera/camera.h" 
#include "shader/shader.h"

class Render
{
private: 
    float SCR_WIDTH = 1400; 
    float SCR_HEIGHT = 1000; 

    glm::mat4 view; 
    glm::mat4 projection; 

public:

    void viewProject(Camera& camera);   
    void setShaders(Shader& shader); 

    void drawVoxel(Shader& shader, glm::vec3 position); 


}; 