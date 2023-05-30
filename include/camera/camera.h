#pragma once 
#include <iostream>
#include <cmath>

#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "player/player.h" 



class Camera 
{
private: 
    bool firstMouse = true; 
    void updateDirectionVectors(); 

public: 
    bool isThirdPerson = true; 
    float SCR_WIDTH = 1300; 
    float SCR_HEIGHT = 1000; 
    
    glm::vec3 mDirection; 
    glm::vec3 mPlayerDirection; 

    glm::vec3 mPosition; 
    glm::vec3 mFront; 
    glm::vec3 mRight; 
    glm::vec3 mUp; 
    glm::vec3 mTarget; 

    glm::vec3 mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f); // k basis vector
    glm::vec3 mPlayerForward; 

    float mYaw; 
    float mPitch; 
    float mFov; 

    float mSpeed; 
    float mSensitivity; 

    float angle;
    float ratio; 
    float nearD; 
    float farD; 

    // Camera(glm::vec3 position, glm::vec3 front, float yaw, float pitch, float fov, float speed, float sensitivity);    
    Camera(glm::vec3 position, glm::vec3 front, float yaw, float pitch, float fov, float speed, float sensitivity) : mPosition(position), mFront(front), mYaw(yaw), mPitch(pitch), mFov(fov), mSpeed(speed), mSensitivity(sensitivity) {
        angle = glm::radians(mFov); 
        ratio = (float)(SCR_WIDTH/SCR_HEIGHT);  
        nearD = 0.1f; 
        farD = 1000.0f; 
        updateDirectionVectors(); 
    }; 

    // player variables 
    float distanceFromPlayer = 20; 
    float angleAroundPlayer = 0; 

    glm::mat4 getViewMatrix(); 
    glm::mat4 getPlayerViewMatrix(Player& player);  
    glm::mat4 getProjectionMatrix(); 
    void processInput(GLFWwindow* window, float deltaTime, Player& player); // camera movement  

    void processMouseMovement(double xoffset, double yoffset); // updates pitch and yaw based on mouse movement and updates direction vectors  

    void zoom(double xoffset, double yoffset); // decreases the fov to zoom 

    void updateThirdPersonInfo(Player& player);  
    float calculateHorizontalDistance();  
    float calculateVerticalDistance(); 
    void calculatePosition(float horizDistance, float vertDistance, Player& player); // caclulates camera position based on player location   

    void changeViewPerson(); 
}; 