#include "world/light.h"

void Light::spotLightInit(Shader& shader,  Camera& camera) 
{ // gives out a spotlight from the camera front 
    shader.setBool("spotlight", true);   
    shader.setBool("dirAttenuation", false);  
    shader.setBool("sunlight", false);   

    shader.setVec3("light.position", camera.mPosition.x, camera.mPosition.y, camera.mPosition.z);  
    shader.setVec3("light.direction", camera.mFront.x, camera.mFront.y, camera.mFront.z);  
    shader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f))); 
    shader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f))); 
    shader.setVec3("viewPos", camera.mPosition.x, camera.mPosition.y, camera.mPosition.z);  

    shader.setVec3("light.ambient",  0.1f, 0.1f, 0.1f);   
    shader.setVec3("light.diffuse",  0.8f, 0.8f, 0.8f);  
    shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);  

    shader.setFloat("light.constant",  1.0f); 
    shader.setFloat("light.linear",    0.09f); 
    shader.setFloat("light.quadratic", 0.032f); 

    shader.setFloat("material.shininess", 32.0f);   
}

void Light::sunLightInit(Shader& shader, Camera& camera) 
{
    shader.setBool("sunlight", true); 
    shader.setBool("spotlight", false);  
    shader.setBool("dirAttenuation", false); 


    shader.setVec3("light.direction", 0.0f, -1.0f, 0.0f);   
 
    shader.setVec3("viewPos", camera.mPosition.x, camera.mPosition.y, camera.mPosition.z);  

    shader.setVec3("light.ambient",  0.1f, 0.1f, 0.1f);   
    shader.setVec3("light.diffuse",  0.8f, 0.8f, 0.8f);  
    shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);  
    shader.setFloat("material.shininess", 32.0f); 

    shader.setFloat("light.constant",  1.0f); 
    shader.setFloat("light.linear",    0.09f); 
    shader.setFloat("light.quadratic", 0.032f);  
}

void Light::sourceLightInit(Shader& shader, Camera& camera, glm::vec3 lightPosition)   
{
    shader.setBool("sunlight", false); 
    shader.setBool("spotlight", false);  
    shader.setBool("dirAttenuation", true);  



    shader.setVec3("light.position", lightPosition.x, lightPosition.y, lightPosition.z);    
    // shader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);   

 
    shader.setVec3("viewPos", camera.mPosition.x, camera.mPosition.y, camera.mPosition.z);  

    shader.setVec3("light.ambient",  0.2f, 0.2f, 0.2f);   
    shader.setVec3("light.diffuse",  0.5f, 0.5f, 0.5f);   
    shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);  
    
    shader.setFloat("light.constant",  1.0f); 
    shader.setFloat("light.linear",    0.09f); 
    shader.setFloat("light.quadratic", 0.032f); 
    shader.setFloat("material.shininess", 32.0f); 
}