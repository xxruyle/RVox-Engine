#include "debug/debug_tools.h"   


void DebugTools::getOutlineVertices()   
{
    std::vector<vertexTEX> vertices;   
    std::vector<unsigned int> indices; 

    for (unsigned int i = 0; i < 6; i++)  
        indices.push_back(vertices.size() + backIndices[i]);   
    for (unsigned int i = 0; i < backFace.size(); i++)  
    {
        vertexTEX v1;  
        v1.Position = backFace[i];  
        v1.TexCoord = backTEX[i]; 
        vertices.push_back(v1); 
    } 


    for (unsigned int i = 0; i < 6; i++)  
        indices.push_back(vertices.size() + frontIndices[i]);  
    for (unsigned int i = 0; i < frontFace.size(); i++)  
    {
        vertexTEX v1;  
        v1.Position = frontFace[i];  
        v1.TexCoord = frontTEX[i];  
        vertices.push_back(v1); 

    } 


    for (unsigned int i = 0; i < 6; i++)  
        indices.push_back(vertices.size() + leftIndices[i]);  
    for (unsigned int i = 0; i < leftFace.size(); i++) 
    {
        vertexTEX v1;  
        v1.Position = leftFace[i];  
        v1.TexCoord = leftTEX[i];  
        vertices.push_back(v1); 
    } 


    for (unsigned int i = 0; i < 6; i++)  
        indices.push_back(vertices.size() + rightIndices[i]);  
    for (unsigned int i = 0; i < rightFace.size(); i++) 
    {
        vertexTEX v1;  
        v1.Position = rightFace[i];  
        v1.TexCoord = rightTEX[i]; 
        vertices.push_back(v1);  
    } 


    for (unsigned int i = 0; i < 6; i++)  
        indices.push_back(vertices.size() + bottomIndices[i]); 
    for (unsigned int i = 0; i < bottomFace.size(); i++) 
    {
        vertexTEX v1;  
        v1.Position = bottomFace[i];  
        v1.TexCoord = bottomTEX[i]; 
        vertices.push_back(v1);                 
    } 


    for (unsigned int i = 0; i < 6; i++)  
        indices.push_back(vertices.size() + topIndices[i]); 
    for (unsigned int i = 0; i < topFace.size(); i++) 
    {
        vertexTEX v1;  
        v1.Position = topFace[i];  
        v1.TexCoord = topTEX[i];  
        vertices.push_back(v1);         
    } 

    TextureMesh texMesh(vertices, indices, "res/textures/outline.png");  
    meshes.push_back(texMesh);  
}


void DebugTools::drawVoxelOutline(Shader& shader, glm::vec3 position)   
{
    glm::mat4 model; 
    model = glm::mat4(1.0f); 
    model = glm::translate(model, position); 
    model = glm::scale(model, glm::vec3(1.01f, 1.01f, 1.01f)); 
    shader.setMat("model", 1, GL_FALSE, model);  
    shader.setInt("outlineTexture", 0);   

    for (unsigned int i = 0; i < meshes.size(); i++) 
    {
        meshes[i].Draw(shader);  
    }
}
