#pragma once 
#include "camera/camera.h"
#include "world/chunk_manager.h" 

class InputHandler 
{
private:
    Camera& camera; 
    ChunkManager& chunkManager;

public: 
    

    InputHandler(Camera& camera, ChunkManager& chunkManager) : camera(camera), chunkManager(chunkManager) {};  

    void handleMouseCallbackInput(GLFWwindow* window, int button, int action, int mods); 
    void handleKeyCallbackInput(GLFWwindow* window, int button, int action, int mods);
};