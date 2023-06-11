#include "world/chunk_manager.h"  


void ChunkManager::generateChunk(glm::vec3 chunkCoord) 
{
    // the chunk that we are intending to generate 
    Chunk& cMiddle = chunkMap[glm::vec3(chunkCoord.x, 0, chunkCoord.z)];   
    chunkSet.insert(glm::vec3(chunkCoord.x, 0, chunkCoord.z)); 
    cMiddle.position = glm::vec3(glm::vec3(chunkCoord.x, 0, chunkCoord.z));   
    cMiddle.generateSolidChunk(currentRandomSeed, cMiddle.position.x * 32, cMiddle.position.z * 32);       

/*     using namespace std::chrono;
    auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);   
    std::cout << duration.count() << std::endl; */
     if (!chunkExists(glm::vec3(chunkCoord.x + 1, 0, chunkCoord.z))) // if the chunk doesn't already exist generate it 
    {
        Chunk& cLeft = chunkMap[glm::vec3(chunkCoord.x + 1, 0, chunkCoord.z)];  
        chunkSet.insert(glm::vec3(chunkCoord.x + 1, 0, chunkCoord.z)); 
        cLeft.position = glm::vec3(glm::vec3(chunkCoord.x + 1, 0, chunkCoord.z));  
        cLeft.generateSolidChunk(currentRandomSeed, cLeft.position.x * 32, cLeft.position.z * 32);  

        cMiddle.mLeft = &cLeft;    
    } else {
        cMiddle.mLeft = &chunkMap[glm::vec3(chunkCoord.x + 1, 0, chunkCoord.z)]; 
    }


    if (!chunkExists(glm::vec3(chunkCoord.x - 1, 0, chunkCoord.z)))
    {
        Chunk& cRight = chunkMap[glm::vec3(chunkCoord.x - 1, 0, chunkCoord.z)]; 
        chunkSet.insert(glm::vec3(chunkCoord.x - 1, 0, chunkCoord.z));  
        cRight.position = glm::vec3(glm::vec3(chunkCoord.x - 1, 0, chunkCoord.z));  
        cRight.generateSolidChunk(currentRandomSeed, cRight.position.x * 32, cRight.position.z * 32);   

        cMiddle.mRight = &cRight;  
    } else {
        cMiddle.mRight = &chunkMap[glm::vec3(chunkCoord.x - 1, 0, chunkCoord.z)];
    }


    if (!chunkExists(glm::vec3(chunkCoord.x, 0, chunkCoord.z+1)))
    {
        Chunk& cUp = chunkMap[glm::vec3(chunkCoord.x, 0, chunkCoord.z+1)];    
        chunkSet.insert(glm::vec3(chunkCoord.x, 0, chunkCoord.z+1)); 
        cUp.position = glm::vec3(glm::vec3(chunkCoord.x, 0, chunkCoord.z+1));    
        cUp.generateSolidChunk(currentRandomSeed, cUp.position.x * 32, cUp.position.z * 32);    

        cMiddle.mUp = &cUp;  
    } else {
        cMiddle.mUp = &chunkMap[glm::vec3(chunkCoord.x, 0, chunkCoord.z+1)]; 
    }

    if (!chunkExists(glm::vec3(chunkCoord.x, 0, chunkCoord.z-1))) 
    {
        Chunk& cBottom = chunkMap[glm::vec3(chunkCoord.x, 0, chunkCoord.z-1)]; 
        chunkSet.insert(glm::vec3(chunkCoord.x, 0, chunkCoord.z-1)); 
        cBottom.position = glm::vec3(glm::vec3(chunkCoord.x, 0, chunkCoord.z-1));    
        cBottom.generateSolidChunk(currentRandomSeed, cBottom.position.x * 32, cBottom.position.z * 32);      

        cMiddle.mBot = &cBottom; 
    } else {
        cMiddle.mBot = &chunkMap[glm::vec3(chunkCoord.x, 0, chunkCoord.z-1)]; 
    }

    cMiddle.generateAndMesh(); // multithreading  
/*     cMiddle.mesh(); 
    cMiddle.finishMeshing();  */
}

bool ChunkManager::chunkIsRenderable(glm::vec3 chunkPos) 
{
    return (
        chunkMap[chunkPos].mLeft != nullptr &&
        chunkMap[chunkPos].mRight != nullptr &&
        chunkMap[chunkPos].mUp != nullptr &&
        chunkMap[chunkPos].mBot != nullptr 
    ); 
}

bool ChunkManager::chunkExists(glm::vec3 chunkPos)  
{
    return (chunkSet.count(chunkPos));    
}

void ChunkManager::createChunks(int randSeed) 
{ // creates chunks and makes them generate a chunk 
    chunkMap.clear(); 
    currentRandomSeed = randSeed; 
}



