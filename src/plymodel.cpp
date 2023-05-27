#include "mesh/plymodel.h" 


void PLYModel::readIn(std::string const &path, glm::vec3 position, float scale)  
{ // reading in the magica voxel ply file export 
    std::vector<Vertex> vertices; 
    std::vector<unsigned int> indices; 


    std::vector<std::array<double, 3>> vPos; 
    std::vector<std::array<unsigned char, 3>> vColors;  
    std::vector<std::vector<size_t>> faceIndices; 

    happly::PLYData plyIn(path); 

    vPos = plyIn.getVertexPositions();  
    vColors = plyIn.getVertexColors();  
    faceIndices = plyIn.getFaceIndices<size_t>(); 

    for (unsigned int i = 0; i < vPos.size(); i++)  
    {
        Vertex v1; 

        // Loading in the vertex coordinates  
        glm::vec3 vec; 
        // For X: ((* 10.0f) - 0.5) + position in chunk
        // For Y: ((* 10.0f) + 0.5) + position in chunk 
        vec.x = (float)(((vPos[i][0] * scale) - 0.5f) + position.x); // ((* scale) - offset) + position in chunk
        vec.z = (float)(((vPos[i][1] * scale) + 0.5f) + position.z); 
        vec.y = (float)((vPos[i][2] * scale) + position.y);             

        v1.Position = vec; 


        // loading in the colors 
        glm::vec3 vecColor; 
        vecColor.x = (float)(vColors[i][0] / 255.0f);   
        vecColor.y = (float)(vColors[i][1] / 255.0f);   
        vecColor.z = (float)(vColors[i][2] / 255.0f);   

        v1.Color = vecColor; 

        // adding the vertex 
        vertices.push_back(v1); 
    }


    // loading the indices and adding normals  
    for (unsigned int i = 0; i < faceIndices.size(); i++) 
    {
        indices.push_back((unsigned int)faceIndices[i][2]); 
        indices.push_back((unsigned int)faceIndices[i][1]); 
        indices.push_back((unsigned int)faceIndices[i][0]); 

        indices.push_back((unsigned int)faceIndices[i][3]); 
        indices.push_back((unsigned int)faceIndices[i][2]); 
        indices.push_back((unsigned int)faceIndices[i][0]); 

        // getting the vectors 
        glm::vec3 A = vertices[(unsigned int)faceIndices[i][0]].Position - vertices[(unsigned int)faceIndices[i][1]].Position; 
        glm::vec3 B = vertices[(unsigned int)faceIndices[i][2]].Position - vertices[(unsigned int)faceIndices[i][1]].Position; 

        // getting the normal  
        glm::vec3 normal = glm::normalize(glm::cross(A, B)); 

        // assigning the normals  
        vertices[(unsigned int)faceIndices[i][2]].Normal = normal; 
        vertices[(unsigned int)faceIndices[i][1]].Normal = normal; 
        vertices[(unsigned int)faceIndices[i][0]].Normal = normal; 
        vertices[(unsigned int)faceIndices[i][3]].Normal = normal; 
        vertices[(unsigned int)faceIndices[i][2]].Normal = normal; 
        vertices[(unsigned int)faceIndices[i][0]].Normal = normal;  
    }

    std::cout << "Loading mesh data..." << std::endl;  

    Mesh mesh(vertices, indices);
    meshes.push_back(mesh); 
}

void PLYModel::Draw(Shader& shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++) 
    {
        meshes[i].Draw(shader);  
    }
}