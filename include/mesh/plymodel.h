#pragma once 
#include <algorithm>
#include "mesh/mesh.h" 
#include "mesh/happly.h" 
#include "debug/debug_tools.h" 



class PLYModel   
{
public: 
    glm::vec3 mPosition; 
    AABB ModelBoundingBox; 
    float angle = 0.0f; 

    PLYModel(std::string const &path, glm::vec3 position, float scale) // default scale should be 10     
    {
        mPosition  = position; 
        readIn(path, position, scale);   
        printBoundingBoxInfo(); 
    }

    void Draw(Shader& shader);  
    void printBoundingBoxInfo(); 

private: 
    void readIn(std::string const &path, glm::vec3 position, float scale); 

    std::vector<Mesh> meshes; 
}; 