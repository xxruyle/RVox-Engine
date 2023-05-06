#pragma once 
#include <iostream> 
#include <cmath> 
#include <vector> 
#include <string> 
#include <unordered_map> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <noise/FastNoiseLite.h>

#include "world.h" 
#include "chunk.h"
#include "voxel.h"
#include "render/render.h" 
#include "shader/shader.h"


#define GLM_ENABLE_EXPERIMENTAL // enables hashing of glm::vec3 
#include "glm/gtx/hash.hpp"


class ChunkManager 
{ // class to manage, create, and send chunks to be rendered
private: 
    int renderDistance = 96; // the number of blocks the player can see radius wise 
    bool isNearPlayer(glm::vec3 cameraPosition, glm::vec3 chunkPosition); // checks to see if chunk is within renderDistance 


public: 
    World& world; 
    Render& renderer; 
    Camera& camera; 

    ChunkManager(World& world, Render& renderer, Camera& camera) : world(world), renderer(renderer), camera(camera) {};       
    
    std::vector<Chunk> chunks; // an unordered_map containing chunk position as key and chunk data as value  

    
    void createChunks(int randSeed); // creates the chunks and adds to the map 
    void renderChunks(Shader& shader); // renders the valid chunks in the map 

    void printCameraChunkLocation(); // prints what chunk coorddinate the camera is in  

    void printTotalVoxels(); 

    glm::vec3 brensenCast(); 
    std::vector<glm::vec3> voxelTraversal();  
    void voxelOutline(); 
    void deleteVoxel(); 
}; 