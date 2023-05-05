#pragma once 
#include <iostream>
#include <cmath>
#include <math.h> 
#include <stdio.h> 

#include <algorithm>
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "world/world.h"


class Camera 
{
    private: 
        bool firstMouse = true; 
        void updateDirectionVectors(); 

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

        World* mWorld; // the world of cubes 

    

        Camera(glm::vec3 position, glm::vec3 front, float yaw, float pitch, float fov, float speed, float sensitivity, World* world);   

        glm::mat4 getViewMatrix(); 
        void processInput(GLFWwindow* window, float deltaTime); // camera movement 

        void processMouseMovement(double xoffset, double yoffset); // updates pitch and yaw based on mouse movement and updates direction vectors  
    

        void zoom(double xoffset, double yoffset); // decreases the fov to zoom 


        glm::vec3 rayCast(); // casts a vector from camera and gets first cube that the vector hits, returns glm::vec4(origin, xray, yray, zray)  
        glm::vec3 brensenCast(); // raycast using brensenham line algorithm

        std::vector<glm::vec3> bresenhamCoords(); 

        std::vector<glm::vec3> voxelTraversal(); 

        void printPosition(); 

        glm::vec3 getVoxelFace(glm::vec3 origin, float xray, float yray, float zray); // returns the side of the cube that the raycast is hitting
        void deleteVoxel();  
}; 