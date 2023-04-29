#include "buffer/VAO.h" 

VAO::VAO(VBO& VBO) 
{
    glGenVertexArrays(1, &ID); 
    VBO.Bind(); // binds the specified VBO 
}


void VAO::configVertexAttributes(GLuint layout, GLuint size, GLsizeiptr stride, GLsizeiptr offset)
{
    glVertexAttribPointer(layout, size, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)offset); 
    glEnableVertexAttribArray(layout); 
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
