// opengl and standard library 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream> 
#include <vector> 
#include <cmath>
#include <random>
#include <time.h> 

// header files 
#include "shader/shader.h" 
#include "buffer/VBO.h"
#include "buffer/VAO.h" 
#include "buffer/EBO.h" 
#include "texture/texture.h"
#include "camera/camera.h"
#include "world/world.h"
#include "world/chunk_manager.h"
#include "world/light.h"
#include "game_time/gameTime.h"
#include "input/input_handler.h" 
#include "texture/stb_image.h" 
#include "render/render.h"
#include "camera/frustum.h" 
#include "mesh/mesh.h"



// global variables 
const unsigned int SCR_WIDTH = 1300;
const unsigned int SCR_HEIGHT = 1000;


GLfloat vertices[] = {
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,         
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    // Left face
    -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,      
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,    
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  
};


GLfloat crosshairVertices[] = {
    // pos      // tex
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
};

/* std::vector<std::string> cubeMapFaces = {
    "res/textures/grassblock/right.jpg", 
    "res/textures/grassblock/left.jpg", 
    "res/textures/grassblock/top.jpg", 
    "res/textures/grassblock/bottom.jpg", 
    "res/textures/grassblock/front.jpg", 
    "res/textures/grassblock/back.jpg", 
};  */

std::vector<std::string> cubeMapFaces = {
    "res/textures/iron.jpg", 
    "res/textures/iron.jpg",
    "res/textures/iron.jpg",
    "res/textures/iron.jpg", 
    "res/textures/iron.jpg", 
    "res/textures/iron.jpg",
}; 

std::vector<std::string> specularMap = {
    "res/textures/ironSpecular.jpg",     
    "res/textures/ironSpecular.jpg",    
    "res/textures/ironSpecular.jpg",    
    "res/textures/ironSpecular.jpg",     
    "res/textures/ironSpecular.jpg",     
    "res/textures/ironSpecular.jpg",    
}; 

std::vector<std::string> lampFaces = {
    "res/textures/redstonelamp/right.png", 
    "res/textures/redstonelamp/left.png", 
    "res/textures/redstonelamp/top.png", 
    "res/textures/redstonelamp/bottom.png", 
    "res/textures/redstonelamp/front.png", 
    "res/textures/redstonelamp/back.png", 
};

// initializing helper classes 
World world; 
Camera gameCamera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, -1.0f), 90.0f, 0.0f, 90.0f, 10.0f, 0.1f);   
Light lighting; 
GameTime gTime;
Frustum frustum(gameCamera);  
Render renderer(SCR_WIDTH, SCR_HEIGHT, frustum);  
ChunkManager chunkManager(world, renderer, gameCamera);     
InputHandler inputHandler(gameCamera, chunkManager);   


GLuint indices[] = {  // note that we start from 0!
    0, 1, 3,  // sole triangle
    1, 2, 3, 
};




void processInput(GLFWwindow* window); 
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods); 
void key_callback(GLFWwindow* window, int button, int scancode, int action, int mods); 

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

