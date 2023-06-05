#pragma once 
#include <iostream> 
#include <cmath> 
#include <vector> 
#include <string> 
#include <unordered_map> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <noise/FastNoiseLite.h>

#include "chunk.h"
#include "voxel.h"
#include "render/render.h" 
#include "shader/shader.h"
#include "camera/frustum.h" 
#include "debug/debug_tools.h" 
#include "world/collision.h" 


#define GLM_ENABLE_EXPERIMENTAL // enables hashing of glm::vec3 
#include "glm/gtx/hash.hpp"
#include "glm/gtx/string_cast.hpp" 


class ChunkManager
{ // class to manage, create, and send chunks to be rendered
// TO DO: create chunk size variable  
public: 
    Render& renderer; 
    Camera& camera; 
    Frustum& frustum; 
    Collision collisionTest; 

    ChunkManager(Render& renderer, Camera& camera, Frustum& frustum) : renderer(renderer), camera(camera), frustum(frustum) {};        



    std::vector<Chunk> chunks; // a vector cainting each chunk
    std::unordered_map<glm::vec3, Chunk> chunkMap;  // an unordered map containing the chunk space coordinates as a key and chunk as value 
    std::vector<glm::vec3> chunkBuffer; // buffer for loading the chunk coordinates that are near the player   

    void createChunks(int randSeed); // creates the chunks and adds to the map 
    void renderChunks(Shader& shader); // renders the valid chunks in the map 

    void meshNeighbors(Chunk& chunk);  // gives chunks data about their neighbors 

    void checkCollision(Player& player); 
    void checkAutoJump(Player& player, glm::vec3 collisionNormal); // checks for blocks that the player can auto jump and give collider this info   
    bool checkGround(Player& player); 

    glm::vec3 getChunkLocation(glm::vec3 coordinatePosition); // prints what chunk coorddinate the camera is in  
    glm::vec3 getWorldCoordinates(glm::vec3 chunkCoordinates, glm::vec3 voxelPosition); // convert voxel position in local chunk coords to world coordinates 
    glm::vec3 getChunkCoordinates(glm::vec3 voxelPosition); 
    void printChunkLocation(); 
    void getNearChunks(); // get the nearest chunks that player is near   

    void printTotalVoxels(); 

    glm::vec3 brensenCast(); 
    std::vector<glm::vec3> voxelTraversal();  
    void voxelOutline(Shader& shader, DebugTools& debugTools);  
    void deleteVoxel(); 

    void spawnPlayer(glm::vec3 chunkCoord, Player& player);  

private: 
    static const int renderDistance = 500; // the number of blocks the player can see radius wise  
    int currentRandomSeed; // the current random seed 

    bool isNearPlayer(glm::vec3 cameraPosition, glm::vec3 chunkPosition); // checks to see if chunk is within renderDistance 


}; 