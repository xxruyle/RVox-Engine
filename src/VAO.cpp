#include "buffer/VAO.h" 

VAO::VAO()
{
    glGenVertexArrays(1, &ID); 
}

void VAO::LinkVBO(VBO VBO, GLuint layout)
{
    VBO.Bind(); 
    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0); 
    glEnableVertexAttribArray(layout); 
    VBO.Unbind(); 
}

void VAO::Bind()
{
    glBindVertexArray(ID); 
}

void VAO::Unbind()  
{
    glBindVertexArray(0); 
}

void VAO::Delete() 
{
    glDeleteVertexArrays(0, &ID); 
}
