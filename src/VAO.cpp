#include "buffer/VAO.h" 

VAO::VAO()
{
    glGenVertexArrays(1, &ID); 
}

void VAO::LinkVBO(VBO VBO, GLuint layout)
{
    VBO.Bind(); 
    // position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    //glEnableVertexAttribArray(0);
    //// color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof//(GLfloat)));
    //glEnableVertexAttribArray(1);
    // VBO.Unbind(); // tutorial says it's not necessary to unbind VAOs or VBOs 
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
