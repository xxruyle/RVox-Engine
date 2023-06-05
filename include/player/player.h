#pragma once 
#include <iostream> 
#include <string> 
#include <cmath> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh/plymodel.h" 
#define GLM_ENABLE_EXPERIMENTAL // enables hashing of glm::vec3 
#include "glm/gtx/string_cast.hpp" 
#include "glm/gtx/vector_angle.hpp"

class Player 
{
public: 
    glm::vec3 mPosition;  // default position 
    glm::vec3 toMove; 
    glm::vec3 mRight; 
    glm::vec3 mForward;  
    float currentAngle = 0.0f; 
    float angle; 
    PLYModel* playerModel;
    

    bool jumping = false; 
    bool onGround = false;  

    Player(glm::vec3 position)   
    {
        mPosition = position; 
        std::string const &path = "res/models/chr_human_default_armor.ply";              
        this->playerModel = new PLYModel(path, mPosition, 0.8f);       
    }; 

    void move(GLFWwindow* window, float deltaTime);   
    void printVelocity();  

    ~Player() { 
        std::cout << "deleting player model" << std::endl;  
        delete playerModel;
        } 

    float mSpeed = 0.5f; 
    float currentSpeed = 0.0f; 
    glm::vec3 sweptVelocity; 
    float velocityX = 0.0f; 
    float velocityY = 0.0f; 
    float velocityZ = 0.0f; 
    float gravity = 0.0f;     
    float velocityLimit = 20.0f; 
    glm::vec3 collisionNormal; 

private: 
    glm::vec3 prevPosition;  
    bool firstMove = false; 
    void calculateVelocity(float deltaTime);     
    void calculateSweptVelocity(float deltaTime); 
    void limitVelocity(); 
    void calculateAngle(float deltaTime); 
    void interpolateAngle(float deltaTime);

    void getJumpCooldown(float deltaTime);  
    float jumpCooldown; 
    float jumpCooldownDuration = 1.0f;  

}; 