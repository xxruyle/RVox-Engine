#include "camera/orthocamera.h" 

void OrthoCamera::updateDirectionVectors()   
{
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)); 
    direction.y = sin(glm::radians(pitch)); 
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));  

    front = glm::normalize(direction);  // the front face direction of the camera 
    right = glm::normalize(glm::cross(front, worldUp));  
    up = glm::normalize(glm::cross(right, front)); 


    forward = glm::normalize(glm::cross(worldUp, right));  // the vector pointing along the x axis orthogonal to the y and camera's right direction 
}

glm::mat4 OrthoCamera::getViewMatrix()
{
    return glm::lookAt(glm::vec3(40.0f*cos(glfwGetTime()), 50.0f, 40.0f*sin(glfwGetTime())), glm::vec3(16.0f, 32.0f, 16.0f), up);    
}

glm::mat4 OrthoCamera::getOrthoMatrix() 
{
    return glm::ortho((float)-SCR_WIDTH/ 50.0f, (float)SCR_WIDTH/ 50.0f, (float)-SCR_HEIGHT / 50.0f, (float)SCR_HEIGHT / 50.0f , 0.0f, 200.0f);     
}