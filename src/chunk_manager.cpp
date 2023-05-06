#include "world/world.h" 
#include "world/chunk_manager.h" 
#include "world/chunk.h"

void ChunkManager::createChunks(int randSeed) 
{ // creates chunks and makes them generate a chunk 
    chunks.clear(); 
    for (int i = 0; i < 25; i++) 
    {
        for (int j = 0; j < 25; j++) 
        {
            Chunk c1; 
            c1.position = glm::vec3(i, 0, j);  
            // std::cout << c1.position.x << ' ' << c1.position.z << std::endl; 
            c1.generate(randSeed, c1.position.x * 32, c1.position.z * 32);
            chunks.push_back(c1);  
        }
    }
}

void ChunkManager::renderChunks(Shader& shader)  
{ // renders the existing chunks 
    for (auto& chunk : chunks)  
    {
        for (unsigned int i = 0; (i < chunk.voxels.size()); i++)   
        {
            if (isNearPlayer(camera.mPosition, chunk.position))
            {
                renderer.drawVoxel(shader, chunk.voxels[i].coordinates, chunk.voxels[i].color, 1.0f);   

            }
        }
    }
}


void ChunkManager::printCameraChunkLocation() 
{
    std::cout << "ChunkCoord X : " << floor(camera.mPosition.x / 32) << "| ChunkCoord Z: " << floor(camera.mPosition.z / 32) << std::endl;  
}

void ChunkManager::printTotalVoxels()  
{
    int sum = 0;  
    for (auto chunk : chunks) 
    {
        sum += chunk.voxels.size(); 
    }

    std::cout << "Total Voxels: " << sum << std::endl;  
}


bool ChunkManager::isNearPlayer(glm::vec3 cameraPosition, glm::vec3 chunkPosition) 
{ // checks to see if chunk is within renderDistance 

    // converting chunk position to world coordinates 
    glm::vec3 chunkWorldCoordinates = glm::vec3((chunkPosition.x * 32) + 16, chunkPosition.y, (chunkPosition.z*32) + 16);  
    

    glm::vec3 distanceVector = glm::vec3(cameraPosition.x, 0, cameraPosition.z) - glm::vec3(chunkWorldCoordinates.x, 0, chunkWorldCoordinates.z);   

    return glm::length(distanceVector) < renderDistance;   
} 


glm::vec3 ChunkManager::brensenCast() 
{
    std::vector<glm::vec3> voxelCoords = voxelTraversal(); 

    for (auto& chunk : chunks) 
    {
        for (unsigned int i = 0; i < voxelCoords.size(); i++) 
        {
            for (unsigned int j = 0; j < chunk.voxels.size(); j++) 
            {
                if (chunk.voxels[j].coordinates == voxelCoords[i]) 
                    return voxelCoords[i];
            }
        }
    }

    // std::cout << "No block casted" << std::endl; 
    return glm::vec3(0); 
}

#define SIGN(x) (x > 0 ? 1 : (x < 0 ? -1 : 0))
#define FRAC0(x) (x - floorf(x))
#define FRAC1(x) (1 - x + floorf(x))

