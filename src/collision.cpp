#include "world/collision.h" 


AABB Collision::getVoxelBoundingBox(glm::vec3 voxelPosition) // obtain bounding box in world coordinates from local chunk coordinates  
{
    AABB b1; 
    b1.minX = voxelPosition.x - 0.5f; 
    b1.maxX = voxelPosition.x + 0.5f;  
    b1.minY = voxelPosition.y - 0.5f;  
    b1.maxY = voxelPosition.y + 0.5f; 
    b1.minZ = voxelPosition.z - 0.5f; 
    b1.maxZ = voxelPosition.z + 0.5f; 
    b1.calculateVertices(); 
    return b1; 
}

glm::vec3 Collision::calculateNormal(Player& player, AABB& bbStatic)    
{ // calculate the collision normal iff there was a collision
    AABB* playerAABB = &(player.playerModel->ModelBoundingBox);

    float overlapX = std::min(abs(playerAABB->maxX - bbStatic.minX), abs(bbStatic.maxX - playerAABB->minX)); 
    float overlapY = std::min(abs(playerAABB->maxY - bbStatic.minY), abs(bbStatic.maxY - playerAABB->minY));  
    float overlapZ = std::min(abs(playerAABB->maxZ - bbStatic.minZ), abs(bbStatic.maxZ - playerAABB->minZ));  

    glm::vec3 collisionNormal; 
    if (overlapX <= overlapY && overlapX <= overlapZ) 
    {
        if ((playerAABB->maxX - bbStatic.minX) < (bbStatic.maxX - playerAABB->minX))
            collisionNormal = glm::vec3(-1.0f, 0.0f, 0.0f); 
        else
            collisionNormal = glm::vec3(1.0f, 0.0f, 0.0f); 
    }

    else if (overlapZ <= overlapX && overlapZ <= overlapY) 
    {
        if ((playerAABB->maxZ - bbStatic.minZ) < (bbStatic.maxZ - playerAABB->minZ))
            collisionNormal = glm::vec3(0.0f, 0.0f, -1.0f); 
        else
            collisionNormal = glm::vec3(0.0f, 0.0f, 1.0f); 
    }

    else if (overlapY <= overlapX && overlapY <= overlapZ)  
    {
        if ((playerAABB->maxY - bbStatic.minY) < (bbStatic.maxY - playerAABB->minY))
            collisionNormal = glm::vec3(0.0f, -1.0f, 0.0f); 
        else
            collisionNormal = glm::vec3(0.0f, 1.0f, 0.0f); 
    }


    return collisionNormal; 
}


bool Collision::AABBtoAABB(AABB& bbox1, AABB& bbox2) 
{
    return( 
        bbox1.maxX  >= bbox2.minX  &&
        bbox1.minX  <= bbox2.maxX  && 

        bbox1.maxY >= bbox2.minY  &&
        bbox1.minY  <= bbox2.maxY  &&

        bbox1.maxZ >= bbox2.minZ && 
        bbox1.minZ <= bbox2.maxZ   
    );
}

void Collision::resolveCollision(AABB& b1, Player& player, glm::vec3 collisionNormal)   
{
        // x normals  
        if (collisionNormal == glm::vec3(-1.0, 0.0, 0.0))  
        {
            player.velocityX = 0.0f; 
            player.mPosition = glm::vec3(b1.minX - player.playerModel->ModelBoundingBox.getXExtent() - 0.2f, player.mPosition.y, player.mPosition.z); 
        }
        else if (collisionNormal == glm::vec3(1.0f, 0.0f, 0.0f))  
        {
            player.velocityX = 0.0f;
            player.mPosition = glm::vec3(b1.maxX + player.playerModel->ModelBoundingBox.getXExtent() + 0.23f, player.mPosition.y, player. mPosition.z); 
        }
        // y normals
        if (collisionNormal == glm::vec3(0.0f, -1.0f, 0.0f)) 
        {
            player.velocityY = 0.0f;  
            player.mPosition = glm::vec3(player.mPosition.x, b1.minY - player.playerModel->ModelBoundingBox.getYExtent() - 0.00001f, player.mPosition.z); 
        }
        else if (collisionNormal == glm::vec3(0.0f, 1.0f, 0.0f)) 
        {
            player.velocityY = 0.0f; 
            player.onGround = true; 
            player.mPosition = glm::vec3(player.mPosition.x, b1.maxY + 0.00001f, player.mPosition.z);   
            
        }
        // z normals 
        if (collisionNormal == glm::vec3(0.0f, 0.0f, -1.0f)) 
        {
            player.velocityZ = 0.0f; 
            player.mPosition = glm::vec3(player.mPosition.x, player.mPosition.y, b1.minZ - player.playerModel->ModelBoundingBox.getZExtent() - 0.00001f);  
        }
        else if (collisionNormal == glm::vec3(0.0f, 0.0f, 1.0f)) 
        {
            player.velocityZ = 0.0f; 
            player.mPosition = glm::vec3(player.mPosition.x, player.mPosition.y, b1.maxZ + player.playerModel->ModelBoundingBox.getZExtent() + 0.23f);                           
        } 
}



