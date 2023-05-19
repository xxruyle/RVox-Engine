#include "world/chunk_manager.h"  


void ChunkManager::createChunks(int randSeed) 
{ // creates chunks and makes them generate a chunk 
    chunkMap.clear(); 
    world.worldMap.clear(); 
    currentRandomSeed = randSeed; 
    for (int i = 0; i < world.wSizeX; i++) 
    {
        for (int j = 0; j < world.wSizeZ; j++)  
        {
            Chunk& c1 = chunkMap[glm::vec3(i, 0, j)]; 
            c1.position = glm::vec3(i, 0, j);  
            c1.generateSolidChunk(randSeed, c1.position.x * 32, c1.position.z * 32);     
            // c1.generate(randSeed, c1.position.x * 32, c1.position.z * 32); 
            // c1.generateDebugChunk(randSeed);   
            c1.mesh();  
        }
    }
}



void ChunkManager::renderChunks(Shader& shader)  
{ // renders the existing chunks 

    // looping over every chunk 
/*     for (auto& chunk: chunkMap) 
    {
        if (isNearPlayer(camera.mPosition, chunk.first)) //chunk.first is the vec3 coordinates which is the key of the chunkMap 
        {
            
            for (auto voxel : chunk.second.voxelMap) // looping over each voxel in a voxel map 
            {
                
                // if (!(voxel.second.isInterior)) // if the voxel is not an interior voxel  
                    
                    renderer.drawVoxel(shader, voxel.second.coordinates, voxel.second.color, 1.0f); 
            }
        }
    } 

    // infinite terrain generation 
    chunkBuffer.clear(); 
    getNearChunks(); 

    for (unsigned int i = 0; i < chunkBuffer.size(); i++)  
    {
        if (!(chunkMap.count(chunkBuffer[i])))
        {
            Chunk& c1 = chunkMap[chunkBuffer[i]];
            c1.world = &world; 
            
            c1.position = chunkBuffer[i]; 
            c1.generate(currentRandomSeed, c1.position.x * 32, c1.position.z * 32); 
        } 
    } */

    // looping over chunks that are within player render distance  
    chunkBuffer.clear(); 
    getNearChunks(); 

    for (unsigned int i = 0; i < chunkBuffer.size(); i++) 
    {
        if (chunkMap.count(chunkBuffer[i])) // if the coordinates exist in the world    
        {
            chunkMap[chunkBuffer[i]].draw(shader, frustum);   


            // if (isNearPlayer(camera.mPosition, chunkBuffer[i])) // final check to see if player is within render distance chunk 
            // {
/*                 for (auto& voxel : chunkMap[chunkBuffer[i]].voxelMap) // looping over each voxel in a voxel map 
                {
                    if (!(voxel.second.isInterior)) // if the voxel is not an interior voxel   
                        renderer.drawVoxel(shader, voxel.second.coordinates, voxel.second.color, 1.0f); 
                } */
            // }
        }  else { // if coordinate's do not already exist in the world, keep generating. (Allows for "infinite" terrain generation)  
            Chunk& c1 = chunkMap[chunkBuffer[i]]; 
            c1.position = chunkBuffer[i]; 
            c1.generateSolidChunk(currentRandomSeed, c1.position.x * 32, c1.position.z * 32);   
            c1.mesh(); 
        }
    } 
}


void ChunkManager::renderOneVoxel(Shader& shader)  
{ // for debugging 
    Voxel voxel(glm::vec3(0.0,0.0,0.0), glm::vec3(1.0, 0.5, 0.31), 1);     
    renderer.drawVoxel(shader, voxel.coordinates, voxel.color, 1.0f);  
}

glm::vec3 ChunkManager::getChunkLocation(glm::vec3 coordinatePosition)  
{
    int xChunkPos = floor(coordinatePosition.x / 32); 
    int zChunkPos = floor(coordinatePosition.z / 32); 

    return glm::vec3(xChunkPos, 0, zChunkPos); 
}

void ChunkManager::printChunkLocation()
{
    int xChunkPos = floor(camera.mPosition.x / 32); 
    int zChunkPos = floor(camera.mPosition.z / 32);  

    std::cout << "X: " << xChunkPos << ' ' << "Z: " << zChunkPos << std::endl; 

}

