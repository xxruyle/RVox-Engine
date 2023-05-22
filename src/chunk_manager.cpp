#include "world/chunk_manager.h"  


void ChunkManager::createChunks(int randSeed) 
{ // creates chunks and makes them generate a chunk 
    chunkMap.clear(); 
    world.worldMap.clear(); 

    currentRandomSeed = randSeed; 
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++)  
        {
            Chunk& c1 = chunkMap[glm::vec3(i, 0, j)]; 
            c1.position = glm::vec3(i, 0, j);  
            c1.generateSolidChunk(randSeed, c1.position.x * 32, c1.position.z * 32);     
            c1.mesh(); 
            meshNeighbors(c1); 

            // std::cout << sizeof(c1) << std::endl;   

        }
    }
}



void ChunkManager::renderChunks(Shader& shader)  
{ // renders the existing chunks 

    // looping over chunks that are within camera area  
    chunkBuffer.clear(); 
    getNearChunks(); 

    for (unsigned int i = 0; i < chunkBuffer.size(); i++) 
    {
        if (chunkMap.count(chunkBuffer[i])) // if the coordinates exist in the world    
        {
            if (isNearPlayer(camera.mPosition, chunkBuffer[i]))  
                chunkMap[chunkBuffer[i]].draw(shader, frustum);
            

        }  else if ((isNearPlayer(camera.mPosition, chunkBuffer[i]))){ // if coordinate's do not already exist in the world, keep generating. (Allows for "infinite" terrain generation)  
            Chunk& c1 = chunkMap[chunkBuffer[i]]; 
            c1.position = chunkBuffer[i]; 
            c1.generateSolidChunk(currentRandomSeed, c1.position.x * 32, c1.position.z * 32);   
            c1.mesh(); 
            // meshNeighbors(c1);  
        }
    } 
}


void ChunkManager::meshNeighbors(Chunk& chunk) 
{
    // top neighbor 
    if (chunkMap.count(glm::vec3(chunk.position.x, chunk.position.y, chunk.position.z + 1))) // z remains constant 
    {
        // give chunkmap chunk border info 
        for (unsigned int x = 0; x < 33; x++)   
        {
            for (unsigned int y = 0; y < 256; y++) 
            {   
                chunkMap[glm::vec3(chunk.position.x, chunk.position.y, chunk.position.z + 1)].voxels[x][y][0] = chunk.voxels[x][y][32];       // update chunk's  neighbor's border  
            }
        }
        

 
        chunkMap[glm::vec3(chunk.position.x, chunk.position.y, chunk.position.z + 1)].mesh(); 
    }

    // left neighbor 
    if (chunkMap.count(glm::vec3(chunk.position.x + 1, chunk.position.y, chunk.position.z))) // x remains constant 
    {
        // give chunkmap chunk border info 
        for (unsigned int z = 0; z < 33; z++)   
        {
            for (unsigned int y = 0; y < 256; y++) 
            {   
                chunkMap[glm::vec3(chunk.position.x + 1, chunk.position.y, chunk.position.z)].voxels[0][y][z] = chunk.voxels[32][y][z];    // update chunk's  neighbor's border 
            }
        }
        
        chunkMap[glm::vec3(chunk.position.x + 1, chunk.position.y, chunk.position.z)].mesh(); 



    }



    // right neighbor 
    if (chunkMap.count(glm::vec3(chunk.position.x - 1, chunk.position.y, chunk.position.z))) // using the given chunks current position to find its neighbors  
    {
        // give chunkmap chunk border info 
        for (unsigned int z = 0; z < 33; z++) // only need to go over the z axis since the x axis remains constant  
        {
            for (unsigned int y = 0; y < 256; y++) 
            {   
                int val = chunk.voxels[0][y][z]; 
                chunkMap[glm::vec3(chunk.position.x - 1, chunk.position.y, chunk.position.z)].voxels[32][y][z] = val;       // update chunk's  neighbor's border  
            }
        }
        

        // and then mesh the right neighbor as well 
        chunkMap[glm::vec3(chunk.position.x - 1, chunk.position.y, chunk.position.z)].mesh(); 

        // finally, we mesh the chunk itself 

    }

    // bottom neighbor  
    if (chunkMap.count(glm::vec3(chunk.position.x, chunk.position.y, chunk.position.z - 1))) // z remains constant 
    {
        // give chunkmap chunk border info 
        for (unsigned int x = 0; x < 33; x++)   
        {
            for (unsigned int y = 0; y < 256; y++) 
            {   
                chunkMap[glm::vec3(chunk.position.x, chunk.position.y, chunk.position.z - 1)].voxels[x][y][32] = chunk.voxels[x][y][0];     // update chunk's  neighbor's border 
            }
        }
        

        // finally, we mesh the chunk itself 
        // and then mesh the right neighbor as well 
        chunkMap[glm::vec3(chunk.position.x, chunk.position.y, chunk.position.z - 1)].mesh(); 
    }

    // mesh the chunk itself 
    chunk.mesh(); 
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
    int xPos = floor(camera.mPosition.x / 32); // the chunk coordinate of the x position 
    int zPos = floor(camera.mPosition.z / 32);  // the chunk coordinate of the z position 
    int maxDistance = (int)(renderDistance / 32); // calculating the max amount of chunks the player can see radius wise with respect to chunk coordinates  

    for (int i = -maxDistance; i < maxDistance; i++)  // going from left of the grid to the right of the grid to get all of the nearest chunks near camera 
    {
        for (int j = -maxDistance; j < maxDistance; j++) // going from bottom to top of the grid 
        {
            chunkBuffer.push_back(glm::vec3(xPos + i, 0, zPos + j)); 
        }
    }
}

