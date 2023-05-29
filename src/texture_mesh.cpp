#include "mesh/texture_mesh.h"

TextureMesh::TextureMesh(std::vector<vertexTEX> vertices, std::vector<unsigned int> indices, const char* textureFilePath)   
{

    texture.Init(GL_TEXTURE0); 
    texture.Bind(GL_TEXTURE_2D, GL_TEXTURE0);   
    texture.Generate(textureFilePath, 512, 512, GL_RGBA, false);     
    texture.Unbind();  

    this->vertices = vertices; 
    this->indices = indices;  

    setupTextureMesh();  
}


void TextureMesh::setupTextureMesh()   
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO); 
  

    std::cout << "Setting up Texture mesh..." << std::endl;  


    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertexTEX), &vertices[0], GL_STATIC_DRAW);  



    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW);



    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexTEX), (void*)0);



    // vertex tex coordinates  
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertexTEX), (void*)offsetof(vertexTEX, TexCoord)); 

    glBindVertexArray(0);   
}

void TextureMesh::Draw(Shader& shader)  
{
    texture.Bind(GL_TEXTURE_2D, GL_TEXTURE0);   
    glBindVertexArray(VAO); 
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);  
    glBindVertexArray(0); 
    texture.Unbind();   
}