void ChunkManager::renderChunks(Shader& shader)  
{ // renders the existing chunks 

    // looping over chunks that are within camera area  
    chunkBuffer.clear();  
    getNearChunks(); 

    for (unsigned int i = 0; i < chunkBuffer.size(); i++) 
    {
        glm::vec3 chunkPos = chunkBuffer[i]; 
        if (chunkExists(chunkPos) && chunkMap[chunkPos].renderable) // if the coordinates exist in the world           
        {
            if (isNearPlayer(camera.mPosition, chunkPos))  
            {     
                chunkMap[chunkPos].draw(shader, frustum);  
            }
                
        } else if (chunkMap[chunkPos].startedThread) { 
            if (chunkMap[chunkPos].meshFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready)   
            {
                chunkMap[chunkPos].finishMeshing(); 
            }
        }  else if (isNearPlayer(camera.mPosition, chunkPos)){ // if coordinate's do not already exist in the world, keep generating. (Allows for "infinite" terrain generation)    
            generateChunk(chunkPos);  
        }
    } 
}


void ChunkManager::meshAfterDeletion(Chunk& chunk, glm::vec3 deletedVoxelCoord)  
{
    if (deletedVoxelCoord.x == 0) // right 
    {
        chunk.mRight->mesh(); 
        chunk.mRight->finishMeshing(); 
    }

    if (deletedVoxelCoord.x == 31) // left  
    {
        chunk.mLeft->mesh(); 
        chunk.mLeft->finishMeshing(); 
    }

    if (deletedVoxelCoord.z == 0) // bottom 
    {
        chunk.mBot->mesh(); 
        chunk.mBot->finishMeshing(); 
    }

    if (deletedVoxelCoord.z == 31) // top 
    {
        chunk.mUp->mesh(); 
        chunk.mUp->finishMeshing(); 
    }

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
        sum += sizeof(chunk.second.voxelArray) / sizeof(char);   
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
            int voxelIndex = chunkMap[playerChunk].getVoxelIndex(glm::vec3(underVoxel.x, underVoxel.y, underVoxel.z));   
            if ((chunkMap[playerChunk].voxelArray[voxelIndex] > 0)) // if the block is solid  
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
    int voxelIndex = chunkMap[potentialBlockChunk].getVoxelIndex(glm::vec3(potentialBlockCoords.x, potentialBlockCoords.y, potentialBlockCoords.z)); 
    if (!(chunkMap[potentialBlockChunk].voxelArray[voxelIndex] > 0))   // if the block is not solid  
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


                int voxelIndex = chunkMap[playerChunk].getVoxelIndex(glm::vec3(xCoord, yCoord, zCoord)); 
                if (chunkMap[playerChunk].voxelArray[voxelIndex] > 0) 
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

        int voxelIndex = chunkMap[chunkCoord].getVoxelIndex(glm::vec3(xCoord, yCoord, zCoord)); 

        if (xCoord >= 0 && yCoord >= 0 && zCoord >= 0) 
        {
            if (chunkMap[chunkCoord].voxelArray[voxelIndex] > 0) // if block is not air   
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
    if (voxelPosition != glm::vec3(0, 1000, 0))  
    {

    
        glm::vec3 chunkCoord = getChunkLocation(voxelPosition);  
        int xCoord = (int)voxelPosition.x - (int)chunkMap[chunkCoord].position.x * 32; 
        int yCoord = (int)voxelPosition.y - (int)chunkMap[chunkCoord].position.y * 32; 
        int zCoord = (int)voxelPosition.z - (int)chunkMap[chunkCoord].position.z * 32; 


        int voxelIndex = chunkMap[chunkCoord].getVoxelIndex(glm::vec3(xCoord, yCoord, zCoord)); 
        if (chunkMap[chunkCoord].voxelArray[voxelIndex] > 0)  
            debugTools.drawVoxelOutline(shader, glm::vec3(voxelPosition.x, voxelPosition.y, voxelPosition.z), glm::vec3(1.001f, 1.001f, 1.001f));            
    }
    
}

void ChunkManager::deleteVoxel()
{
    glm::vec3 voxelPosition = brensenCast(); 

    glm::vec3 chunkCoord = getChunkLocation(voxelPosition);   
    std::cout << chunkCoord.x << ' ' << chunkCoord.z << std::endl; // for debugging purposes

    // getting chunk local space coordinates 
    int xCoord = (int)voxelPosition.x - (int)chunkMap[chunkCoord].position.x * 32; 
    int yCoord = (int)voxelPosition.y - (int)chunkMap[chunkCoord].position.y * 32; 
    int zCoord = (int)voxelPosition.z - (int)chunkMap[chunkCoord].position.z * 32;  

    std::cout << xCoord << ' ' << yCoord << ' ' << zCoord << std::endl; // for debugging purposes  
    int voxelIndex = chunkMap[chunkCoord].getVoxelIndex(glm::vec3(xCoord, yCoord, zCoord)); 

    chunkMap[chunkCoord].voxelArray[voxelIndex] = 0;     

    chunkMap[chunkCoord].mesh(); 
    chunkMap[chunkCoord].finishMeshing(); 
    meshAfterDeletion(chunkMap[chunkCoord], glm::vec3(xCoord, yCoord, zCoord)); // mesh neighbors if deleted voxel was on the chunk border
}