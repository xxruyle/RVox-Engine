#pragma once 
#include <iostream> 
#include <string> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh/plymodel.h" 

class Player 
{
public: 
    glm::vec3 mPosition; 
    PLYModel* playerModel;



    Player(glm::vec3 position) 
    {
        mPosition = position; 
        std::string const &path = "res/models/chr_player_default.ply"; 
        this->playerModel = new PLYModel(path, position, 1.0f);    
    }; 


    void move(GLFWwindow* window, float deltaTime);   

    ~Player() { 
        std::cout << "deleting player" << std::endl; 
        delete playerModel;
        } 

private: 
    float mSpeed = 10.0f; 
}; 