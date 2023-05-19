#pragma once 
#include <iostream> 
#include <string> 
#include <shader/shader.h> 
#include <mesh/mesh.h> 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model 
{
    public:
        Model(char *path)
        {
            loadModel(path);
        }
        void Draw(Shader &shader);	
    private:
        // model data
        std::vector<Mesh> meshes;
        std::string directory; 

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};