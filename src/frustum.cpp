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
    glm::vec3 nc, fc, X, Y, Z;  

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
            // std::cout << "(0,0,0) point outside frustum" << std::endl; 
            return false;
        }
              
    }
    // std::cout << "(0,0,0) point inside frustum" << std::endl; 
    return true;   
}