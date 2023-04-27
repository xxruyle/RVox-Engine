#pragma once 
#include "camera/camera.h"

class InputHandler 
{
private:
    Camera& camera; 
    World& world; 
    
public: 
    InputHandler(Camera& camera, World& world) : camera(camera), world(world) {};

    void handleMouseCallbackInput(GLFWwindow* window, int button, int action, int mods); 
    void handleKeyCallbackInput(GLFWwindow* window, int button, int action, int mods);
};