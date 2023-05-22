#include "camera/frustum.h" 

#define ANG2RAD 3.14159265358979323846/180.0
 

void Frustum::setCamInternals()    
{
    this->ratio = (float)(camera.SCR_WIDTH/camera.SCR_HEIGHT); 
    this->nearD = camera.nearD; 
    this->farD = camera.farD;    

    // compute width and height using near and far plane sections 
    tang = (float)tan(camera.angle * 0.5);          
    nh = nearD * tang; 
    nw = nh * ratio; 
    fh = farD * tang; 
    fw = fh * ratio; 

}



void Frustum::setCamDef() 
{
    glm::vec3  nc, fc, X, Y, Z;  

    Z = -camera.mFront; 

    X = camera.mRight; 

    Y = camera.mUp; 

	nc = camera.mPosition - Z * nearD;   // near plane center 
    fc = camera.mPosition - Z * farD; // far center  


	ntl = nc + Y * nh - X * nw; // near plane top left  
	ntr = nc + Y * nh + X * nw; // near plane top right  
	nbl = nc - Y * nh - X * nw; // near plane bottom left  
	nbr = nc - Y * nh + X * nw; // near plane bottom right  

    // std::cout << ntl.x << ' ' << ntl.y << ' ' << ntl.z << std::endl; 
    // far plane corner points 
    ftl = fc + Y * fh - X * fw; // far top left  
    ftr = fc + Y * fh + X * fw;  // far top right  
	fbl = fc - Y * fh - X * fw;  // far bottom left  
	fbr = fc - Y * fh + X * fw;  //far bottom center  


    pl.clear(); 
    // setting the six plans 
    FPlane top(ntr, ntl, ftl); 
    pl.push_back(top); 

    FPlane bot(nbl, nbr, fbr); 
    pl.push_back(bot); 

    FPlane left(ntl, nbl, fbl); 
    pl.push_back(left); 

    FPlane right(nbr, ntr, fbr); 
    pl.push_back(right); 

    FPlane nearp(ntl, ntr, nbr); 
    pl.push_back(nearp); 

    FPlane farp(ftr, ftl, fbl); 
    pl.push_back(farp);  
}

bool Frustum::pointInFrustum(glm::vec3 point)  
{
    for (int i = 0; i < 6; i++) 
    {
        if (pl[i].distance(point) < 0) // outside frustum
        {
            return false;
        }
              
    }
    return true;   
}

bool Frustum::chunkInFrustum(glm::vec3 chunkPostition) 
{ // returns true if chunk is in the frustum, false otherwise 
    std::vector<glm::vec3> chunkCorners = getChunkCorners(glm::vec3(chunkPostition.x * 32, 0, chunkPostition.z * 32));   

	// for each plane do ...
	for(unsigned int i=0; i < 6; i++) {

		// reset counters for corners in and out
		int out = 0, in = 0;
		// for each corner of the box do ...
		// get out of the cycle as soon as a box as corners
		// both inside and out of the frustum
		for (unsigned int k = 0; k < chunkCorners.size(); k++) {

			// is the corner outside or inside
			if (pl[i].distance(chunkCorners[k]) < 0) 
				out++;
			else
				in++;
		}
		//if all corners are out
		if (!in)
			return false;

	}
    // if there is atleast one corner in 
    return true; 
}

std::vector<glm::vec3> Frustum::getChunkCorners(glm::vec3 chunkPosition)   
{ // gets corners based on chunk position 
    std::vector<glm::vec3> corners; 
    corners.push_back(glm::vec3(chunkPosition.x, 0, chunkPosition.z)); // front bottom right 
    corners.push_back(glm::vec3(chunkPosition.x, 256, chunkPosition.z)); // front top right 

    corners.push_back(glm::vec3(chunkPosition.x + 32, 0, chunkPosition.z)); // front bottom left 
    corners.push_back(glm::vec3(chunkPosition.x + 32, 256, chunkPosition.z)); // front  top left 

    corners.push_back(glm::vec3(chunkPosition.x, 0, chunkPosition.z + 32)); // back bottom right 
    corners.push_back(glm::vec3(chunkPosition.x, 256, chunkPosition.z + 32)); // back top right 

    corners.push_back(glm::vec3(chunkPosition.x + 32, 0, chunkPosition.z + 32)); // back bottom left 
    corners.push_back(glm::vec3(chunkPosition.x + 32, 256, chunkPosition.z + 32)); // back top left 

    return corners;
}