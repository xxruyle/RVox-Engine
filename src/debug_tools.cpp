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


void DebugTools::getWireFrameVertices()   
{
    std::vector<vertexTEX> vertices;   
    std::vector<unsigned int> indices; 

    for (unsigned int i = 0; i < 6; i++)  
        indices.push_back(vertices.size() + backIndices[i]);   
    for (unsigned int i = 0; i < backFacewire.size(); i++)  
    {
        vertexTEX v1;  
        v1.Position = backFacewire[i];  
        v1.TexCoord = backTEX[i]; 
        vertices.push_back(v1); 
    } 


    for (unsigned int i = 0; i < 6; i++)  
        indices.push_back(vertices.size() + frontIndices[i]);  
    for (unsigned int i = 0; i < frontFaceWire.size(); i++)  
    {
        vertexTEX v1;  
        v1.Position = frontFaceWire[i];  
        v1.TexCoord = frontTEX[i];  
        vertices.push_back(v1); 

    } 


    for (unsigned int i = 0; i < 6; i++)  
        indices.push_back(vertices.size() + leftIndices[i]);  
    for (unsigned int i = 0; i < leftFaceWire.size(); i++) 
    {
        vertexTEX v1;  
        v1.Position = leftFaceWire[i];  
        v1.TexCoord = leftTEX[i];  
        vertices.push_back(v1); 
    } 


    for (unsigned int i = 0; i < 6; i++)  
        indices.push_back(vertices.size() + rightIndices[i]);  
    for (unsigned int i = 0; i < rightFaceWire.size(); i++) 
    {
        vertexTEX v1;  
        v1.Position = rightFaceWire[i];  
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
    for (unsigned int i = 0; i < topFaceWire.size(); i++) 
    {
        vertexTEX v1;  
        v1.Position = topFaceWire[i];   
        v1.TexCoord = topTEX[i];  
        vertices.push_back(v1);         
    } 

    TextureMesh texMesh(vertices, indices, "res/textures/outline.png");  
    meshes.push_back(texMesh);  
}


void DebugTools::drawVoxelOutline(Shader& shader, glm::vec3 position, glm::vec3 scale)   
{
    glm::mat4 model; 
    model = glm::mat4(1.0f); 
    model = glm::translate(model, position); 
    model = glm::scale(model, scale);  
    shader.setMat("model", 1, GL_FALSE, model);  
    shader.setInt("outlineTexture", 0);   

    for (unsigned int i = 0; i < meshes.size(); i++) 
    {
        meshes[i].Draw(shader);  
    }
}

void DebugTools::DrawBoundingBox(Shader& shader, glm::vec3 position) 
{
    glm::mat4 model; 
    model = glm::mat4(1.0f); 
    model = glm::translate(model, position); 
    shader.setMat("model", 1, GL_FALSE, model);  
    shader.setInt("outlineTexture", 0);   

    for (unsigned int i = 0; i < meshes.size(); i++) 
    {
        meshes[i].Draw(shader);  
    }
}

void DebugTools::getBoundingBoxVertices(BoundingBox& boundingBox) 
{
    std::vector<vertexTEX> vertices; 
    std::vector<unsigned int> indices; 
    // getting the vertices from the given bounding box min and maxes  
    // Naming convention is when looking at the box in the positive z direction

    boundingBox.calculateVertices(); 

    std::vector<glm::vec3> verticesBack = {
        boundingBox.bottomBackRight,
        boundingBox.topBackLeft, 
        boundingBox.bottomBackLeft, 
        boundingBox.topBackRight, 
    }; 

    std::vector<glm::vec3> verticesFront = {
        boundingBox.bottomFrontRight, 
        boundingBox.bottomFrontLeft, 
        boundingBox.topFrontLeft, 
        boundingBox.topFrontRight, 
    }; 

    std::vector<glm::vec3> verticesLeft = {
        boundingBox.topBackLeft, 
        boundingBox.topFrontLeft, 
        boundingBox.bottomFrontLeft,
        boundingBox.bottomBackLeft, 
    }; 

    std::vector<glm::vec3> verticesRight = {
        boundingBox.topFrontRight, 
        boundingBox.bottomBackRight, 
        boundingBox.topBackRight, 
        boundingBox.bottomFrontRight  
    }; 

    std::vector<glm::vec3> verticesBottom = {
        boundingBox.bottomFrontRight, 
        boundingBox.bottomFrontLeft, 
        boundingBox.bottomBackLeft, 
        boundingBox.bottomBackRight, 
    }; 

    std::vector<glm::vec3> verticesTop = {
        boundingBox.topFrontRight,
        boundingBox.topBackLeft, 
        boundingBox.topFrontLeft, 
        boundingBox.topBackRight, 
    }; 

    // creating the mesh 

    // back 

    for (unsigned int i = 0; i < 6; i++)  
        indices.push_back(vertices.size() + backIndices[i]);   
    for (unsigned int i = 0; i < verticesBack.size(); i++)  
    {
        vertexTEX v1;  
        v1.Position = verticesBack[i];  
        v1.TexCoord = backTEX[i]; 
        vertices.push_back(v1); 
    } 


    for (unsigned int i = 0; i < 6; i++)  
        indices.push_back(vertices.size() + frontIndices[i]);  
    for (unsigned int i = 0; i < verticesFront.size(); i++)  
    {
        vertexTEX v1;  
        v1.Position = verticesFront[i];  
        v1.TexCoord = frontTEX[i];  
        vertices.push_back(v1); 

    } 


    for (unsigned int i = 0; i < 6; i++)  
        indices.push_back(vertices.size() + leftIndices[i]);  
    for (unsigned int i = 0; i < verticesLeft.size(); i++) 
    {
        vertexTEX v1;  
        v1.Position = verticesLeft[i];   
        v1.TexCoord = leftTEX[i];  
        vertices.push_back(v1); 
    } 


    for (unsigned int i = 0; i < 6; i++)  
        indices.push_back(vertices.size() + rightIndices[i]);  
    for (unsigned int i = 0; i < verticesRight.size(); i++) 
    {
        vertexTEX v1;  
        v1.Position = verticesRight[i];   
        v1.TexCoord = rightTEX[i]; 
        vertices.push_back(v1);  
    } 


    for (unsigned int i = 0; i < 6; i++)  
        indices.push_back(vertices.size() + bottomIndices[i]); 
    for (unsigned int i = 0; i < verticesBottom.size(); i++) 
    {
        vertexTEX v1;  
        v1.Position = verticesBottom[i];  
        v1.TexCoord = bottomTEX[i]; 
        vertices.push_back(v1);                 
    } 


    for (unsigned int i = 0; i < 6; i++)  
        indices.push_back(vertices.size() + topIndices[i]); 
    for (unsigned int i = 0; i < verticesTop.size(); i++) 
    {
        vertexTEX v1;  
        v1.Position = verticesTop[i];    
        v1.TexCoord = topTEX[i];  
        vertices.push_back(v1);         
    } 

    TextureMesh texMesh(vertices, indices, "res/textures/outline.png");  
    meshes.push_back(texMesh);   
}
