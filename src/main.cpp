#include <iostream> 
#include <vector> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <random>
#include <time.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>



#include "shader/shader.h" 
#include "buffer/VBO.h"
#include "buffer/VAO.h" 
#include "buffer/EBO.h" 
#include "texture/texture.h"


const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 1000;

float deltaTime = 0.0f; 
float lastFrame = 0.0f; 

glm::vec3 cameraPos = glm::vec3(0.0f, 20.0f, 3.0f); 
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f); // facing towards the positive z axis 
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); 

glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); 
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget); // makes a unit vector of the direction vector pointing from origin to camera 



GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};



std::vector<glm::vec3> cubePositions = {};

void pushCubes() // creates a 3D grid block of cubes 
{
    for (float i = 0.0f; i < 64.0f; i++) // makes a row of cubes 
    {
        float k = rand() % 10 + 1; 
        for (float j = 0.0f; j < k; j++) 
        {
            for (float l = 0.0f; l < 64.0f; l++) 
            {
                cubePositions.push_back(glm::vec3{i, j, l}); 
            }
        }
    }
}

GLuint indices[] = {  // note that we start from 0!
    0, 1, 3,  // sole triangle
    1, 2, 3, 
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    // Whenever the window changes in size GLFW calls this function
    glViewport(0, 0, width, height); 
}


float visible = 0.0f;  // visibility of the epic face 
float cameraSpeed = 10.0f * deltaTime;  



float yaw = 90.0f; 
float pitch = 0.0f; 
float fov = 90.0f;

void processInput(GLFWwindow* window) 
{

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // forward 
        cameraPos += cameraSpeed * cameraFront; 

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // backward 
        cameraPos -= cameraSpeed * cameraFront; 
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // left 
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; 
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // right 
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; 
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) // up 
        cameraPos += glm::normalize(glm::cross(glm::normalize(glm::cross(cameraFront, cameraUp)), cameraFront)) * cameraSpeed; 
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) // down 
        cameraPos -= glm::normalize(glm::cross(glm::normalize(glm::cross(cameraFront, cameraUp)), cameraFront)) * cameraSpeed; 
    

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        cameraSpeed += 1.0f * deltaTime; 

    }
    else
    {
        cameraSpeed = 10.0f * deltaTime; 

    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); 

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // wireframe 
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // solid
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) 
        fov = 90.0f; 
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

    float sensitivity = 0.1f; 
    xoffset *= sensitivity; 
    yoffset *= sensitivity; 
    yaw += xoffset; 
    pitch += yoffset; 

    if (pitch > 89.0f) 
        pitch = 89.0f; 
    if (pitch < -89.0f)
        pitch = -89.0f; 


    glm::vec3 direction; 
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)); 
    direction.y = sin(glm::radians(pitch)); 
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch)); 

    cameraFront = glm::normalize(direction); 
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
    fov -= (float)yoffset; 
    if (fov < 1.0f) 
        fov = 1.0f; 
    if (fov > 90.0f) 
        fov = 90.0f; 
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

    glfwSetCursorPosCallback(window, mouse_callback); // gets info from mouse 
    glfwSetScrollCallback(window, scroll_callback); 
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hide cursor and capture it 



    gladLoadGL(); 

    // enable depth buffer 
    glEnable(GL_DEPTH_TEST); 


    Shader shaderProgram("res/shaders/default.vert", "res/shaders/default.frag"); 
    shaderProgram.Activate();  
    shaderProgram.Delete(); 
    

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));

	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO
	VAO1.LinkVBO(VBO1, 0);
    VAO1.configVertexAttributes(0, 3, 5, 0); // coordinates 
    VAO1.configVertexAttributes(1, 2, 5, 3*sizeof(GLfloat)); // texture
    // VAO1.configVertexAttributes(2, 2, 8, 6*sizeof(GLfloat)); // texture 




    // Making textures 
    Texture TEX(GL_TEXTURE0); 
    TEX.Bind(GL_TEXTURE0); 
    TEX.setParameters(GL_REPEAT); 
    TEX.Generate("res/textures/iron.jpg", 512, 512, GL_RGB, true);  // grass texture 

    Texture TEX2(GL_TEXTURE1); // assigned to texture unit 1 
    TEX2.Bind(GL_TEXTURE1); 
    TEX2.setParameters(GL_REPEAT); 
    TEX2.Generate("res/textures/awesomeface.png", 512, 512, GL_RGBA, true); // second texture 


    shaderProgram.Activate(); 
    shaderProgram.setInt("texture1", 0); 
    shaderProgram.setInt("texture2", 1); 

    pushCubes(); 

    
    // The main render loop 
    while (!glfwWindowShouldClose(window)) 
    {
        // getting delta time 
        float currentFrame = glfwGetTime(); 
        deltaTime = currentFrame - lastFrame; 
        lastFrame = currentFrame; 


        // input 
        processInput(window); 

        // rendering commands 
        glClearColor(57.0f/255.0f, 54.0f/255.0f, 70.0f/255.0f, 1.0f); 


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activating the shader 
        shaderProgram.Activate(); 


        glm::mat4 view; // view matrix 
        glm::mat4 projection = glm::mat4(1.0f);  // projection matrix 


        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); 
        projection = glm::perspective(glm::radians(fov), (float)(SCR_WIDTH/SCR_HEIGHT), 0.1f, 100.0f);

        shaderProgram.setMat("view", 1, GL_FALSE, view); 
        shaderProgram.setMat("projection", 1, GL_FALSE, projection); 

        // visibility of second texture 
        shaderProgram.setFloat("visibility", visible); 

        TEX.Bind(GL_TEXTURE0); 
        TEX2.Bind(GL_TEXTURE1); 

        VAO1.Bind();
        for (unsigned int i = 0; i < cubePositions.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.0, 1.0, 1.0)); 
            shaderProgram.setMat("model", 1, GL_FALSE, model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // When using EBO 



        glfwSwapBuffers(window); // swaps the color buffer which is used to render during each render iteration and show output to the screen 
        glfwPollEvents(); 
    }


    VAO1.Delete(); 
    VBO1.Delete(); 
    EBO1.Delete(); 
    TEX.Delete(); 
    TEX2.Delete(); 
    shaderProgram.Delete(); 


    glfwDestroyWindow(window);
    glfwTerminate(); 

    return 0; 
}

