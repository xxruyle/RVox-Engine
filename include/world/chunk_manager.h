#pragma once 
#include <iostream> 
#include <cmath> 
#include <vector> 
#include <string> 
#include <utility> // for pair 
#include <queue> 
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


class ChunkManager : World 
{ // class to manage, create, and send chunks to be rendered
// TO DO: create chunk size variable  
// TO DO: ADD FAST LOOKUP TO VOXEL COORDINATES ISNTEAD OF LOOPING OVER THEM AND CHECKING EACH TIME 
private: 
    static const int renderDistance = 96; // the number of blocks the player can see radius wise  
    int currentRandomSeed; // the current random seed 

    bool isNearPlayer(glm::vec3 cameraPosition, glm::vec3 chunkPosition); // checks to see if chunk is within renderDistance 


public: 
    World& world; 
    Render& renderer; 
    Camera& camera; 

    ChunkManager(World& world, Render& renderer, Camera& camera) : world(world), renderer(renderer), camera(camera) {};        



    std::vector<Chunk> chunks; // a vector cainting each chunk
    std::unordered_map<glm::vec3, Chunk> chunkMap;  // an unordered map containing the chunk space coordinates as a key and chunk as value 
    // std::vector<glm::vec3> chunkBuffer; // buffer for loading the chunk coordinates that are near the player  
    glm::vec3 chunkBuffer[((renderDistance / 32) * 2) * ((renderDistance / 32) * 2)];    
    
    void createChunks(int randSeed); // creates the chunks and adds to the map 
    void renderChunks(Shader& shader); // renders the valid chunks in the map 

    void renderOneVoxel(Shader& shader);  

    glm::vec3 getChunkLocation(glm::vec3 coordinatePosition); // prints what chunk coorddinate the camera is in  
    void printChunkLocation(); 
    void getNearChunks(); // get the nearest chunks that player is near   

    void printTotalVoxels(); 

    glm::vec3 brensenCast(); 
    std::vector<glm::vec3> voxelTraversal();  
    void voxelOutline(); 
    void deleteVoxel(); 

    // initializing the prev outlined voxel for voxelOutline. Not the most elegant solution for the voxelOutline bug, but whatever lol 
    Voxel* prevOutlinedVoxel = nullptr; 

}; 