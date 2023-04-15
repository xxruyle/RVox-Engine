#pragma once 
#include <glad/glad.h> 
#include "buffer/VBO.h"

class VAO 
{
    public: 
        GLuint ID; 
        VAO(); 

        void LinkVBO(VBO VBO, GLuint layout); 
        void configVertexAttributes(GLuint layout, GLuint size, GLsizeiptr stride, GLsizeiptr offset); 
        void Bind(); 
        void Unbind(); 
        void Delete(); 
}; 