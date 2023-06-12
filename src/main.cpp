// opengl and standard library 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <iostream> 
#include <vector> 
#include <cmath>
#include <random>
#include <time.h> 

// header files 
#include "shader/shader.h" 
#include "texture/texture.h"
#include "camera/camera.h"
#include "camera/orthocamera.h" 
#include "world/chunk_manager.h"
#include "world/light.h"
#include "game_time/gameTime.h"
#include "input/input_handler.h" 
#include "render/render.h"
#include "camera/frustum.h" 
#include "mesh/plymodel.h" 
#include "hud/hud.h" 
#include "debug/debug_tools.h"
#include "player/player.h" 

#include "world/depth_map.h" 



// global variables 
const unsigned int SCR_WIDTH = 1300;
const unsigned int SCR_HEIGHT = 1000;

// initializing helper classes 
Camera gameCamera(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0f, 0.0f, -1.0f), 90.0f, 0.0f, 90.0f, 10.0f, 0.1f);   
OrthoCamera orthoCamera(glm::vec3(-30.0, 70.0, -30.0), 45.0, 0.0, 90.0f);  
GameTime gTime;
Light lighting;  
Frustum frustum(gameCamera);  
Render renderer(SCR_WIDTH, SCR_HEIGHT, frustum);  
ChunkManager chunkManager(renderer, gameCamera, frustum);      
InputHandler inputHandler(gameCamera, chunkManager);   
Hud hud(renderer); 




void processInput(GLFWwindow* window, Player& player);  
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods); 
void key_callback(GLFWwindow* window, int button, int scancode, int action, int mods); 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); 


