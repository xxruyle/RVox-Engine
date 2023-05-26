#pragma once 
#include "mesh/mesh.h" 
#include "mesh/happly.h" 
#include "shader/shader.h"


class PLYModel   
{
public: 

    void readIn(std::string const &path); 

    void Draw(Shader& shader);  
private: 

    std::vector<Mesh> meshes; 
}; 