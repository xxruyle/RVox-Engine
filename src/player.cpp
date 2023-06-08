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
        calculateAngle(deltaTime); 
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // backward
    {
        currentSpeed = mSpeed; 
        velocityX -= mForward.x * currentSpeed;
        velocityZ -= mForward.z * currentSpeed; 
        calculateAngle(deltaTime); 

    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // left
    {
        currentSpeed = mSpeed; 
        velocityX -= mRight.x * currentSpeed;
        velocityZ -= mRight.z * currentSpeed; 
        calculateAngle(deltaTime); 

    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // right
    {
        currentSpeed = mSpeed; 
        velocityX += mRight.x * currentSpeed;
        velocityZ += mRight.z * currentSpeed; 
        calculateAngle(deltaTime); 

    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) // going down  
    {

            // currentSpeed = mSpeed; 
            // velocityY -= glm::vec3(0, 1, 0).y * currentSpeed; 
            velocityY = -1.2;    
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) // Jumping
    {
        if (onGround)    
        { 
            currentSpeed = mSpeed;  
            onGround = false; 
            velocityY = 0.3;   
            jumpCooldown = jumpCooldownDuration;  
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



    // if the player has taken a first movement 
    // we do this because the movement happens before the collision is updated 
    // not optimal but hey what can you do at this point there are tradeoffs with having the collision come first due to rendering problems 
    if (firstMove)  
    { 
        getJumpCooldown(deltaTime);   // calculate jumpcooldown 

        // calculate velocities 
        calculateVelocity(deltaTime); 
        mPosition += glm::vec3(velocityX, velocityY, velocityZ);  

        playerModel->mPosition = mPosition; 
    }

    sweptVelocity = (mPosition - prevPosition)  / deltaTime;  
    prevPosition = mPosition; 


    firstMove = true; 

    // recalculate AABB min and max data  
    playerModel->ModelBoundingBox.translateValues(mPosition);   
    playerModel->angle = currentAngle;  


} 

void Player::printVelocity()   
{
    // std::cout << velocityX << ' ' << velocityY << ' ' << velocityZ << std::endl;    
    std::cout << glm::to_string(sweptVelocity) << std::endl; 
}



void Player::calculateVelocity(float deltaTime)      
{
    limitVelocity(); // handle velocity being too high 
    
    // handling jumping limits and functionality 
    if (!onGround)       
        velocityY -= gravity * deltaTime; 
    else 
        velocityY *= deltaTime; 

    velocityX *= deltaTime; 
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
    else if (velocityY < -0.03)    
        velocityY = -0.03;     
}

void Player::calculateAngle(float deltaTime) 
{
    glm::vec2 worldVec = glm::vec2(0, 1); // The 2D vector pointing to the positive z direction
    glm::vec2 mDir = glm::vec2(velocityX, velocityZ); // The 2D vector normalized to the xy plane that is sent out from the 3D forward xyz  vector

    float angleRad = atan2(worldVec.y, worldVec.x) - atan2(mDir.y, mDir.x);
    if (angleRad < 0)
        angleRad += 2 * glm::pi<float>();

    float angleDeg = glm::degrees(angleRad);

    angle = angleDeg;

    interpolateAngle(deltaTime);  
}

void Player::interpolateAngle(float deltaTime)  
{ // interpolate angle so that there is gradual movement to the target angle 
     // Current angle (starting angle) starts with 0.0f and will be assigned angle at the end of the function 
    float targetAngle = angle; // Target angle (calculated previously)
    float duration = 0.1f; // Duration of the interpolation in seconds
    float elapsedTime = 0.0f; // Elapsed time since interpolation started

    // Calculate the difference between the angles
    float angleDifference = targetAngle - currentAngle;

    // Adjust the angle difference to take the shortest path
    if (angleDifference > 180.0f) {
        angleDifference -= 360.0f;
    } else if (angleDifference < -180.0f) {
        angleDifference += 360.0f;
    }

    elapsedTime += deltaTime; // deltaTime is the time since the last frame

    if (elapsedTime >= duration) {
        currentAngle = targetAngle; // Interpolation finished, set the angle to the target
    } else {
        float t = elapsedTime / duration; // Interpolation progress (between 0 and 1)
        float interpolatedAngle = currentAngle + angleDifference * t; // Interpolate between current and target angle
        currentAngle = glm::mod(interpolatedAngle, 360.0f); // Wrap the angle within the range [0, 360)
    }
}

void Player::getJumpCooldown(float deltaTime) 
{
    if (jumpCooldown > 0.0f)
    {
        jumpCooldown -= deltaTime; // Reduce the cooldown time
        if (jumpCooldown < 0.0f)
        {
            jumpCooldown = 0.0f; // Clamp the cooldown time to zero if it becomes negative
        }
    }
}