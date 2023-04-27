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



// global variables 
const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 1000;



GLfloat vertices[] = {

    // Top
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,


    // Bottom 
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,


    // Left 
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,


    // Right 
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    // Front
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    // Back 
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

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
    "res/textures/snowblock/right.jpg", 
    "res/textures/snowblock/left.jpg", 
    "res/textures/snowblock/top.jpg", 
    "res/textures/snowblock/bottom.jpg", 
    "res/textures/snowblock/front.jpg", 
    "res/textures/snowblock/back.jpg", 
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

 


	VAO VAO1;
	VAO1.Bind();
	VBO VBO1(vertices, sizeof(vertices));
	VAO1.LinkVBO(VBO1, 0);
    VAO1.configVertexAttributes(0, 3, 5, 0); // coordinates 
    VAO1.configVertexAttributes(1, 2, 5, 3*sizeof(GLfloat)); // texture
    VAO1.Unbind(); 


    // VAO for light source 
    VAO lightVAO; 
    lightVAO.Bind();
    lightVAO.LinkVBO(VBO1, 0);  
    lightVAO.configVertexAttributes(0, 3, 5, 0);  
    lightVAO.configVertexAttributes(1, 2, 5, 3*sizeof(GLfloat)); // texture
    lightVAO.Unbind();  

    // Grass/block texture  
    Texture TEX(GL_TEXTURE0); 
    TEX.Bind(GL_TEXTURE_2D, GL_TEXTURE0); 
    TEX.Generate("res/textures/grass.jpg", 512, 512, GL_RGB, true);  

    Texture lightTEX(GL_TEXTURE1); 
    lightTEX.Bind(GL_TEXTURE_2D, GL_TEXTURE0); 
    lightTEX.Generate("res/textures/green.jpg", 512, 512, GL_RGB, true); 

     
    world.generatePlane(rand() % 3000 + 1); // generating God seed...

    // world.generateSingle(); 

    glm::vec3 lightPos(0.2f, 30.0f, 2.0f); 

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



        // rendering cube 
        shaderProgram.Activate(); 
        shaderProgram.setInt("texture1", 0); // setting texture 
        shaderProgram.setVec3("objectColor", 1.0f, 0.5f, 0.31f); 
        shaderProgram.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        glm::mat4 view; // view matrix 
        glm::mat4 projection = glm::mat4(1.0f);  // projection matrix 
        view = gameCamera.getViewMatrix();  
        projection = glm::perspective(glm::radians(gameCamera.mFov), (float)(SCR_WIDTH/SCR_HEIGHT), 0.1f, 100.0f);

        shaderProgram.setMat("view", 1, GL_FALSE, view); 
        shaderProgram.setMat("projection", 1, GL_FALSE, projection); 

        TEX.Bind(GL_TEXTURE_2D, GL_TEXTURE0); 
        VAO1.Bind();

        for (auto setF : world.positions)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, setF);
            shaderProgram.setMat("model", 1, GL_FALSE, model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }



        // rendering light source 
        lightShaderProgram.Activate(); 
        lightVAO.Bind(); 
        lightTEX.Bind(GL_TEXTURE_2D, GL_TEXTURE1); 
        lightShaderProgram.setInt("lightTexture", 0); // texture setting 
        lightShaderProgram.setMat("view", 1, GL_FALSE, view); 
        lightShaderProgram.setMat("projection", 1, GL_FALSE, projection); 

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::mat4(1.0f); 
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.5f)); 

        lightShaderProgram.setMat("model", 1, GL_FALSE, model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        

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

