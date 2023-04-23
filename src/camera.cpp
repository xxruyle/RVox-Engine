#include <camera/camera.h>

Camera::Camera(glm::vec3 position, glm::vec3 front, float yaw, float pitch, float fov, float speed, float sensitivity, Cube* c)
{
    mPosition = position; 
    mFront = front; 
    mYaw = yaw; 
    mPitch = pitch; 
    mFov = fov; 
    mSpeed = speed; 
    mSensitivity = sensitivity; 
    mC = c; 
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
    mTarget = mFront * 2.0f; 


    mPlayerForward = glm::normalize(glm::cross(mWorldUp, mRight));  // the vector pointing along the x axis orthogonal to the y and camera's right direction  

}

void Camera::processInput(GLFWwindow* window, float deltaTime) 
{
    float velocity = mSpeed * deltaTime; 
    // std::cout << mSpeed; 
    // player type fps movement 
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
        if (mSpeed > 30.0f) 
            mSpeed = 30.0f;
    }
    else
    {
        mSpeed = 10.0f; 
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) // resetting the fov
        mFov = 90.0f; 


}

glm::vec3 Camera::rayCast()  
{
    // std::cout << std::to_string(ceil(mPosition.x)) << " " << std::to_string(ceil(mPosition.y)) << " " << std::to_string(ceil(mPosition.z)) << std::endl; 

    for (float castLength = 1.0f; castLength < 5; castLength+=0.5) 
    {
        float xray = mPosition.x + (mFront.x * castLength); 
        float yray = mPosition.y + (mFront.y * castLength); 
        float zray = mPosition.z + (mFront.z * castLength); 
        glm::vec3 origin = glm::vec3((int)(xray), (int)(yray), (int)(zray)); // origin of the cube
        // std::cout << ray.x << " " << ray.y << " " << ray.z << std::endl; 
        for (unsigned int i = 0; i < mC->positions.size(); i++) 
        {
            if (mC->positions[i] == origin) // if there is a block in the world that matches the "collides"  with the raycast vector 
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

                int axisDirection = 1.0f; // decides which side a block will be placed

                if (maxIndex == 0) {
                    (origin.x - xray) > 0 ? axisDirection *= -1.0f : axisDirection = 1.0f; 
                    return glm::vec3(origin.x + axisDirection, origin.y, origin.z); 
                } else if (maxIndex == 1) {
                    (origin.y - yray) > 0 ? axisDirection *= -1.0f : axisDirection = 1.0f;
                    return glm::vec3(origin.x, origin.y + axisDirection, origin.z);
                } else if (maxIndex == 2) {
                    (origin.z - zray) > 0 ? axisDirection *= -1.0f : axisDirection = 1.0f;
                    return glm::vec3(origin.x, origin.y, origin.z + axisDirection); 
                }                
            }
        }
    }
    std::cout << "nope lol" << std::endl; 
    return glm::vec3(0.0f, 0.0f, 0.0f); 
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
    mFov -= (float)yoffset;
    if (mFov < 1.0f)
        mFov = 1.0f;  

    if (mFov > 90.0f) 
        mFov = 90.0f; 
}