void ChunkManager::printTotalVoxels()  
{ // prints the total amount of voxels (including air) 
    int sum = 0;  
    for (auto& chunk : chunkMap) 
    {
        sum += sizeof(chunk.second.voxels) / sizeof(int); 
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

    for (unsigned int i = 1; i < voxelCoords.size(); i++) 
    {
        chunkCoord = getChunkLocation(voxelCoords[i]); 

        int xCoord = (int)voxelCoords[i].x - (int)chunkMap[chunkCoord].position.x * 32; 
        int yCoord = (int)voxelCoords[i].y - (int)chunkMap[chunkCoord].position.y * 32; 
        int zCoord = (int)voxelCoords[i].z - (int)chunkMap[chunkCoord].position.z * 32; 

        if (xCoord >= 0 && yCoord >= 0 && zCoord >= 0) 
        {
            if (chunkMap[chunkCoord].voxels[xCoord][yCoord][zCoord] == 1)  
            {
                return glm::vec3((int)voxelCoords[i].x, (int)voxelCoords[i].y, (int)voxelCoords[i].z);    
            }
        }
    }  
    return glm::vec3(0, 1000, 0); // returning junk if no voxel was found 
}

#define SIGN(x) (x > 0 ? 1 : (x < 0 ? -1 : 0))
#define FRAC0(x) (x - floorf(x))
#define FRAC1(x) (1 - x + floorf(x))

std::vector<glm::vec3> ChunkManager::voxelTraversal()  
{  // uses fast voxel traversal algorithm to raytrace, returns a vector of voxels that the ray traveled over 
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
        voxelRayIntersections.push_back(glm::vec3(x, y, z)); 
    }


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
    std::cout << chunkCoord.x << ' ' << chunkCoord.z << std::endl; // for debugging purposes

    int xCoord = (int)voxelPosition.x - (int)chunkMap[chunkCoord].position.x * 32; 
    int yCoord = (int)voxelPosition.y - (int)chunkMap[chunkCoord].position.y * 32; 
    int zCoord = (int)voxelPosition.z - (int)chunkMap[chunkCoord].position.z * 32;  

    // std::cout << voxelPosition.x << ' ' << voxelPosition.y << ' ' << voxelPosition.z << std::endl; // for debugging purposes  
    std::cout << xCoord << ' ' << yCoord << ' ' << zCoord << std::endl; // for debugging purposes  
    chunkMap[chunkCoord].voxels[xCoord][yCoord][zCoord] = 0;    
    chunkMap[chunkCoord].mesh();   

    if (xCoord == 0 || zCoord == 0 || xCoord == 32 || zCoord == 32)
        meshNeighbors(chunkMap[chunkCoord]); 

}