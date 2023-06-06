#include "mesh/plymodel.h" 



void PLYModel::readIn(std::string const &path, glm::vec3 position, float scale)  
{ // reading in the magica voxel ply file export 
    AABB boundingBox; 

    std::vector<Vertex> vertices; 
    std::vector<unsigned int> indices; 


    std::vector<std::array<double, 3>> vPos; 
    std::vector<std::array<unsigned char, 3>> vColors;  
    std::vector<std::vector<size_t>> faceIndices; 

    happly::PLYData plyIn(path); 

    vPos = plyIn.getVertexPositions();  
    vColors = plyIn.getVertexColors();  
    faceIndices = plyIn.getFaceIndices<size_t>(); 

    float minX = 10000.0f; 
    float maxX = -10000.0f; 

    float minY = 10000.0f; 
    float maxY = -10000.0f; 

    float minZ = 10000.0f; 
    float maxZ = -10000.0f;  


    for (unsigned int i = 0; i < vPos.size(); i++)  
    {
        Vertex v1; 

        // Loading in the vertex coordinates  
        glm::vec3 vec; 
        // when loading in big models 
        // For X: ((* 10.0f) - 0.5) + position in chunk
        // For Y: ((* 10.0f) + 0.5) + position in chunk 

        float xPos = -vPos[i][0]; 
        float yPos = vPos[i][2]; 
        float zPos = -vPos[i][1];  

        vec.x = (float)(((xPos * scale)) + position.x); // ((* scale) - offset) + position in chunk
        vec.z = (float)(((zPos * scale)) + position.z);  
        vec.y = (float)((yPos * scale) + position.y);             

        minX = std::min(minX, vec.x); 
        minY = std::min(minY, vec.y); 
        minZ = std::min(minZ, vec.z); 

        maxX = std::max(maxX, vec.x);  
        maxY = std::max(maxY, vec.y);  
        maxZ = std::max(maxZ, vec.z);  

        v1.Position = vec; 


        // loading in the colors 
        glm::vec3 vecColor; 
        vecColor.x = (float)(vColors[i][0] / 255.0f);   
        vecColor.y = (float)(vColors[i][1] / 255.0f);   
        vecColor.z = (float)(vColors[i][2] / 255.0f);   

        v1.Color = vecColor; 

        // placeholder for ambient occlusion value 
        v1.aoValue = glm::vec3(1.0f, 1.0f, 1.0f);  

        // adding the vertex 
        vertices.push_back(v1); 
    }

    // giving bounding box the min and max coordinates of the mesh  
    boundingBox.minX = minX; 
    boundingBox.startminX = minX; 

    boundingBox.maxX = maxX; 
    boundingBox.startmaxX = maxX; 

    boundingBox.startminY = minY; 
    boundingBox.minY = minY; 

    boundingBox.startmaxY = maxY; 
    boundingBox.maxY = maxY; 

    boundingBox.startminZ = minZ; 
    boundingBox.minZ = minZ; 

    boundingBox.startmaxZ = maxZ; 
    boundingBox.maxZ = maxZ; 

    ModelBoundingBox = boundingBox;  

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
    glm::mat4 model; 
    model = glm::mat4(1.0f); 
    model = glm::translate(model, mPosition); 
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));            
    shader.setMat("model", 1, GL_FALSE, model);  

    for (unsigned int i = 0; i < meshes.size(); i++) 
    {
        meshes[i].Draw(shader);  
    }
}

void PLYModel::printBoundingBoxInfo() 
{
    std::cout << "minX: " << ModelBoundingBox.minX << std::endl; 
    std::cout << "maxX: " << ModelBoundingBox.maxX << std::endl; 

    std::cout << "minY: " << ModelBoundingBox.minY << std::endl; 
    std::cout << "maxY: " << ModelBoundingBox.maxY << std::endl; 

    std::cout << "minZ: " << ModelBoundingBox.minZ << std::endl;  
    std::cout << "maxZ: " << ModelBoundingBox.maxZ << std::endl; 
    std::cout << "------" << std::endl; 
}