void Collision::resolveAutoJump(Player& player, glm::vec3 potentialBlockCoords) 
{
    AABB b1 = getVoxelBoundingBox(potentialBlockCoords); 
    player.mPosition = glm::vec3(player.mPosition.x, b1.minY + 0.0001f, player.mPosition.z);  
}


float Collision::sweptAABB(Player& player, AABB& b2Static)  
{
    AABB* playerAABB = &(player.playerModel->ModelBoundingBox); // the player's AABB
    float xInvEntry, zInvEntry, yInvEntry; 
    float xInvExit, zInvExit, yInvExit;  

    // find the distance between the objects on the near and far sides for both x, y, and z 
    if (player.sweptVelocity.x > 0.0f)  
    { 
    xInvEntry = b2Static.minX - playerAABB->maxX;       
    xInvExit = b2Static.maxX - playerAABB->minX;     
    }
    else 
    { 
    xInvEntry = b2Static.maxX - playerAABB->minX;      
    xInvExit = b2Static.minX - playerAABB->maxX;    
    } 


    if (player.sweptVelocity.y > 0.0f)  
    { 
    yInvEntry = b2Static.minY - playerAABB->maxY;   
    yInvExit = (b2Static.maxY) - playerAABB->minY;    
    }
    else 
    { 
    yInvEntry = (b2Static.maxY) - playerAABB->minY;     
    yInvExit = b2Static.minY - playerAABB->maxY;    
    }


    if (player.sweptVelocity.z > 0.0f)   
    { 
    zInvEntry = b2Static.minZ - playerAABB->maxZ;   
    zInvExit = b2Static.maxZ - playerAABB->minZ;    
    }
    else 
    { 
    zInvEntry = b2Static.maxZ - playerAABB->minZ;     
    zInvExit = b2Static.minZ - playerAABB->maxZ;    
    }


    // find time of collision and time of leaving or each axis (conditionals to prevent division by zero error) 
    float xEntry, zEntry, yEntry;   
    float xExit, zExit, yExit; 
    if (player.sweptVelocity.x == 0.0f) 
    {
        xEntry = -std::numeric_limits<float>::infinity(); 
        xExit = std::numeric_limits<float>::infinity();   
    }
    else 
    {
        xEntry = xInvEntry / player.sweptVelocity.x; 
        xExit = xInvExit / player.sweptVelocity.x; 
    }


    if (player.sweptVelocity.y == 0.0f) 
    {
        yEntry = -std::numeric_limits<float>::infinity(); 
        yExit = std::numeric_limits<float>::infinity();   
    }
    else 
    {
        yEntry = yInvEntry / player.sweptVelocity.y; 
        yExit  = yInvExit / player.sweptVelocity.y;   
    }

    if (player.sweptVelocity.z == 0.0f) 
    {
        zEntry = -std::numeric_limits<float>::infinity(); 
        zExit = std::numeric_limits<float>::infinity();      
    }
    else 
    {
        zEntry = zInvEntry / player.sweptVelocity.z;  
        zExit  = zInvExit / player.sweptVelocity.z;    
    }

    float entryTime, exitTime; 
    entryTime = std::max(std::max(xEntry, yEntry), zEntry);
    exitTime = std::min(std::min(xExit, yExit), zExit);

    // if there was no collision
    if (entryTime > exitTime || (xEntry < 0.0f && yEntry < 0.0f && zEntry < 0.0f) || xEntry > 1.0f || yEntry > 1.0f || zEntry > 1.0f)   
    {
        player.collisionNormal = glm::vec3(0, 0, 0); 
        return 1.0f; 
    } 
    else // if there was a  collision  
    {  
        // calculate the normals of the collided surface 
        if (xEntry > yEntry && xEntry > zEntry)  
        {
            if (xInvEntry < 0.0f) 
            {
                player.collisionNormal = glm::vec3(1.0f, 0.0f, 0.0f); 
            }
            else
            {
                player.collisionNormal = glm::vec3(-1.0f, 0.0f, 0.0f);  
            }
        } 
        else if (yEntry > xEntry && yEntry > zEntry) 
        {
            if (yInvEntry < 0.0f) 
            {
                player.collisionNormal = glm::vec3(0.0f, 1.0f, 0.0f);  
            } 
            else 
            {
                player.collisionNormal = glm::vec3(0.0f, -1.0f, 0.0f); 
            }
        }
        else if (zEntry > xEntry && zEntry > yEntry) 
        {
            if (zInvEntry < 0.0f) 
            {
                player.collisionNormal = glm::vec3(0.0f, 0.0f, 1.0f);  
            }
            else
            {
                player.collisionNormal = glm::vec3(0.0f, 0.0f, -1.0f); 
            }
        }

        return entryTime; 
    }
}

void Collision::sweptResponse(Player& player, AABB& b2Static) 
{
    float collisionTime = sweptAABB(player, b2Static);  
    
    if (collisionTime < 1.0f) 
    {
        std::cout << collisionTime << std::endl;      

        player.mPosition.x += player.sweptVelocity.x * collisionTime; 
        player.mPosition.y += player.sweptVelocity.y * collisionTime; 
        player.mPosition.z += player.sweptVelocity.z * collisionTime;

        // Calculate the remaining time after collision

    }
         
}



