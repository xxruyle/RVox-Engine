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
#include "render/render.h" 



class Hud 
{
public: 
    Render& renderer; 

    Hud(Render& renderer) : renderer(renderer) {}; 
    void crossHairInit(const char* textureFilePath);   

    void DrawCrosshair(Shader& shader);   



private: 
    unsigned int VAO, VBO; 
    Texture texture; 
    void setupMesh();    
}; 