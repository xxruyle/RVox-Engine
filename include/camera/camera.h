#pragma once 
#include <iostream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
    private: 
        bool firstMouse = true; 
        bool isCollided = false; 
        void updateDirectionVectors(); 

        enum eInput {
            FORWARD, 
            BACKWARDS,
            RIGHT, 
            LEFT, 
            ZOOM, 
            SPRINT
        }; 

    public: 
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

    

        Camera(glm::vec3 position, glm::vec3 front, float yaw, float pitch, float fov, float speed, float sensitivity);   

        glm::mat4 getViewMatrix(); 
        void processInput(GLFWwindow* window, float deltaTime);   

        void processMouseMovement(double xoffset, double yoffset); 

        void zoom(double xoffset, double yoffset); 

        void checkCollision(float deltaTime); 
}; 