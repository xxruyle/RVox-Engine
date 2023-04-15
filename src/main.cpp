#include <iostream> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <cmath>

#include "shader/shader.h" 
#include "buffer/VBO.h"
#include "buffer/VAO.h" 
#include "buffer/EBO.h" 
#include "texture/texture.h"


// Vertices coordinates for zelda triforce 
GLfloat vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

GLfloat texCoords[] = {
    0.0f, 0.0f,  // lower-left corner  
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
}; 

GLuint indices[] = {  // note that we start from 0!
    1, 2, 3, 
    0, 1, 3,  // sole triangle
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    // Whenever the window changes in size GLFW calls this function
    glViewport(0, 0, width, height); 
}

void processInput(GLFWwindow* window) 
{
    // handles user input
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); 

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // wireframe 
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // solid
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
    GLFWwindow* window = glfwCreateWindow(1000, 1000, "learnopengl", NULL, NULL); 
    if (window == NULL) 
    {
        std::cout << "Failed to create GLFW window" << std::endl; 
        glfwTerminate(); 
        return -1; 
    }

    glfwMakeContextCurrent(window); 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // updates window size

    gladLoadGL(); 

    glViewport(0,0, 1000, 1000); 

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
    VAO1.configVertexAttributes(0, 8, 0); 
    VAO1.configVertexAttributes(1, 8, 3*sizeof(GLfloat)); 
    VAO1.configVertexAttributes(2, 8, 6*sizeof(GLfloat)); 


    Texture TEX; 
    TEX.Bind(); 
    TEX.setParameters(); 
    TEX.Generate("res/textures/grass_texture.jpg", 512, 512); 


    // The main render loop 
    while (!glfwWindowShouldClose(window)) 
    {
        // input 
        processInput(window); 

        // rendering commands 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT); 


        // activating the shader 
        shaderProgram.Activate(); 
        TEX.Bind(); 
        VAO1.Bind();


        // oscillating and testing with uniform 
        float timeValue = glfwGetTime(); 
        float offset = sin(timeValue) / 2.0f; 
        shaderProgram.setFloat("offSet", offset); 

        //glDrawArrays(GL_TRIANGLES, 0, 3); // When not using EBO 
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // When using EBO 
    


        glfwSwapBuffers(window); // swaps the color buffer which is used to render during each render iteration and show output to the screen 
        glfwPollEvents(); 
    }


    VAO1.Delete(); 
    VBO1.Delete(); 
    EBO1.Delete(); 
    shaderProgram.Delete(); 


    glfwDestroyWindow(window);
    glfwTerminate(); 

    return 0; 
}

