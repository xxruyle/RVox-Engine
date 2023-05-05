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


glm::vec3 Camera::brensenCast() 
{
    std::vector<glm::vec3> voxelCoords = voxelTraversal(); 

    for (unsigned int i = 0; i < voxelCoords.size(); i++) 
    {
        for (unsigned int j = 0; j < mWorld->voxelVec.size(); j++) 
        {
            if (mWorld->voxelVec[j].coordinates == voxelCoords[i])
                return voxelCoords[i];
        }
    }

    std::cout << "No block casted" << std::endl; 
    return glm::vec3(0); 
}


#define SIGN(x) (x > 0 ? 1 : (x < 0 ? -1 : 0))
#define FRAC0(x) (x - floorf(x))
#define FRAC1(x) (1 - x + floorf(x))

std::vector<glm::vec3> Camera::voxelTraversal()  
{
    std::vector<glm::vec3> voxelRayIntersections; 

    float castLength = 4.0; 

    // we add 0.5f here to offset the grid layout that happens when placing blocks (the origin of a block is the top left of the cube)
    float xOrigin; 
    float yOrigin; 
    float zOrigin; 

    // if in negative space, shift the grid in negative direction, vice versa 
    mPosition.x > 0 ? xOrigin = mPosition.x + 0.5f : xOrigin = mPosition.x - 0.5f; 
    mPosition.y > 0 ? yOrigin = mPosition.y + 0.5f : yOrigin = mPosition.y - 0.5f; 
    mPosition.z > 0 ? zOrigin = mPosition.z + 0.5f : zOrigin = mPosition.z - 0.5f;

    // voxelRayIntersections.push_back(glm::vec3(floor(xOrigin), floor(yOrigin), floor(zOrigin))); 

    // 4 magnitude vector in direction of the ray 
    float xEnd = xOrigin + (mFront.x * castLength); 
    float yEnd = yOrigin + (mFront.y * castLength); 
    float zEnd = zOrigin + (mFront.z * castLength); 

    voxelRayIntersections.push_back(glm::vec3(xEnd, yEnd, zEnd)); 



    float tMaxX, tMaxY, tMaxZ; // the value of t which the ray crosses the first vertical voxel boundary 
    float tDeltaX, tDeltaY, tDeltaZ; // how far along the ray we must move in t for the component to equal the width of the voxel 


    // coordinate positions 

    int dx = SIGN(xEnd - xOrigin);
    if (dx != 0) tDeltaX = fmin(dx / (xEnd - xOrigin), 10000000.0f); else tDeltaX = 10000000.0f;
    if (dx > 0) tMaxX = tDeltaX * FRAC1(xOrigin); else tMaxX = tDeltaX * FRAC0(xOrigin);
    int x = (int)xOrigin; 

    int dy = SIGN(yEnd - yOrigin);
    if (dy != 0) tDeltaY = fmin(dy / (yEnd - yOrigin), 10000000.0f); else tDeltaY = 10000000.0f;
    if (dy > 0) tMaxY = tDeltaY * FRAC1(yOrigin); else tMaxY = tDeltaY * FRAC0(yOrigin);
    int y = (int)yOrigin; 

    int dz = SIGN(zEnd - zOrigin);
    if (dz != 0) tDeltaZ = fmin(dz / (zEnd - zOrigin), 10000000.0f); else tDeltaZ = 10000000.0f;
    if (dz > 0) tMaxZ = tDeltaZ * FRAC1(zOrigin); else tMaxZ = tDeltaZ * FRAC0(zOrigin);
    int z = (int)zOrigin; 

    for (int i = 0; i < 6; i++) { 
        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                x += dx;
                tMaxX += tDeltaX;
            } else {
                z += dz;
                tMaxZ += tDeltaZ;
            }
        } else {
            if (tMaxY < tMaxZ) {
                y += dy;
                tMaxY += tDeltaY;
            } else {
                z += dz;
                tMaxZ += tDeltaZ;
            }
        }
        // if (tMaxX > 1 && tMaxY > 1 && tMaxZ > 1) break;
        voxelRayIntersections.push_back(glm::vec3(x, y, z)); 
    }


    for (unsigned int i = 0; i < voxelRayIntersections.size(); i++)
    {
        std::cout << voxelRayIntersections[i].x << " " << voxelRayIntersections[i].y << " " << voxelRayIntersections[i].z << std::endl; 
    }
    std::cout << "-------" << std::endl; 

    return voxelRayIntersections; 
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

void Camera::deleteVoxel()
{
    glm::vec3 voxel_position = brensenCast(); 
    for (unsigned int i = 0; i < mWorld->voxelVec.size(); i++) 
    {
        if (mWorld->voxelVec[i].coordinates == voxel_position)
        {
            mWorld->voxelVec.erase(mWorld->voxelVec.begin() + i); 
            break; 
        }
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





