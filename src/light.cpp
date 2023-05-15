#include "world/light.h"

void Light::sunLightInit(Shader& shader, Camera& camera)
{

    shader.setInt("material.diffuse", 0); 
    shader.setInt("material.specular", 1);

    shader.setVec3("dirLight.direction", 0.0f, -1.0, 0.0);   
    shader.setVec3("dirLight.color", 250.0f/255.0f, 150.0f/255.0f, 27.0f/255.0f); 
    shader.setVec3("viewPos", camera.mPosition.x, camera.mPosition.y, camera.mPosition.z);  
    shader.setVec3("dirLight.ambient",  0.1f, 0.1f, 0.1f);   
    shader.setVec3("dirLight.diffuse",  0.8f, 0.8f, 0.8f);  
    shader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);  

    shader.setFloat("material.shininess", 32.0f); 

}

void Light::setLightSource(Shader& shader, glm::vec3 position, int i)
{
    std::string index = std::to_string(i); 

    shader.setVec3(("pointLights[" + index + "].position").c_str(), position.x, position.y, position.z); 
    shader.setFloat(("pointLights[" + index + "].constant").c_str(), 1.0f); 
    shader.setFloat(("pointLights[" + index + "].linear").c_str(), .045f); 
    shader.setFloat(("pointLights[" + index + "].quadratic").c_str(), .0075f); 

    
    shader.setVec3(("pointLights[" + index + "].ambient").c_str(),  0.2f, 0.2f, 0.2f);   
    shader.setVec3(("pointLights[" + index + "].diffuse").c_str(),  0.5f, 0.5f, 0.5f);   
    shader.setVec3(("pointLights[" + index + "].specular").c_str(), 1.0f, 1.0f, 1.0f);  
}

void Light::spotLightInit(Shader& shader,  Camera& camera) 
{ // gives out a spotlight from the camera front 
    shader.setVec3("spotLight.position", camera.mPosition.x, camera.mPosition.y, camera.mPosition.z);  
    shader.setVec3("spotLight.direction", camera.mFront.x, camera.mFront.y, camera.mFront.z);  
    shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f))); 
    shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f))); 

    shader.setVec3("spotLight.ambient",  0.1f, 0.1f, 0.1f);    
    shader.setVec3("spotLight.diffuse",  0.8f, 0.8f, 0.8f);   
    shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);   
}

