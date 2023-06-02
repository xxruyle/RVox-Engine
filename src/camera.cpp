#include <camera/camera.h>

void Camera::updateDirectionVectors() 
{
    mDirection.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch)); 
    mDirection.y = sin(glm::radians(mPitch)); 
    mDirection.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch)); 

    mFront = glm::normalize(mDirection);  // the front face direction of the camera
    mRight = glm::normalize(glm::cross(mFront, mWorldUp)); 
    mUp = glm::normalize(glm::cross(mRight, mFront)); 
    mTarget = glm::normalize(glm::cross(mUp, mRight));


    mPlayerForward = glm::normalize(glm::cross(mWorldUp, mRight));  // the vector pointing along the x axis orthogonal to the y and camera's right direction  

}


void Camera::processInput(GLFWwindow* window, float deltaTime, Player& player)  
{
    if (!isThirdPerson)
    {
         float velocity = mSpeed * deltaTime; 

        // fps movement 
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // forward 
        {
            mPosition += velocity * mPlayerForward;  
            // player.mPosition += velocity * mPlayerForward; 
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // backward 
        {
            mPosition -= velocity * mPlayerForward;  
            // player.mPosition -= velocity * mPlayerForward; 
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // left 
        {
            mPosition -= mRight * velocity;  
            // player.mPosition -= mRight * velocity; 
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // right 
        {
            mPosition += mRight * velocity;  
            // player.mPosition += mRight * velocity; 
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) // jumping 
        {
            mPosition += mWorldUp * velocity; 
            // player.mPosition += mWorldUp * velocity; 
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) // jumping 
        {
            mPosition -= mWorldUp * velocity; 
            // player.mPosition -= mWorldUp * velocity; 
        }


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
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) // resetting the fov
            mFov = 90.0f;  
    }


        
    if (isThirdPerson) 
    {
        player.move(window, deltaTime); 
        updateThirdPersonInfo(player);  
    }
}


float Camera::calculateHorizontalDistance() 
{
    return distanceFromPlayer * cos(glm::radians(mPitch));  
}

float Camera::calculateVerticalDistance() 
{
    return distanceFromPlayer * sin(glm::radians(mPitch)); 
}

void Camera::calculatePosition(float horizDistance, float vertDistance, Player& player)  
{
    float theta = angleAroundPlayer;
    float offsetX = horizDistance * sin(glm::radians(theta));    
    float offsetZ = horizDistance * cos(glm::radians(theta));     
    mPosition.x = player.mPosition.x - offsetX;   
    mPosition.z = player.mPosition.z - offsetZ;   
    mPosition.y = player.mPosition.y + vertDistance;  
}

void Camera::updateThirdPersonInfo(Player& player)  
{
    player.mRight = mRight; 
    player.mForward = mPlayerForward; 
    float horizontalDistance = calculateHorizontalDistance(); 
    float verticalDistance = calculateVerticalDistance(); 
    calculatePosition(horizontalDistance, verticalDistance, player); 
}



void Camera::processMouseMovement(double xoffset, double yoffset) 
{
    xoffset *= mSensitivity; 
    yoffset *= mSensitivity; 

    // first person 
    if (!isThirdPerson)
    {
        mYaw += xoffset;   
        mPitch += yoffset;
    }   
    else {
        mYaw += xoffset;    
        angleAroundPlayer -= xoffset; 
        mPitch -= yoffset;  
    }
        

    if (mPitch > 87.0f) 
        mPitch = 87.0f; 
    if (mPitch < -87.0f)
        mPitch = -87.0f; 

    updateDirectionVectors();   
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(mPosition, mPosition + mFront, mUp);  
}

glm::mat4 Camera::getPlayerViewMatrix(Player& player) 
{
    if (!isThirdPerson) 
        return glm::lookAt(mPosition, mPosition + mFront, mUp); 

    glm::vec3 playerPosition = glm::vec3((player.playerModel->ModelBoundingBox.minX + player.playerModel->ModelBoundingBox.maxX) / 2, player.playerModel->ModelBoundingBox.maxY, (player.playerModel->ModelBoundingBox.minZ + player.playerModel->ModelBoundingBox.maxZ) / 2);       
    glm::vec3 lookTarget = glm::normalize((playerPosition - mPosition)); 
    return glm::lookAt(mPosition, mPosition + lookTarget, mWorldUp);
}

glm::mat4 Camera::getProjectionMatrix()
{
    return glm::perspective(glm::radians(mFov), (float)(SCR_WIDTH/SCR_HEIGHT), nearD, farD);  
}

void Camera::zoom(double xoffset, double yoffset)
{
/*     mFov -= yoffset;
    if (mFov < 1.0f)
        mFov = 1.0f;  

    if (mFov > 90.0f) 
        mFov = 90.0f;  */
    distanceFromPlayer -= yoffset * 1.5;  

    if (distanceFromPlayer < 5) 
        distanceFromPlayer = 5;

    if (distanceFromPlayer > 45) 
        distanceFromPlayer = 45; 
}

void Camera::changeViewPerson() 
{
    if (isThirdPerson) 
    {
        isThirdPerson = false; 
    }
    else 
        isThirdPerson = true; 
}







