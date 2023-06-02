#include "player/player.h"




void Player::move(GLFWwindow* window, float deltaTime)     
{
    velocityX = 0.0f; 
    velocityZ = 0.0f;  

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // forward
    {
        currentSpeed = mSpeed; 
        velocityX += mForward.x * currentSpeed;
        velocityZ += mForward.z * currentSpeed; 
        calculateAngle(); 

    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // backward
    {
        currentSpeed = mSpeed; 
        velocityX -= mForward.x * currentSpeed;
        velocityZ -= mForward.z * currentSpeed; 
        calculateAngle(); 

    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // left
    {
        currentSpeed = mSpeed; 
        velocityX -= mRight.x * currentSpeed;
        velocityZ -= mRight.z * currentSpeed; 
        calculateAngle(); 

    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // right
    {
        currentSpeed = mSpeed; 
        velocityX += mRight.x * currentSpeed;
        velocityZ += mRight.z * currentSpeed; 
        calculateAngle(); 

    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) // going down  
    {

            currentSpeed = mSpeed; 
            velocityY -= glm::vec3(0, 1, 0).y * currentSpeed * 0.01f;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) // going up 
    {
        if (onGround)
        {
            currentSpeed = mSpeed;  
            velocityY = 1.1f;     
            onGround = false; 
        }
    } 

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // "sprint"
    {
        mSpeed = 30.0f;   
    } else {
        mSpeed = 10.0f;  
    }

    if ((glfwGetKey(window, GLFW_KEY_W) != GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS) &&
        (glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS)) 
    {
        currentSpeed = 0.0f; 
        velocityX = 0.0f;
        velocityZ = 0.0f; 
    }

    
    // calculate velocities 
    calculateVelocity(deltaTime); 

    // if the player has taken a first movement 
    // we do this because the movement happens before the collision is updated 
    // not optimal but hey what can you do at this point there are tradeoffs with having the collision come first due to rendering problems 
    if (firstMove) 
    {
        // finally moving the player after velocities are calculated 
        mPosition += glm::vec3(velocityX, velocityY, velocityZ);     
        playerModel->mPosition = mPosition; 


        // recalculate AABB min and max data  
        playerModel->ModelBoundingBox.translateValues(mPosition);   
        playerModel->angle = angle; 
    }

    firstMove = true; 


} 

void Player::printVelocity()   
{
    std::cout << velocityX << ' ' << velocityY << ' ' << velocityZ << std::endl;    
    // std::cout << glm::to_string(calculateMaxVelocityVector()) << std::endl;   
}

void Player::calculateVelocity(float deltaTime)      
{
    limitVelocity(); // handle velocity being too high 

    velocityX *= deltaTime; 

    // handling jumping limits and functionality 
    if (!onGround) 
        velocityY -= gravity * deltaTime; 
    else 
        velocityY *= deltaTime; 



    velocityZ *= deltaTime; 
}

void Player::limitVelocity() 
{ // limit velocity if it is bigger or smaller than the velocity limit 
    
    if (velocityX > velocityLimit)  
        velocityX = velocityLimit; 
    else if (velocityX < -velocityLimit)  
        velocityX = -velocityLimit;   
    
    if (velocityZ > velocityLimit)  
        velocityZ = velocityLimit;  
    else if (velocityZ < -velocityLimit)  
        velocityZ = -velocityLimit;  

    if (velocityY > velocityLimit)  
        velocityY = velocityLimit;  
    else if (velocityY < -0.12)    
        velocityY = -0.12;   
}

void Player::calculateAngle() 
{
    glm::vec2 worldVec = glm::vec2(0, 1); // The 2D vector pointing to the positive z direction
    glm::vec2 mDir = glm::vec2(velocityX, velocityZ); // The 2D vector normalized to the xy plane that is sent out from the 3D forward xyz  vector

    float angleRad = atan2(worldVec.y, worldVec.x) - atan2(mDir.y, mDir.x);
    if (angleRad < 0)
        angleRad += 2 * glm::pi<float>();

    float angleDeg = glm::degrees(angleRad);

    angle = angleDeg;

    std::cout << angle << std::endl;

}