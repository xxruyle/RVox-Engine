#include "mesh/plymodel.h" 


void PLYModel::readIn(std::string const &path) 
{
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
        vec.x = (float)(vPos[i][0] * 10.0f);   
        vec.z = (float)(vPos[i][1] * 10.0f);    
        vec.y = (float)((vPos[i][2] * 10.0f) + 50.0f);      

        v1.Position = vec; 

        // loading in the colors 
        glm::vec3 vecColor; 
        vecColor.x = (float)(vColors[i][0] / 255.0f);   
        vecColor.y = (float)(vColors[i][1] / 255.0f);   
        vecColor.z = (float)(vColors[i][2] / 255.0f);   

        v1.Color = vecColor; 

        // adding the vertex 
        vertices.push_back(v1); 


/*         for (unsigned int j = 0; j < 3; j++) 
        {
            glm::vec3 vec; 

            std::cout << "Vertex Pos : " << vPos[i][j] << std::endl; 
        }

        for (unsigned int j = 0; j < 3; j++) 
        {
            std::cout << "vertex Color: " << (unsigned int)vColors[i][j] << std::endl;   
        }
 */

    }


    // loading the indices  
    for (unsigned int i = 0; i < faceIndices.size(); i++) 
    {
/*         for (unsigned int j = 0; j < faceIndices[i].size(); j++) 
        {
            std::cout << (unsigned int)faceIndices[i][j] << std::endl;  
        }
        std::cout << "-----" << std::endl;  */

        indices.push_back((unsigned int)faceIndices[i][0]); 

        indices.push_back((unsigned int)faceIndices[i][1]); 
        indices.push_back((unsigned int)faceIndices[i][2]); 
        indices.push_back((unsigned int)faceIndices[i][3]); 

        indices.push_back((unsigned int)faceIndices[i][2]); 
        indices.push_back((unsigned int)faceIndices[i][0]); 

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