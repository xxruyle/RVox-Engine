#include "game_time/gameTime.h" 


void GameTime::getFPS(GLFWwindow* window) 
{
    // getting fps 
    currentTime = glfwGetTime(); 
    timeDiff = currentTime - prevTime; 
    counter++; 
    if (timeDiff >= 1.0f / 10.0) 
    {
        std::string FPS = std::to_string((1.0/timeDiff) * counter);
        std::string ms = std::to_string((timeDiff/ counter) * 1000); 
        std::string newTitle = "VoxelEngine - " + FPS + " FPS/ " + ms + "ms"; 
        glfwSetWindowTitle(window, newTitle.c_str()); 
        prevTime = currentTime; 
        counter = 0; 
    }
}

void GameTime::getDeltaTime()
{
    current_frame = glfwGetTime(); 
    deltaTime = current_frame - lastFrame; 
    lastFrame = current_frame;
}

