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
#include "game_time/gameTime.h"
#include "input/input_handler.h" 
#include "texture/stb_image.h" 
#include "render/render.h"



// global variables 
const unsigned int SCR_WIDTH = 1300;
const unsigned int SCR_HEIGHT = 1000;


GLfloat vertices[] = {
    // vertices           // normals 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f

};

GLfloat crosshairVertices[] = {
    // top right triangle 
     0.5f, -0.5f,      0.0f, 1.0f, 
     0.5f, 0.5f,       1.0f, 1.0f, 
    -0.5f, 0.5f,       0.0f, 1.0f, 

    // bottom left triangle 
     0.5, -0.5,        0.0f, 1.0f,
    -0.5f, 0.5f,       0.0f, 1.0f, 
    -0.5f, -0.5f,      0.0f, 0.0f,
};

std::vector<std::string> cubeMapFaces = {
    "res/textures/grassblock/right.jpg", 
    "res/textures/grassblock/left.jpg", 
    "res/textures/grassblock/top.jpg", 
    "res/textures/grassblock/bottom.jpg", 
    "res/textures/grassblock/front.jpg", 
    "res/textures/grassblock/back.jpg", 
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
Camera gameCamera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, -1.0f), 90.0f, 0.0f, 90.0f, 10.0f, 0.1f, &world); 
GameTime gTime;


GLuint indices[] = {  // note that we start from 0!
    0, 1, 3,  // sole triangle
    1, 2, 3, 
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    // Whenever the window changes in size GLFW calls this function
    glViewport(0, 0, width, height); 
}


InputHandler inputHandler(gameCamera, world); 

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

    // redstone lamp texture 
    Texture lightTEX(GL_TEXTURE1); 
    lightTEX.Bind(GL_TEXTURE_CUBE_MAP, GL_TEXTURE1); 
    // lightTEX.Generate("res/textures/green.jpg", 512, 512, GL_RGB, true); 
    lightTEX.GenerateCubeMap(lampFaces, 512, 512, GL_RGBA, false); 
    lightTEX.Unbind(); 

    VBO VBO2(crosshairVertices, sizeof(crosshairVertices)); 
    VAO crosshairVAO(VBO2); 
    crosshairVAO.Bind(); 
    crosshairVAO.configVertexAttributes(0, 4, 4, 0); 
    crosshairVAO.Unbind(); 

    Texture crosshairTEX(GL_TEXTURE2); 
    crosshairTEX.Bind(GL_TEXTURE_2D, GL_TEXTURE2); 
    crosshairTEX.Generate("res/textures/crosshair.png", 512, 512, GL_RGBA, false); 
    crosshairTEX.Unbind(); 

     
    world.generateLand(rand() % 3000 + 1, false); // generating God seed...
    // world.generatePlane();
    // world.generateSingle(); 

    Render renderer(SCR_WIDTH, SCR_HEIGHT);  

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



        // float lightX = 150*sin(0.5f*(float)glfwGetTime()) + 100.0f, lightY = 60*cos(0.5f*(float)glfwGetTime()) + 5.0f, lightZ = 100.0f; 
        float lightX = 5.0f, lightY = 100.0f, lightZ = 0.0f; 
        glm::vec3 lightPos(lightX, lightY, lightZ); 


        // rendering cube 
        shaderProgram.Activate(); 
        // shaderProgram.setInt("cubeMap", 0); // setting texture 
        shaderProgram.setFloat("ambience", 0.1f); 
        shaderProgram.setVec3("objectColor", 1.0f, 1.0f, 1.0f); 
        shaderProgram.setVec3("lightColor", 1.0f, 1.0f, 1.0f); 
        shaderProgram.setVec3("lightPos", lightX, lightY, lightZ); 
        shaderProgram.setVec3("viewPos", (float)gameCamera.mPosition.x, (float)gameCamera.mPosition.y, (float)gameCamera.mPosition.z);  

        renderer.viewProject(gameCamera); 
        renderer.setShaders(shaderProgram); 

        // TEX.Bind(GL_TEXTURE_CUBE_MAP, GL_TEXTURE0); 
        VAO1.Bind();

        // drawing the world voxel vector 
        for (auto voxel : world.voxelVec)
        {
            renderer.drawVoxel(shaderProgram, voxel.coordinates, voxel.color, 1.0f); 
        }

        // drawing the raycast with scaled down voxels 
/*         std::vector<glm::vec3> raycast = gameCamera.voxelTraversal(); 

        for (auto position: raycast) 
        {
            renderer.drawVoxel(shaderProgram, position, 0.2); 
        } */

        TEX.Unbind(); 


        // rendering light source 
        lightShaderProgram.Activate(); 
        lightVAO.Bind(); 
        lightTEX.Bind(GL_TEXTURE_CUBE_MAP, GL_TEXTURE1); 
        lightShaderProgram.setInt("lightCubeMap", 1); // texture setting 
        lightShaderProgram.setVec3("lightColor", 1.0f, 1.0f, 1.0f); 
        
    
        renderer.setShaders(lightShaderProgram); 
        renderer.drawRotatingVoxel(lightShaderProgram, lightPos, 20.0f, 2.0f*(float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));  
        lightTEX.Unbind(); 



        // 2D Rendering 
        glDisable(GL_DEPTH_TEST); 
        crossHairProgram.Activate(); 

        crosshairVAO.Bind(); 
        crosshairTEX.Bind(GL_TEXTURE_2D, GL_TEXTURE2);  
        crossHairProgram.setInt("crosshairSprite", 0); 
        
        renderer.draw2D(crossHairProgram, glm::vec2((float)SCR_WIDTH /2, (float)SCR_HEIGHT/2), 10.0f); 

        crosshairVAO.Unbind(); 
        crosshairTEX.Unbind(); 

        
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