std::vector<glm::vec3> ChunkManager::voxelTraversal()  
{
    std::vector<glm::vec3> voxelRayIntersections; 

    float castLength = 4.0; 

    // we add 0.5f here to offset the grid layout that happens when placing blocks (the origin of a block is the top left of the cube)
    float xOrigin; 
    float yOrigin; 
    float zOrigin; 

    // if in negative space, shift the grid in negative direction, vice versa 
    camera.mPosition.x > 0 ? xOrigin = camera.mPosition.x + 0.5f : xOrigin = camera.mPosition.x - 0.5f; 
    camera.mPosition.y > 0 ? yOrigin = camera.mPosition.y + 0.5f : yOrigin = camera.mPosition.y - 0.5f; 
    camera.mPosition.z > 0 ? zOrigin = camera.mPosition.z + 0.5f : zOrigin = camera.mPosition.z - 0.5f;

    // voxelRayIntersections.push_back(glm::vec3(floor(xOrigin), floor(yOrigin), floor(zOrigin))); 

    // 4 magnitude vector in direction of the ray 
    float xEnd = xOrigin + (camera.mFront.x * castLength); 
    float yEnd = yOrigin + (camera.mFront.y * castLength); 
    float zEnd = zOrigin + (camera.mFront.z * castLength); 

    voxelRayIntersections.push_back(glm::vec3(xEnd, yEnd, zEnd)); 



    float tMaxX, tMaxY, tMaxZ; // the value of t which the ray crosses the first vertical voxel boundary 
    float tDeltaX, tDeltaY, tDeltaZ; // how far along the ray we must move in t for the component to equal the width of the voxel 


    // coordinate positions 

    int dx = SIGN(xEnd - xOrigin);
    if (dx != 0) tDeltaX = fmin(dx / (xEnd - xOrigin), 10000000.0f); else tDeltaX = 10000000.0f;
    if (dx > 0) tMaxX = tDeltaX * FRAC1(xOrigin); else tMaxX = tDeltaX * FRAC0(xOrigin);
    int x = (int)xOrigin; 

    int dy = SIGN(yEnd - yOrigin);
    if (dy != 0) tDeltaY = fmin(dy / (yEnd - yOrigin), 10000000.0f); else tDeltaY = 10000000.0f;
    if (dy > 0) tMaxY = tDeltaY * FRAC1(yOrigin); else tMaxY = tDeltaY * FRAC0(yOrigin);
    int y = (int)yOrigin; 

    int dz = SIGN(zEnd - zOrigin);
    if (dz != 0) tDeltaZ = fmin(dz / (zEnd - zOrigin), 10000000.0f); else tDeltaZ = 10000000.0f;
    if (dz > 0) tMaxZ = tDeltaZ * FRAC1(zOrigin); else tMaxZ = tDeltaZ * FRAC0(zOrigin);
    int z = (int)zOrigin; 

    for (int i = 0; i < 7; i++) { 
        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                x += dx;
                tMaxX += tDeltaX;
            } else {
                z += dz;
                tMaxZ += tDeltaZ;
            }
        } else {
            if (tMaxY < tMaxZ) {
                y += dy;
                tMaxY += tDeltaY;
            } else {
                z += dz;
                tMaxZ += tDeltaZ;
            }
        }
        // if (tMaxX > 1 && tMaxY > 1 && tMaxZ > 1) break;
        voxelRayIntersections.push_back(glm::vec3(x, y, z)); 
    }


/*     for (unsigned int i = 0; i < voxelRayIntersections.size(); i++)
    {
        std::cout << voxelRayIntersections[i].x << " " << voxelRayIntersections[i].y << " " << voxelRayIntersections[i].z << std::endl; 
    }
    std::cout << "-------" << std::endl;  */

    return voxelRayIntersections; 
}

void ChunkManager::voxelOutline() 
{ // changes color of voxel to voxel outline color if casted
 
// TO DO: OPTIMIZE BETTER, BAD FRAMEDROPS WHEN ENABLED  
    glm::vec3 voxel_position = brensenCast(); 
    for (auto& chunk : chunks) 
    {
        for (unsigned int i = 0; i < chunk.voxels.size(); i++) 
        {
            if (chunk.voxels[i].coordinates == voxel_position) 
            {
                chunk.voxels[i].color = chunk.voxels[i].outlineColor;  
            } else {
                chunk.voxels[i].color = chunk.voxels[i].colorCopy;  
            }
        }
    }

}

void ChunkManager::deleteVoxel()
{
    glm::vec3 voxel_position = brensenCast(); 
    for (auto& chunk : chunks) 
    {
        for (unsigned int i = 0; i < chunk.voxels.size(); i++) 
        {
            if (chunk.voxels[i].coordinates == voxel_position)
            {
                chunk.voxels.erase(chunk.voxels.begin() + i);  
                break; 
            }
        }
    }

}