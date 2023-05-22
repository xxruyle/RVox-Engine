#pragma once 
#include "camera.h" 
#include <iostream>
#include <cmath>
#include <vector> 

#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "fplane.h" 

 
class Frustum
{


public: 
    std::vector<FPlane> pl;         

    Camera& camera; 

    Frustum(Camera& camera) : camera(camera) {}; 



    enum {OUTSIDE, INTERSECT, INSIDE};  


    glm::vec3 ntl,ntr,nbl,nbr,ftl,ftr,fbl,fbr; 
    float nearD, farD, ratio, angle, tang; 
    float nw, nh, fw, fh;  


    void setCamInternals();   
    void setCamDef();
    bool pointInFrustum(glm::vec3 point); 
    bool chunkInFrustum(glm::vec3 chunkPosition); 

private: 
    enum {
        TOP = 0, BOTTOM, LEFT,
        RIGHT, NEARP, FARP 
    }; 


    std::vector<glm::vec3> getChunkCorners(glm::vec3 chunkPosition);   

}; 