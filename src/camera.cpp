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

void Camera::processInput(GLFWwindow* window, float deltaTime) 
{
    float velocity = mSpeed * deltaTime; 

    // fps movement 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // forward 
        mPosition += velocity * mPlayerForward; 

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // backward 
        mPosition -= velocity * mPlayerForward; 
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // left 
        mPosition -= mRight * velocity; 
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // right 
        mPosition += mRight * velocity; 
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) // jumping 
        mPosition += mWorldUp * velocity;
    
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) // jumping 
        mPosition -= mWorldUp * velocity;
     

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





void Camera::processMouseMovement(double xoffset, double yoffset) 
{
    xoffset *= mSensitivity; 
    yoffset *= mSensitivity; 


    mYaw += xoffset; 
    mPitch += yoffset; 

    if (mPitch > 89.0f) 
        mPitch = 89.0f; 
    if (mPitch < -89.0f)
        mPitch = -89.0f; 

    updateDirectionVectors(); 
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(mPosition, mPosition + mFront, mUp);  
}

glm::mat4 Camera::getPlayerViewMatrix(Player& player) 
{
    mPosition = player.mPosition - glm::vec3(0, -player.playerModel->ModelBoundingBox.startmaxY, 5); // updating camera position 
    glm::vec3 lookTarget = glm::vec3((player.playerModel->ModelBoundingBox.minX + player.playerModel->ModelBoundingBox.maxX) / 2, player.playerModel->ModelBoundingBox.maxY, (player.playerModel->ModelBoundingBox.minZ + player.playerModel->ModelBoundingBox.maxZ) / 2);      
    return glm::lookAt(player.mPosition - glm::vec3(0, -player.playerModel->ModelBoundingBox.startmaxY, 5), lookTarget, mUp);       
}

glm::mat4 Camera::getProjectionMatrix()
{
    return glm::perspective(glm::radians(mFov), (float)(SCR_WIDTH/SCR_HEIGHT), nearD, farD);  
}

void Camera::zoom(double xoffset, double yoffset)
{
    mFov -= yoffset;
    if (mFov < 1.0f)
        mFov = 1.0f;  

    if (mFov > 90.0f) 
        mFov = 90.0f; 
}







