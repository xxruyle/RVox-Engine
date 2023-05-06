#include <camera/camera.h>

Camera::Camera(glm::vec3 position, glm::vec3 front, float yaw, float pitch, float fov, float speed, float sensitivity, World* world)
{
    mPosition = position; 
    mFront = front; 
    mYaw = yaw; 
    mPitch = pitch; 
    mFov = fov; 
    mSpeed = speed; 
    mSensitivity = sensitivity; 
    mWorld = world; 
    updateDirectionVectors(); 
}

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
        if (mSpeed > 50.0f) 
            mSpeed = 50.0f;
    }
    else
    {
        mSpeed = 10.0f; 
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) // resetting the fov
        mFov = 90.0f; 
}


glm::vec3 Camera::getVoxelFace(glm::vec3 origin, float xray, float yray, float zray)
{
    std::cout << origin.x - xray << " " << origin.y - yray << " " << origin.z - zray <<  std::endl;
    float maxFromOrigin[3] = {std::abs(origin.x - xray), std::abs(origin.y - yray), std::abs(origin.z - zray)}; 
    float max = -1000;  // max starts with min outside of the range we are testing for 
    int maxIndex = 0;
    for (int j = 0; j < 3; j++) // loop over 3 times and find max value 
    {
        if (maxFromOrigin[j] > max) 
        {
            std::cout << maxFromOrigin[j] << std::endl; 
            max = maxFromOrigin[j];
            maxIndex = j;
        } 
    }
    std::cout << max << std::endl; 
    int axisDirection = 1.0f; // decides which side a block will be plac
    if (maxIndex == 0) {
        (origin.x - xray) > 0 ? axisDirection *= -1.0f : axisDirection = 1.0f; 
        return glm::vec3(origin.x + axisDirection, origin.y, origin.z); 
    } else if (maxIndex == 1) {
        (origin.y - yray) > 0 ? axisDirection *= -1.0f : axisDirection = 1.0f;
        return glm::vec3(origin.x, origin.y + axisDirection, origin.z);
    } else { // if maxIndex == 3
        (origin.z - zray) > 0 ? axisDirection *= -1.0f : axisDirection = 1.0f;
        return glm::vec3(origin.x, origin.y, origin.z + axisDirection); 
    } 
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

void Camera::zoom(double xoffset, double yoffset)
{
    mFov -= yoffset;
    if (mFov < 1.0f)
        mFov = 1.0f;  

    if (mFov > 90.0f) 
        mFov = 90.0f; 
}





