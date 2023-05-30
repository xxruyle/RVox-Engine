#include "player/player.h"


void Player::move(GLFWwindow* window, float deltaTime)     
{
    float velocity = mSpeed * deltaTime;  

    // fps movement 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // forward 
        mPosition += velocity * glm::vec3(0, 0, 1);  

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // backward 
        mPosition -= velocity * glm::vec3(0, 0, 1);  
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // left 
        mPosition -= glm::vec3(1, 0, 0) * velocity;  
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // right 
        mPosition += glm::vec3(1, 0, 0) * velocity; 

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) // going up  
        mPosition += glm::vec3(0, 1, 0) * velocity; 
    
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) // going down  
        mPosition -= glm::vec3(0, 1, 0) * velocity; 


    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // "sprint"
    {
        mSpeed += 1.0f; 
        if (mSpeed > 90.0f) 
            mSpeed = 90.0f;
    }
    else
    {
        mSpeed = 10.0f; 
    }



    playerModel->mPosition = mPosition; 


    playerModel->ModelBoundingBox.translateValues(mPosition);   
    playerModel->ModelBoundingBox.calculateVertices();  
}