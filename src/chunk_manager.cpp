#include "world/chunk_manager.h"  


void ChunkManager::createChunks(int randSeed) 
{ // creates chunks and makes them generate a chunk 
    chunkMap.clear(); 

    currentRandomSeed = randSeed; 
    for (int i = 0; i < 1; i++) 
    {
        for (int j = 0; j < 1; j++)    
        {
            Chunk& c1 = chunkMap[glm::vec3(i, 0, j)]; 
            c1.position = glm::vec3(i, 0, j);  
            c1.generateSolidChunk(randSeed, c1.position.x * 32, c1.position.z * 32);         
            // c1.generateDebugChunk(randSeed, c1.position.x * 32, c1.position.z * 32);     
            c1.mesh(); 
            meshNeighbors(c1); 
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
            {
                chunkMap[chunkBuffer[i]].draw(shader, frustum, renderDistance);
            }
                
            

        }  else if ((isNearPlayer(camera.mPosition, chunkBuffer[i]))){ // if coordinate's do not already exist in the world, keep generating. (Allows for "infinite" terrain generation)  
            Chunk& c1 = chunkMap[chunkBuffer[i]]; 
            c1.position = chunkBuffer[i]; 
            c1.generateSolidChunk(currentRandomSeed, c1.position.x * 32, c1.position.z * 32);     
            // c1.generateDebugChunk(currentRandomSeed, c1.position.x * 32, c1.position.z * 32);   
            c1.mesh(); 
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
                chunkMap[glm::vec3(chunk.position.x, chunk.position.y, chunk.position.z + 1)].voxels[x][y][0] = chunk.voxels[x][y][32]; // the index for the block that needs to be changed is chunk size + 1 - 1 (which is chunk size duh)       // update chunk's  neighbor's border  
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

bool ChunkManager::checkGround(Player& player)  
{ // checks every block beneath the player, if there is at least one block, set player onGround bool to true 
    AABB* playerAABB = &(player.playerModel->ModelBoundingBox); 
    int y = floor(playerAABB->minY + 0.48999f);  // the offsets here are the voxel aabb extents         
    int xmin = floor(playerAABB->minX + 0.48f);  
    int zmin = floor(playerAABB->minZ + 0.48f);  
    int xmax = ceil(playerAABB->maxX - 0.48f);     
    int zmax = ceil(playerAABB->maxZ - 0.48f);          

    for (int x = xmin; x <=xmax; x++) 
    {
        for (int z = zmin; z <=zmax; z++)  
        {
            glm::vec3 underVoxel = getChunkCoordinates(glm::vec3(x, y, z)); 
            glm::vec3 playerChunk = getChunkLocation(glm::vec3(x, y, z)); 
            if ((chunkMap[playerChunk].voxels[(int)underVoxel.x][(int)underVoxel.y][(int)underVoxel.z] > 0)) // if the block is solid 
            {
                return true;  // if at least one of the blocks is solid return true 
            }
        }
    }

    return false;   
}


// An entity can auto jump if there is no block above another block that the player is colliding against
// An entity's maxY must also be taller than the one block height. for this to occur 
// TODO: Increase the maxY for entities smaller than one block heights? 
void ChunkManager::checkAutoJump(Player& player, glm::vec3 collisionNormal)  
{  
    AABB* playerAABB = &(player.playerModel->ModelBoundingBox); // get the entity bounding box
    glm::vec3 playerDirection = -collisionNormal; // opposite of the collision normal direction is theplayer direction
    
    int maxY = floor(playerAABB->maxY); // the y value the the potential block will have      
    glm::vec3 potentialBlock; 

    // 4 posibilites of the collision normal, normalized to the x, z space 
    if (playerDirection == glm::vec3(0, 0, 1)) // positive z direction 
    {
        potentialBlock = glm::vec3(player.mPosition.x, maxY, ceil(playerAABB->maxZ));   
    } 
    else if (playerDirection == glm::vec3(-1, 0, 0)) // negative x dir 
    {
        potentialBlock = glm::vec3(floor(playerAABB->minX), maxY, player.mPosition.z);   
    }
    else if (playerDirection == glm::vec3(0, 0, -1)) // negative z directon 
    {
        potentialBlock = glm::vec3(player.mPosition.x, maxY, floor(playerAABB->minZ));   
    }
    else  // positive x dir 
    {
        potentialBlock = glm::vec3(ceil(playerAABB->maxX), maxY, player.mPosition.z);    
    }

    // check to see if there is no block at potential block location, if so return true 
    glm::vec3 potentialBlockChunk = getChunkLocation(potentialBlock);   
    glm::vec3 potentialBlockCoords = getChunkCoordinates(potentialBlock); 
    if (!(chunkMap[potentialBlockChunk].voxels[(int)potentialBlockCoords.x][(int)potentialBlockCoords.y][(int)potentialBlockCoords.z] > 0))   // if the block is not solid 
    {
        collisionTest.resolveAutoJump(player, potentialBlock); // we use world coordinates for the potential block  
    }   
}

// checks a 4 block radius around player for potential collisions, if a block is found we do an AABB to AABB collision test
// if there is a collision test which passes, we resolve the collision, 
// We also check for potential auto jump possibilites so the player can automatically "teleport" a one block height 
void ChunkManager::checkCollision(Player& player) 
{
    int playerPosX = (int)player.mPosition.x; 
    int playerPosY = (int)player.mPosition.y; 
    int playerPosZ = (int)player.mPosition.z; 
    // glm::vec3 playerChunk = getChunkLocation(glm::vec3(playerPosX, playerPosY, playerPosZ));   

    for (int x = playerPosX - 4; x < playerPosX + 4; x++)  
    {
        for (int y = playerPosY - 4; y < playerPosY + 4; y++)     
        {
            for (int z = playerPosZ - 4; z < playerPosZ + 4; z++)        
            {
                glm::vec3 playerChunk = getChunkLocation(glm::vec3(x, y, z));  

                // converting world coordinates of voxels to chunk coordinates  
                int xCoord = x - (int)chunkMap[playerChunk].position.x * 32; 
                int yCoord = y - (int)chunkMap[playerChunk].position.y * 32; 
                int zCoord = z - (int)chunkMap[playerChunk].position.z * 32; 

                if (chunkMap[playerChunk].voxels[xCoord][yCoord][zCoord] > 0)
                {
                    // we make sure to use world coordinates here for  the AABB min and max calculations    
                    AABB b1 = collisionTest.getVoxelBoundingBox(glm::vec3(x,y,z));  

                    if (collisionTest.AABBtoAABB(b1, player.playerModel->ModelBoundingBox)) 
                    {
                        glm::vec3 collisionNormal = collisionTest.calculateNormal(player, b1); 
                        collisionTest.resolveCollision(b1, player, collisionNormal);   
/*                         if (!((collisionNormal == glm::vec3(0, 1, 0)) || (collisionNormal == glm::vec3(0, -1, 0)))) // if it is not a ground collision 
                        {
                            checkAutoJump(player, collisionNormal);  
                        } */
                    } 
                }
  
            }
        }
    } 

    if (checkGround(player)) 
        player.onGround = true;
    else 
        player.onGround = false; 

/*     AABB b1 = collisionTest.getVoxelBoundingBox(glm::vec3(2,20,2)); 
    collisionTest.sweptResponse(player, b1); */

}

glm::vec3 ChunkManager::getWorldCoordinates(glm::vec3 chunkCoordinates, glm::vec3 voxelPosition)  
{
    float xPos = (chunkCoordinates.x * 32) + voxelPosition.x; 
    float yPos = (chunkCoordinates.y * 32) + voxelPosition.y;  
    float zPos = (chunkCoordinates.z * 32) + voxelPosition.z;  
    return glm::vec3(xPos, yPos, zPos); 
}

glm::vec3 ChunkManager::getChunkCoordinates(glm::vec3 voxelPosition) 
{ // convert world voxel coordinatse to local coordinates 
    glm::vec3 playerChunk = getChunkLocation(voxelPosition);    

    // converting world coordinates of voxels to chunk coordinates  
    int xCoord = voxelPosition.x - (int)chunkMap[playerChunk].position.x * 32; 
    int yCoord = voxelPosition.y - (int)chunkMap[playerChunk].position.y * 32; 
    int zCoord = voxelPosition.z - (int)chunkMap[playerChunk].position.z * 32;  

    return glm::vec3(xCoord, yCoord, zCoord);   

}

bool ChunkManager::isNearPlayer(glm::vec3 cameraPosition, glm::vec3 chunkPosition) 
{ // checks to see if chunk is within renderDistance 

    // converting chunk position to world coordinates 
    glm::vec3 chunkWorldCoordinates = glm::vec3((chunkPosition.x * 32) + 16, chunkPosition.y, (chunkPosition.z*32) + 16);  
    
    glm::vec3 distanceVector = glm::vec3(cameraPosition.x, 0, cameraPosition.z) - glm::vec3(chunkWorldCoordinates.x, 0, chunkWorldCoordinates.z);   

    return glm::length(distanceVector) < renderDistance;   
} 

void ChunkManager::spawnPlayer(glm::vec3 chunkCoord, Player& player)  
{
    glm::vec3 heighestLocation = chunkMap[chunkCoord].heighestLocation; 

    int xCoord = (int)chunkMap[chunkCoord].position.x * 32 + heighestLocation.x; 
    int yCoord = (int)chunkMap[chunkCoord].position.y * 32 + heighestLocation.y; 
    int zCoord = (int)chunkMap[chunkCoord].position.z * 32 + heighestLocation.z; 

    std::cout << "heighest point: " << xCoord << ' ' << yCoord << ' ' << zCoord << std::endl; 
    player.mPosition = glm::vec3(xCoord, yCoord + 10, zCoord);  
    camera.mPosition = glm::vec3(xCoord, yCoord + 10, zCoord);   
    std::cout << "player location: " << player.mPosition.x << ' ' << player.mPosition.y << ' ' << player.mPosition.z << std::endl; 

    player.playerModel->mPosition = player.mPosition;   
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
            if (chunkMap[chunkCoord].voxels[xCoord][yCoord][zCoord] > 0) // if block is not air  
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

void ChunkManager::voxelOutline(Shader& shader, DebugTools& debugTools)  
{ // changes color of voxel to voxel outline color if casted
    glm::vec3 voxelPosition = brensenCast(); 
    if (voxelPosition != glm::vec3(0, 1000, 0) ) 
    {

    
        glm::vec3 chunkCoord = getChunkLocation(voxelPosition);  
        int xCoord = (int)voxelPosition.x - (int)chunkMap[chunkCoord].position.x * 32; 
        int yCoord = (int)voxelPosition.y - (int)chunkMap[chunkCoord].position.y * 32; 
        int zCoord = (int)voxelPosition.z - (int)chunkMap[chunkCoord].position.z * 32; 

        // std::cout << xCoord << ' ' << yCoord << ' ' << zCoord << std::endl; // for debugging purposes  


        if (chunkMap[chunkCoord].voxels[xCoord][yCoord][zCoord] > 0) 
            debugTools.drawVoxelOutline(shader, glm::vec3(voxelPosition.x, voxelPosition.y, voxelPosition.z), glm::vec3(1.001f, 1.001f, 1.001f));            
    }
    
}

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