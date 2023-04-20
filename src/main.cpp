#include <iostream> 
#include <vector> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 

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

void pushCubes() 
{
    for (float i = 0.0f; i < 64.0f; i++) 
    {
        cubePositions.push_back(glm::vec3(i, 0.0f, 0.0f));     
        for (float j = 0.0f; j < 64.0f; j++) 
        {
            cubePositions.push_back(glm::vec3(i, 0.0f, -j));
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
float movex = 0.0f; 
float movez = 0.0f;
float movey = 0.0f; 
float rotate = 0.0f; 
void processInput(GLFWwindow* window) 
{

    float cameraSpeed = 0.1f; 
    // handles user input
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); 

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // wireframe 
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // solid
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
    // {
    //     if (visible < 0.5f) 
    //         visible += 0.0001f; 
// 
    // }
        

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // moving camera right 
    {
        movex += cameraSpeed; 
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // moving camera left 
    {
        movex -= cameraSpeed; 
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // moving camera right 
    {
        movez += cameraSpeed; 
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // moving camera left 
    {
        movez -= cameraSpeed; 
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // moving camera left 
    {
        movey += cameraSpeed; 
    }


    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // moving camera left 
    {
        movey -= cameraSpeed; 
    }


    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // moving camera left 
    {
        rotate += 0.01f; 
        movex += cameraSpeed; 
    }

    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // moving camera left 
    {
        rotate -= 0.01f; 
        movex -= cameraSpeed;
    }
}


int main() 
{
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

    gladLoadGL(); 

    glViewport(0,0, SCR_WIDTH, SCR_HEIGHT); 

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
    TEX.Generate("res/textures/grass.jpg", 512, 512, GL_RGB, true);  // grass texture 

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
        float currentFrame = glfwGetTime(); 
        deltaTime = currentFrame - lastFrame; 
        lastFrame = currentFrame; 


        // input 
        processInput(window); 

        // rendering commands 
        glClearColor(57.0f/255.0f, 54.0f/255.0f, 70.0f/255.0f, 1.0f); 

        // enable depth buffer 
        glEnable(GL_DEPTH_TEST);  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activating the shader 
        shaderProgram.Activate(); 



        // oscillating and testing with uniform 
        // float timeValue = glfwGetTime(); 
        // float offset = sin(timeValue) / 2.0f; 
        // shaderProgram.setFloat("offSet", offset); 

        // rotating, translating and scaling 
        //glm::mat4 trans = glm::mat4(1.0f); 
        // // translating it into a circular travel 
        //trans = glm::translate(trans, glm::vec3(0.5f*sin((float)glfwGetTime()), 0.5f*cos((float)glfwGetTime()), 0.0f)); 
        //trans = glm::rotate(trans, (float)glfwGetTime() * 2.0f, glm::vec3(0.0, 0.0, 1.0)); 
        //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5)); 

        // glm::mat4 model = glm::mat4(1.0f); // model matrix
        glm::mat4 view = glm::mat4(1.0f); // view matrix 
        glm::mat4 projection = glm::mat4(1.0f);  // projection matrix 


        // model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(90.0f), (float)(SCR_WIDTH/SCR_HEIGHT), 0.1f, 100.0f);

        // model = glm::rotate(model, (float)glfwGetTime() * 1.2f, glm::vec3(1.0f, 0.0f, 1.0f)); 
        // model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3)); 
        // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));


        // retrieve the matrix uniform locations
        // shaderProgram.setMat("model", 1, GL_FALSE, model); 
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
            model = glm::rotate(model, rotate, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.0, 1.0, 1.0)); 
            model = glm::translate(model, glm::vec3(-movex, -movey, movez)); 
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