void ChunkManager::getNearChunks() 
{ // gets the nearest available chunks that the camera can possibly render and pushes it on a queue 
    // std::vector<std::pair<int, int>> nearChunks; // a vector full of pairs 
    int xPos = floor(camera.mPosition.x / 32); // the chunk coordinate of the x position 
    int zPos = floor(camera.mPosition.z / 32);  // the chunk coordinate of the z position 
    int maxDistance = (int)(renderDistance / 32); // calculating the max amount of chunks the player can see radius wise with respect to chunk coordinates  

    for (int i = -maxDistance; i < maxDistance; i++)  // going from left of the grid to the right of the grid to get all of the nearest chunks near camera 
    {
        for (int j = -maxDistance; j < maxDistance; j++) // going from bottom to top of the grid 
        {
            // std::pair<int,int> point = std::make_pair(xPos + i, zPos + j);  // a point in the grid 
            // nearChunks.push_back(point);
            chunkBuffer.push_back(glm::vec3(xPos + i, 0, zPos + j)); 

        }
    }



/*     // getting the grid 
    for (unsigned int i = 0; i < nearChunks.size(); i++) 
    {
        // std::cout << nearChunks[i].first << ' ' << nearChunks[i].second << std::endl; 
        chunkBuffer.push_back(glm::vec3(nearChunks[i].first, 0, nearChunks[i].second)); 
    } */
}

void ChunkManager::printTotalVoxels()  
{
    int sum = 0;  
    for (auto& chunk : chunkMap) 
    {
        sum += chunk.second.voxelMap.size();  
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

    glm::vec3 chunkCoord; 

    for (unsigned int i = 0; i < voxelCoords.size(); i++) 
    {
        chunkCoord = getChunkLocation(voxelCoords[i]); 
/*         for (unsigned int j = 0; j < chunkMap[chunkCoord].voxels.size(); j++) 
        {
            if (chunkMap[chunkCoord].voxels[j].coordinates == voxelCoords[i])  
            {
                return voxelCoords[i]; 
            }
        } */
        if (chunkMap[chunkCoord].voxelMap.count(voxelCoords[i])) // if there are coordinates with the voxel coords in the voxelmap 
        {
            return voxelCoords[i]; 
        }
    }  

    return glm::vec3(0, 1000, 0); 
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

    // voxel traversal algorithm 
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

/* void ChunkManager::voxelOutline() 
{ // changes color of voxel to voxel outline color if casted
    glm::vec3 voxelPosition = brensenCast(); 
    glm::vec3 chunkCoord = getChunkLocation(voxelPosition);   
    if (voxelPosition != glm::vec3(0, 1000, 0)) 
    {
        for (unsigned int i = 0; i < chunkMap[chunkCoord].voxels.size(); i++) 
        {
            if (chunkMap[chunkCoord].voxels[i].coordinates == voxelPosition)  
            {
                prevOutlinedVoxel = &(chunkMap[chunkCoord].voxels[i]); 
                chunkMap[chunkCoord].voxels[i].color = chunkMap[chunkCoord].voxels[i].outlineColor;
            }    
            else 
            {   
                chunkMap[chunkCoord].voxels[i].color = chunkMap[chunkCoord].voxels[i].colorCopy;
            }
        }
    } else {
        if (prevOutlinedVoxel != nullptr) 
        {
            prevOutlinedVoxel->color = prevOutlinedVoxel->colorCopy;
        }
             
    }
} */

void ChunkManager::deleteVoxel()
{
    glm::vec3 voxelPosition = brensenCast(); 

    glm::vec3 chunkCoord = getChunkLocation(voxelPosition);   
    // std::cout << chunkCoord.x << ' ' << chunkCoord.y << ' ' << chunkCoord.z << std::endl; 
/*     for (unsigned int i = 0; i < chunkMap[chunkCoord].voxels.size(); i++) 
    {
        if (chunkMap[chunkCoord].voxels[i].coordinates == voxelPosition)  
        {
            chunkMap[chunkCoord].voxels.erase(chunkMap[chunkCoord].voxels.begin() + i); 
            break; 
        }
    } */
    std::cout << voxelPosition.x << ' ' << voxelPosition.y << ' ' << voxelPosition.z << std::endl;  
    chunkMap[chunkCoord].voxelMap.erase(voxelPosition); 
    chunkMap[chunkCoord].mesh();   

}