int main() 
{
    srand(time(NULL));  

    glfwInit(); // initializes glfw 

    // hints configure GLFW and tell it what version of opengl it is using  
    // 3 because we are using opengl 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

    Shader lightShaderProgram("res/shaders/lightsource.vert", "res/shaders/lightsource.frag"); 
    lightShaderProgram.Activate(); 

    Shader shaderProgram("res/shaders/default.vert", "res/shaders/default.frag"); 
    shaderProgram.Activate();  

    Shader crossHairProgram("res/shaders/crosshair.vert", "res/shaders/crosshair.frag"); 
    crossHairProgram.Activate(); 

    // cube 
	VBO VBO1(vertices, sizeof(vertices));
	VAO VAO1(VBO1); 
	VAO1.Bind();
    VAO1.configVertexAttributes(0, 3, 6, 0); // coordinates 
    VAO1.configVertexAttributes(1, 3, 6, 3*sizeof(GLfloat)); // texture
    VAO1.Unbind(); 

    // lightsource 
    // VAO for light source 
    VAO lightVAO(VBO1);  
    lightVAO.Bind();
    lightVAO.configVertexAttributes(0, 3, 6, 0); 
    lightVAO.Unbind(); 



    // Grass/block texture  
    Texture TEX(GL_TEXTURE0); 
    TEX.Bind(GL_TEXTURE_CUBE_MAP, GL_TEXTURE0); 
    TEX.GenerateCubeMap(cubeMapFaces, 512, 512, GL_RGB, false);  
    TEX.Unbind(); 

    Texture tSpec(GL_TEXTURE2); 
    tSpec.Bind(GL_TEXTURE_CUBE_MAP, GL_TEXTURE1); 
    tSpec.GenerateCubeMap(specularMap, 512, 512, GL_RGB, false);  
    tSpec.Unbind(); 

    // redstone lamp texture 
    Texture lightTEX(GL_TEXTURE1); 
    lightTEX.Bind(GL_TEXTURE_CUBE_MAP, GL_TEXTURE2); 
    // lightTEX.Generate("res/textures/green.jpg", 512, 512, GL_RGB, true); 
    lightTEX.GenerateCubeMap(lampFaces, 512, 512, GL_RGBA, false); 
    lightTEX.Unbind(); 

    VBO VBO2(crosshairVertices, sizeof(crosshairVertices)); 
    VAO crosshairVAO(VBO2); 
    crosshairVAO.Bind(); 
    crosshairVAO.configVertexAttributes(0, 4, 4, 0); 
    crosshairVAO.Unbind(); 

    Texture crosshairTEX(GL_TEXTURE3); 
    crosshairTEX.Bind(GL_TEXTURE_2D, GL_TEXTURE3); 
    crosshairTEX.Generate("res/textures/crosshairbox.png", 512, 512, GL_RGBA, false); 
    crosshairTEX.Unbind(); 

    chunkManager.createChunks(rand() % 2000 + 1);  


    // The main render loop 
    while (!glfwWindowShouldClose(window)) 
    {
        // time functions for deltaTime and fps 
        gTime.getFPS(window); 
        gTime.getDeltaTime();

        processInput(window); 

        glClearColor(57.0f/255.0f, 54.0f/255.0f, 70.0f/255.0f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        glEnable(GL_DEPTH_TEST); 

        glEnable(GL_CULL_FACE);  
        glCullFace(GL_BACK);  




        float lightX = 16.0f, lightY = 37+(3.0*sin(glfwGetTime())), lightZ = 16.0f;  
        // float lightX = 5.0f, lightY = 5.0f, lightZ = 3.0f;  
        glm::vec3 lightPos(lightX, lightY, lightZ); 


        shaderProgram.Activate(); 
        shaderProgram.setInt("material.diffuse", 0); 
        shaderProgram.setInt("material.specular", 1); 
 

/*         shaderProgram.setVec3("light.position", gameCamera.mPosition.x, gameCamera.mPosition.y, gameCamera.mPosition.z);  
        shaderProgram.setVec3("light.direction", gameCamera.mFront.x, gameCamera.mFront.y, gameCamera.mFront.z);  
        shaderProgram.setFloat("light.cutOff", glm::cos(glm::radians(12.5f))); 
        shaderProgram.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f))); 
        shaderProgram.setVec3("viewPos", gameCamera.mPosition.x, gameCamera.mPosition.y, gameCamera.mPosition.z);  



        // shaderProgram.setVec3("light.direction", -0.2f, -1.0f, -0.3f);    
        shaderProgram.setVec3("light.ambient",  0.1f, 0.1f, 0.1f);   
        shaderProgram.setVec3("light.diffuse",  0.8f, 0.8f, 0.8f);  
        shaderProgram.setVec3("light.specular", 1.0f, 1.0f, 1.0f);  

        shaderProgram.setFloat("light.constant",  1.0f); 
        shaderProgram.setFloat("light.linear",    0.09f); 
        shaderProgram.setFloat("light.quadratic", 0.032f); 

        shaderProgram.setFloat("material.shininess", 32.0f);    */

        // shaderProgram.setVec3("light.position", lightX, lightY, lightZ);  

        // shaderProgram.setFloat("light.innerCutOff", glm::cos(glm::radians(25.0f)));  

        // lighting.spotLightInit(shaderProgram, gameCamera);  
        lighting.sunLightInit(shaderProgram, gameCamera); 
        // lighting.sourceLightInit(shaderProgram, gameCamera, lightPos);




        // shaderProgram.setVec3("lightColor", 1.0f, 1.0f, 1.0f); 

        renderer.viewProject(gameCamera); 
        renderer.setShaders(shaderProgram); 

        TEX.Bind(GL_TEXTURE_CUBE_MAP, GL_TEXTURE0);   
        tSpec.Bind(GL_TEXTURE_CUBE_MAP, GL_TEXTURE1); 

        
        VAO1.Bind();

        // setting up frustum  
        frustum.setCamInternals(); 
        frustum.setCamDef(); 

        // drawing the chunk manager chunks 
        chunkManager.renderChunks(shaderProgram);      
        // chunkManager.renderOneVoxel(shaderProgram);      
        // chunkManager.voxelOutline(); // enables voxel outline coloring 
        // renderer.drawVoxel(shaderProgram, glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f); 
        // renderer.drawVoxel(shaderProgram, glm::vec3(1.0f,1.0f, 3.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f); 


        TEX.Unbind(); 
        tSpec.Unbind(); 


        // rendering light source 
        lightShaderProgram.Activate(); 
        lightVAO.Bind(); 
        lightTEX.Bind(GL_TEXTURE_CUBE_MAP, GL_TEXTURE2); 
        lightShaderProgram.setInt("lightCubeMap", 2); // texture setting 
        lightShaderProgram.setVec3("lightColor", 1.0f, 1.0f, 1.0f);   
        
    
        renderer.setShaders(lightShaderProgram); 
        renderer.drawRotatingVoxel(lightShaderProgram, lightPos, 1.0f, 2.0f*(float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));  
        lightTEX.Unbind(); 


        // 2D Rendering 
        glDisable(GL_DEPTH_TEST); 
        crossHairProgram.Activate(); 

        crosshairVAO.Bind(); 
        crosshairTEX.Bind(GL_TEXTURE_2D, GL_TEXTURE3);  
        crossHairProgram.setInt("crosshairSprite", 3); 
        
        renderer.draw2D(crossHairProgram, glm::vec2(((float)SCR_WIDTH /2) - 5.0f, ((float)SCR_HEIGHT/2) - 5.0f), 10.0f); 

        crosshairVAO.Unbind(); 
        crosshairTEX.Unbind(); 

        glDisable(GL_BLEND); 

        glfwSwapBuffers(window); // swaps the color buffer which is used to render during each render iteration and show output to the screen 
        glfwPollEvents(); 
    }


    VAO1.Delete(); 
    VBO1.Delete(); 
    TEX.Delete(); 
    lightTEX.Delete(); 
    shaderProgram.Delete(); 
    lightShaderProgram.Delete(); 
    glfwDestroyWindow(window);
    glfwTerminate(); 
    return 0; 
}

void processInput(GLFWwindow* window) 
{
    gameCamera.processInput(window, gTime.deltaTime);  
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
    inputHandler.handleMouseCallbackInput(window, button, action, mods); 
}

void key_callback(GLFWwindow* window, int button, int scancode, int action, int mods) 
{
    inputHandler.handleKeyCallbackInput(window, button, action, mods); 
}  