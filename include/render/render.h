#pragma once 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera/camera.h" 
#include "shader/shader.h"
#include "texture/texture.h" 

class Render
{
private: 
    // cube map faces texture file paths 


    float SCR_WIDTH; 
    float SCR_HEIGHT; 

    glm::mat4 view; 
    glm::mat4 projection; 


public:
    Render(float window_width, float window_height) : SCR_WIDTH(window_width), SCR_HEIGHT(window_height) {};


    
    glm::mat4 model; 

    void viewProject(Camera& camera);   
    void setShaders(Shader& shader); 

    void drawVoxelCubeMap(Shader& shader, Texture& texture, glm::vec3 position, float scale); 
    void drawVoxel(Shader& shader, glm::vec3 position, glm::vec3 color, float scale); 
    void drawRotatingVoxel(Shader& shader, glm::vec3 position, float scale, float rotation, glm::vec3 axisRotations); 
    void draw2D(Shader& shader, glm::vec2 position, float scale); 

}; 