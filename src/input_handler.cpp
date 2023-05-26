#include "input/input_handler.h" 

void InputHandler::handleMouseCallbackInput(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
    {
        chunkManager.deleteVoxel();  
    }
}

void InputHandler::handleKeyCallbackInput(GLFWwindow* window, int button, int action, int modsbe)
{
    if (button == GLFW_KEY_ESCAPE && action == GLFW_PRESS)  
        glfwSetWindowShouldClose(window, true); 

    if (button == GLFW_KEY_1 && action == GLFW_PRESS)  
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (button == GLFW_KEY_2 && action == GLFW_PRESS)  
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (button == GLFW_KEY_3 && action == GLFW_PRESS)  // prints how many cubes there are 
        chunkManager.printTotalVoxels();  

    if (button == GLFW_KEY_4 && action == GLFW_PRESS) 
    {
        chunkManager.printChunkLocation(); 
    }

    if (button == GLFW_KEY_5 && action == GLFW_PRESS) 
    {
        chunkManager.chunkMap[glm::vec3(0,0,0)].voxels[0][40][0] = 1; 
        chunkManager.chunkMap[glm::vec3(0,0,0)].mesh(); 
    }


    if (button == GLFW_KEY_G && action == GLFW_PRESS) 
        chunkManager.createChunks(rand() % 2000 + 1);  

}

