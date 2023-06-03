#pragma once 
#include <limits>
#include <cmath> 
#include <initializer_list> // for max and min 
#include "player/player.h" 
#include "debug/debug_tools.h"

class Collision 
{
public: 

    AABB getVoxelBoundingBox(glm::vec3 voxelPosition); // obtain bounding box in world coordinates from local chunk coordinates  

    glm::vec3 calculateNormal(Player& player, AABB& bbox2);  // bbox1 is the object being collided  

    bool AABBtoAABB(AABB& bbox1, AABB& bbox2);

    float sweptAABB(Player& player, AABB& b2Static);   

    void sweptResponse(Player& player, AABB& b2Static); 

    void resolveCollision(AABB& b1, Player& player, glm::vec3 collisionNormal);

    void resolveAutoJump(Player& player, glm::vec3 potentialBlockCoords);  



}; 