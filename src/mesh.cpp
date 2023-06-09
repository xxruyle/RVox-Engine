#include "mesh/mesh.h" 

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)  
{
    this->vertices = vertices; 
    this->indices = indices;  

    setupMesh(); 
}


void Mesh::setupMesh() 
{
    std::cout << "Setting up mesh..." << std::endl; 
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO); 
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // vertex colors
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));   

    // Ambient Occlusion value 
    glEnableVertexAttribArray(3);	
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, aoValue));    


    glBindVertexArray(0);   
}



void Mesh::Draw(Shader& shader) 
{
    glBindVertexArray(VAO); 
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);  
    glBindVertexArray(0); 
}
