#include "world/light.h"

void Light::setupDirLight(Shader& shader, Camera& camera)
{

    shader.setInt("material.diffuse", 0); 
    
    // shader.setVec3("dirLight.direction", cos(0.5f*glfwGetTime()), sin(0.5f*glfwGetTime()), 0.0);     
    shader.setVec3("dirLight.direction", 0.0f, -1.0, 0.0);       
    shader.setVec3("dirLight.color", 250.0f/255.0f, 150.0f/255.0f, 27.0f/255.0f); 
    shader.setVec3("viewPos", camera.mPosition.x, camera.mPosition.y, camera.mPosition.z);  
    shader.setVec3("dirLight.ambient",  0.5f, 0.5f, 0.5f);       
    shader.setVec3("dirLight.diffuse",  0.4f, 0.4f, 0.4f);      
}


void Light::setupSunlight(Shader& shader, PLYModel& sun, Player& player)   
{
    shader.setVec3("SunLight.position", sun.mPosition.x, sun.mPosition.y, sun.mPosition.z); 
}