int main() 
{
    srand(time(NULL));  // random  

    glfwInit(); // initializes glfw 

    // hints configure GLFW and tell it what version of opengl it is using  
    // 3 because we are using opengl 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4); // for anti alias  
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

    //creating window and error handling 
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "learnopengl", NULL, NULL); 
    if (window == NULL) 
    {
        std::cout << "Failed to create GLFW window" << std::endl; 
        glfwTerminate(); 
        return -1; 
    }

    glfwMakeContextCurrent(window); 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // updates window size

    // call back states 
    glfwSetCursorPosCallback(window, mouse_callback); // gets info from mouse 
    glfwSetScrollCallback(window, scroll_callback); 
    glfwSetMouseButtonCallback(window, mouse_button_callback); 
    glfwSetKeyCallback(window, key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hide cursor and capture it 
    gladLoadGL(); // initializing glad 

    Shader shaderProgram("res/shaders/default.vert", "res/shaders/default.frag"); 
    shaderProgram.Activate();  

    Shader hudProgram("res/shaders/hud.vert", "res/shaders/hud.frag"); 
    hudProgram.Activate(); 

    Shader outlineProgram("res/shaders/outline.vert", "res/shaders/outline.frag"); 
    outlineProgram.Activate(); 

    Shader depthMapProgram("res/shaders/depthmap.vert", "res/shaders/depthmap.frag");  
    depthMapProgram.Activate();  

    Shader debugDepth("res/shaders/debugdepth.vert", "res/shaders/debugdepth.frag");   
    debugDepth.Activate(); 

    hud.crossHairInit("res/textures/crosshairbox.png"); // initializes crosshair  


    chunkManager.createChunks(rand() % 2000 + 1);  

    
    // PLYModel plymodel("res/models/chr_skeleton.ply", glm::vec3(0, 0, 5), 1.1f);     
    // PLYModel plymodelPlayer("res/models/chr_player_default.ply", glm::vec3(0,0,0) , 1.0f);  
    // PLYModel xyzAxis("res/models/xyzAxis.ply", gameCamera.mPosition + gameCamera.mFront, 1.0f); 



    Player player(glm::vec3(0, 0, 0));       
    // chunkManager.spawnPlayer(glm::vec3(0, 0, 0), player); 


    DebugTools debugTools; 
    debugTools.getBoundingBoxVertices(player.playerModel->ModelBoundingBox); 

    DebugTools wireFrame; 
    wireFrame.getOutlineVertices(); 


/*     DepthMap depthMap; 


    PLYModel light("res/models/debug.ply", glm::vec3(0, 40, 8), 1.0f);  
    PLYModel obstruction("res/models/monu1.ply", glm::vec3(8, 10, 8), 1.0f);   */



    // The main render loop z
    while (!glfwWindowShouldClose(window)) 
    {
        // depth map rendering 
/*         glm::vec3 lightPos = light.mPosition;   
        light.mPosition = glm::vec3(-20, 30, -20);               

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST); 
        depthMap.Render(depthMapProgram, lightPos, glm::vec3(8, 0, 8));     
        obstruction.Draw(depthMapProgram);  
        chunkManager.renderChunks(depthMapProgram); 
        player.playerModel->Draw(depthMapProgram);  
        depthMap.Unbind(); */ 


        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);    
        glClearColor(255.0f/255.0f, 193.0f/255.0f, 142.0f/255.0f, 1.0f); // sky color 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        glEnable(GL_DEPTH_TEST); 


        // MSAA 
        glEnable(GL_MULTISAMPLE);  
        glEnable(GL_CULL_FACE);   
        glCullFace(GL_BACK);   

        shaderProgram.Activate(); 
        // shaderProgram.setInt("material.diffuse", 0); 
        // shaderProgram.setInt("material.specular", 1); 
        // shaderProgram.setFloat("renderDistance", 300); 
 
        // lighting 
        lighting.setupDirLight(shaderProgram, gameCamera);    
  
        // camera and frustum 
        // renderer.viewProject(gameCamera); // first pesron camera          
        renderer.playerViewProject(gameCamera, player);  // third person camera     
        // renderer.viewOrtho(orthoCamera); // orthographic camera   

        renderer.setShaders(shaderProgram); 
        // depth maps stuff 
        // depthMap.bindTexture();  
        // renderer.setShaders(shaderProgram); 
        // renderer.setDepthMapShaders(shaderProgram, depthMap.lightSpaceMatrix, lightPos);      



        // setting up frustum  
        frustum.setCamInternals(); 
        frustum.setCamDef();  

        



        // drawing the chunk manager chunks 
        chunkManager.renderChunks(shaderProgram);       
        // chunkManager.checkCollision(player);   



        // time functions for deltaTime and fps 
        gTime.getFPS(window); 
        gTime.getDeltaTime();


        // models 
        player.playerModel->Draw(shaderProgram); 

/*         light.Draw(shaderProgram); 
        obstruction.Draw(shaderProgram);  */


        // for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
        glDisable(GL_CULL_FACE); 

        // voxel outline 
        outlineProgram.Activate(); 
        renderer.setShaders(outlineProgram); 
        // chunkManager.voxelOutline(outlineProgram, wireFrame);       

        // debugTools.DrawBoundingBox(outlineProgram, player.playerModel->mPosition);         


        // 2D Rendering 
        hud.DrawCrosshair(hudProgram);  
        glDisable(GL_BLEND); 


        // The player movement is calculated in this function by way of the camera function being called 
        // probably not the best structure here but I don't know a better way lol...
        processInput(window, player);      


        // for debugging and viewing the ortho projection of the depthbuffer light source 
/*         depthMap.bindTexture(); 
        debugDepth.Activate();  
        depthMap.DebugRender(debugDepth);   */


        glfwSwapBuffers(window); // swaps the color buffer which is used to render during each render iteration and show output to the screen 
        glfwPollEvents(); 
    }

    shaderProgram.Delete(); 
    hudProgram.Delete(); 
    outlineProgram.Delete(); 
    glfwDestroyWindow(window);
    glfwTerminate(); 
    return 0; 
}

void processInput(GLFWwindow* window, Player& player)   
{
    gameCamera.processInput(window, gTime.deltaTime, player);    
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
    inputHandler.handleMouseCallbackInput(window, button, action, mods); 
}

void key_callback(GLFWwindow* window, int button, int scancode, int action, int mods) 
{
    inputHandler.handleKeyCallbackInput(window, button, action, mods); 
}  

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    // Whenever the window changes in size GLFW calls this function
    glViewport(0, 0, width, height); 
}

bool firstMouse = true; 
float lastX = SCR_HEIGHT/2, lastY = SCR_WIDTH/2; // put mouse at center of screen 
void mouse_callback(GLFWwindow* window, double xpos, double ypos)   
{
    
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }


    float xoffset = xpos - lastX; 
    float yoffset = lastY - ypos; 
    lastX = xpos; 
    lastY = ypos; 

    gameCamera.processMouseMovement(xoffset, yoffset); 
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
    gameCamera.zoom(xoffset , yoffset);  
}