#pragma once 
#include <iostream> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 

class GameTime
{
    private: 
        double prevTime = 0.0; 
        double currentTime = 0.0; 
        double timeDiff; 
        unsigned int counter = 0; 

        
        float current_frame = 0.0f; 
        float lastFrame = 0.0f;


    public: 
        float deltaTime = 0.0f; 
        void getFPS(GLFWwindow* window);
        void getDeltaTime(); 
};