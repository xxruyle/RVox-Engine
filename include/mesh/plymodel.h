#pragma once 
#include "mesh/mesh.h" 
#include "mesh/happly.h" 
#include "shader/shader.h"


class PLYModel   
{
public: 

    PLYModel(std::string const &path, glm::vec3 position, float scale) // default scale should be 10    
    {
        readIn(path, position, scale);   
    }

    void Draw(Shader& shader);  
private: 
    void readIn(std::string const &path, glm::vec3 position, float scale); 

    std::vector<Mesh> meshes; 
}; 