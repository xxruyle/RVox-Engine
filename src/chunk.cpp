#include "world/chunk.h" 


void Chunk::generate(int randSeed)   
{
    startedGeneration = true; 
    // meshFuture = std::async(std::launch::async, &Chunk::generateAndMesh, this, randSeed, startX, startZ);
    generateFuture = std::async(std::launch::async, &Chunk::generateDebugChunk, this, randSeed, position.x * 32, position.z  * 32);              
} 


void Chunk::threadedMesh() 
{
    if (!startedMesh)  
    {
        startedMesh = true; 
        meshFuture = std::async(std::launch::async, &Chunk::mesh, this); 
    }
}


int Chunk::getVoxelIndex(glm::vec3 coordinate)  
{ // allows access of the flat array using 3D coordinates  
    return coordinate.x + (coordinate.y * 32) + (coordinate.z * 32 * 256);   
}

void Chunk::generateSolidChunk(int randSeed, int startX, int startZ) 
{
    memset(voxelArray, 0, sizeof(voxelArray)); 
    const int xs = 32;
    const int zs = 32;
    float frequency = 0.005f; 

    FastNoiseLite mountain; 
    // mountain.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    // mountain.SetFractalType(FastNoiseLite::FractalType_FBm); 
    mountain.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    mountain.SetFractalType(FastNoiseLite::FractalType_Ridged);  
    mountain.SetFrequency(frequency); 
    mountain.SetSeed(12);   
    mountain.SetFractalOctaves(4); 

        // Gather noise data
    int noiseData[xs][zs];
    int stoneLimit = 3;  


    for (int x = 0; x < xs; x++) 
    {
        for (int z = 0; z < zs; z++)  
        {
            int height = static_cast<int>((pow(mountain.GetNoise((float)(startX + x), (float)(startZ + z)) + 1.0f, 6.7f)));      
            noiseData[x][z] = height; // -40      
        }
    }




    // pushing the positions of each block and making the height map  
    for (int x = 0; x < xs; x++) 
    {
        for (int y = 0; y < 256; y++)  
        {
            for (int z = 0; z <  zs; z++)     
            {
                if (y < stoneLimit + noiseData[x][z])   
                {
                    int surface = stoneLimit + noiseData[x][z] - 5; 

                    if (y < surface) { // stone 
                        voxelArray[getVoxelIndex(glm::vec3(x, y, z))] = 2; 
                        // voxels[i][k][j] = 2;
                        
                    } else if (y < 80) { // grass 
                        voxelArray[getVoxelIndex(glm::vec3(x, y, z))] = 1; 
                        // voxels[i][k][j] = 1;
                    } else { // snow mountains 
                        voxelArray[getVoxelIndex(glm::vec3(x, y, z))] = 3;    
                        // voxels[i][k][j] = 3;
                    }
                }
            } 
        }
    }  
}

void Chunk::generateDebugChunk(int randSeed, int startX, int startZ) 
{ // 3D noise chunk 




    const int xs = 32;
    const int zs = 32;
    float frequency = 0.005f; 



    FastNoiseLite noise; 
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);  
    noise.SetFractalType(FastNoiseLite::FractalType_Ridged);  
    noise.SetFractalOctaves(rand() % 3 + 1);     
    noise.SetFrequency(.01);  
    noise.SetRotationType3D(FastNoiseLite::RotationType3D_ImproveXZPlanes);   
    noise.SetSeed(randSeed);  

/*     FastNoiseLite cliffs; 
    cliffs.SetNoiseType(FastNoiseLite::NoiseType_Cellular); 
    cliffs.SetFractalType(FastNoiseLite::FractalType_FBm);   
    cliffs.SetFrequency(.005);  
    cliffs.SetSeed(randSeed);   
    cliffs.SetFractalOctaves(3);   */

    FastNoiseLite mountain; 
    mountain.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    mountain.SetFractalType(FastNoiseLite::FractalType_Ridged);  
    mountain.SetFrequency(frequency); 
    mountain.SetSeed(randSeed);   
    mountain.SetFractalOctaves(4); 

        // Gather noise data
    int noiseData[xs][zs];
    int stoneLimit = 20;    
    float randomThreshold; 

    int randVal = rand() % 3 + 1; 

    if (randVal == 3)  
        randomThreshold = 0.8f; 
    else if (randVal == 2) 
        randomThreshold = 0.75f; 
    else if (randVal == 1) 
        randomThreshold = 0.7f;   
    else 
        randomThreshold = 0.95f;  






    for (int x = 0; x < xs; x++) 
    {
        for (int z = 0; z < zs; z++)  
        {
            int height = static_cast<int>((pow(mountain.GetNoise((float)(startX + x), (float)(startZ + z)) + 1.0f, 6.5f)));   
            noiseData[x][z] = height; // -40      
        }
    }


    for (int x = 0; x < xs; x++) 
    {
        for (int y = 0; y < 256; y++) 
        {
            for (int z = 0; z < zs; z++) 
            {
                if (y < stoneLimit + noiseData[x][z])   
                {
                    int surface = stoneLimit + noiseData[x][z] - 4;     







                    if (y >= surface) {  
                        if (y - 1 >= 0) 
                        {
                            if (voxelArray[getVoxelIndex(glm::vec3(x,y-1,z))] == 0) { 
                                voxelArray[getVoxelIndex(glm::vec3(x,y,z))] = 0; 
                            } else if (y <= surface + 2) {     
                                voxelArray[getVoxelIndex(glm::vec3(x,y,z))] = 4;   
                            } else {
                                voxelArray[getVoxelIndex(glm::vec3(x,y,z))] = 1;    
                            }
                        }
                    } else if (y < surface) { // stone    
                        noise.SetFrequency(0.01);  

                        char stoneType;  
                        int randomStone = rand() % 500;      
                        (randomStone > 490) ? stoneType = 5 : stoneType = 2;     
                        float MultiNoise = noise.GetNoise((float)(startX + x), (float)(y), (float)(startZ + z));       
                        MultiNoise < randomThreshold ? voxelArray[getVoxelIndex(glm::vec3(x,y,z))] = stoneType : voxelArray[getVoxelIndex(glm::vec3(x,y,z))] =  0;      
                        
                    } else if (y < 80 && y > 1) { // grass 
                        voxelArray[getVoxelIndex(glm::vec3(x, y, z))] = 1;  
                    } else { // snow mountains 
                        voxelArray[getVoxelIndex(glm::vec3(x, y, z))] = 3;    
                    }

                    if (y == 0) { // bedrock 
                        voxelArray[getVoxelIndex(glm::vec3(x,y,z))] = 3;  
                    }

                }
            }
        }
    }

}


void Chunk::mesh()      
{ // checks interior voxels
    indices.clear(); // clear the indices before each mesh! Very important to fix bug that causes overlapping triangles 
    Voxel voxel; // allows for voxel color info  
    for ( int x = 0; x < 32; x++)  
    {
        for (int y = 0; y < 256; y++) 
        {
            for (int z = 0; z < 32; z++)    
            {   
                if (isSolid(voxelArray[getVoxelIndex(glm::vec3(x, y, z))]))     
                {
                    if (isAir(getVoxel(glm::vec3(x, y, z+1)))) // back
                    {
                        for (unsigned int i = 0; i < 6; i++) 
                            indices.push_back(vertices.size() + backIndices[i]);   
                        for (unsigned int i = 0; i < backFace.size(); i++) 
                        {
                            voxelVertex vertex; 
                            vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + backFace[i];  
                            vertex.Normal = glm::vec3(0.0f,  0.0f, -1.0f);    

                            glm::vec3 worldCoord = calculateWorldCoordinates(x, y, z); 
                            vertex.Color = voxel.calculateColor(voxelArray[getVoxelIndex(glm::vec3(x, y, z))], worldCoord.x, worldCoord.z); 

                            vertex.aoValue = backAO(glm::vec3(x,y,z), i); 
                            vertices.push_back(vertex);      
                        }
                    }   
                
                    if (isAir(getVoxel(glm::vec3(x, y, z-1)))) // front 
                    {
                        for (unsigned int i = 0; i < 6; i++) 
                            indices.push_back(vertices.size() + frontIndices[i]); 
                        for (unsigned int i = 0; i < frontFace.size(); i++) 
                        {
                            voxelVertex vertex; 
                            vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + frontFace[i];    
                            vertex.Normal = glm::vec3(0.0f,  0.0f, 1.0f);  


                            glm::vec3 worldCoord = calculateWorldCoordinates(x, y, z); 
                            vertex.Color = voxel.calculateColor(voxelArray[getVoxelIndex(glm::vec3(x, y, z))], worldCoord.x, worldCoord.z); 

                            vertex.aoValue = frontAO(glm::vec3(x,y,z), i);  
                            vertices.push_back(vertex);     
                        }
                    }   
                
                    if (isAir(getVoxel(glm::vec3(x+1, y, z)))) // left 
                    {
                        for (unsigned int i = 0; i < 6; i++) 
                            indices.push_back(vertices.size() + leftIndices[i]); 
                        for (unsigned int i = 0; i < leftFace.size(); i++) 
                        {
                            voxelVertex vertex; 
                            vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + leftFace[i];    
                            vertex.Normal = glm::vec3(-1.0f,  0.0f,  0.0f); 

                            glm::vec3 worldCoord = calculateWorldCoordinates(x, y, z); 
                            vertex.Color = voxel.calculateColor(voxelArray[getVoxelIndex(glm::vec3(x, y, z))], worldCoord.x, worldCoord.z); 

                            vertex.aoValue = leftAO(glm::vec3(x,y,z), i); 
                            vertices.push_back(vertex);     
                        }
                    }   
                
                    if (isAir(getVoxel(glm::vec3(x-1, y, z)))) // right
                    {
                        for (unsigned int i = 0; i < 6; i++) 
                            indices.push_back(vertices.size() + rightIndices[i]); 
                        for (unsigned int i = 0; i < rightFace.size(); i++)  
                        {
                            voxelVertex vertex; 
                            vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + rightFace[i];    
                            vertex.Normal = glm::vec3(1.0f,  0.0f,  0.0f); 

                            glm::vec3 worldCoord = calculateWorldCoordinates(x, y, z); 
                            vertex.Color = voxel.calculateColor(voxelArray[getVoxelIndex(glm::vec3(x, y, z))], worldCoord.x, worldCoord.z); 

                            vertex.aoValue = rightAO(glm::vec3(x,y,z), i); 
                            vertices.push_back(vertex);     
                        }
                    }   
                    if (isAir(getVoxel(glm::vec3(x, y-1, z))))  // bottom 
                    {
                        for (unsigned int i = 0; i < 6; i++) 
                            indices.push_back(vertices.size() + bottomIndices[i]); 
                        for (unsigned int i = 0; i < bottomFace.size(); i++)   
                        {
                            voxelVertex vertex; 
                            vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + bottomFace[i];    
                            vertex.Normal = glm::vec3(0.0f, -1.0f,  0.0f);  

                            glm::vec3 worldCoord = calculateWorldCoordinates(x, y, z); 
                            vertex.Color = voxel.calculateColor(voxelArray[getVoxelIndex(glm::vec3(x, y, z))], worldCoord.x, worldCoord.z); 

                            vertex.aoValue = bottomAO(glm::vec3(x, y, z), i); 
                            vertices.push_back(vertex);     
                        }
                    }   
                    if (isAir(getVoxel(glm::vec3(x, y+1, z)))) // top 
                    {
                        for (unsigned int i = 0; i < 6; i++) 
                            indices.push_back(vertices.size() + topIndices[i]); 
                        for (unsigned int i = 0; i < topFace.size(); i++)   
                        {
                            voxelVertex vertex; 
                            vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + topFace[i];    
                            vertex.Normal = glm::vec3(0.0f,  1.0f,  0.0f);    

                            glm::vec3 worldCoord = calculateWorldCoordinates(x, y, z); 
                            vertex.Color = voxel.calculateColor(voxelArray[getVoxelIndex(glm::vec3(x, y, z))], worldCoord.x, worldCoord.z);  
                            
                            vertex.aoValue = topAO(glm::vec3(x,y,z), i); 
                            vertices.push_back(vertex);     
                        }
                    }   
                }   
            }
        }
    }    
}

void Chunk::finishMeshing() 
{
    setupMesh(); 
    vertices.clear(); 
    renderable = true; 
}

void Chunk::setupMesh() 
{
    std::lock_guard<std::mutex> lock(vLock); 


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO); 
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(voxelVertex), &vertices[0], GL_STATIC_DRAW);             


    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(voxelVertex), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW); 

    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(voxelVertex), (void*)offsetof(voxelVertex, Normal));      


    // vertex colors
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(voxelVertex), (void*)offsetof(voxelVertex, Color)); 

    //ambient occlusion value 
    glEnableVertexAttribArray(3);	
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(voxelVertex), (void*)offsetof(voxelVertex, aoValue));  
 
    glBindVertexArray(0);   
}



// gets the AO value given a vertex index
glm::vec3 Chunk::topAO(glm::vec3 vPos, unsigned int vertexIndex)  
{  // go counter clockwise around face indices
    int sideCount;  
    switch (vertexIndex) 
    {
        case 0: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x-1, vPos.y+1, vPos.z)), 
                getVoxel(glm::vec3(vPos.x, vPos.y+1, vPos.z+1)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y+1, vPos.z+1))
                ); 
            break;
        case 1: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x, vPos.y+1, vPos.z+1)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y+1, vPos.z)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y+1, vPos.z+1))
                );             
            break;
        case 2: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x+1, vPos.y+1, vPos.z)), 
                getVoxel(glm::vec3(vPos.x, vPos.y+1, vPos.z-1)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y+1, vPos.z-1))
                );      
            break;
        default:  
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x, vPos.y+1, vPos.z-1)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y+1, vPos.z)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y+1, vPos.z-1))
                );               
            break; 
    }


    return getAOValue(sideCount);  
}

// gets the AO value given a vertex index
glm::vec3 Chunk::frontAO(glm::vec3 vPos, unsigned int vertexIndex)   
{  // go counter clockwise around face indices
    int sideCount;  
    switch (vertexIndex) 
    {
        case 0: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x-1, vPos.y, vPos.z-1)), 
                getVoxel(glm::vec3(vPos.x, vPos.y+1, vPos.z-1)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y+1, vPos.z-1))
                );             
            break;
        case 1: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x, vPos.y+1, vPos.z-1)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y, vPos.z-1)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y+1, vPos.z-1))
                );    
            break;
        case 2: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x+1, vPos.y, vPos.z-1)), 
                getVoxel(glm::vec3(vPos.x, vPos.y-1, vPos.z-1)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y-1, vPos.z-1))
                );
            break;
        default:  
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x, vPos.y-1, vPos.z-1)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y, vPos.z-1)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y-1, vPos.z-1))
                );
            break; 
    }

    return getAOValue(sideCount);  
}

glm::vec3 Chunk::rightAO(glm::vec3 vPos, unsigned int vertexIndex)    
{  // go counter clockwise around face indices
    int sideCount;  
    switch (vertexIndex) 
    {
        case 0: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x-1, vPos.y, vPos.z+1)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y+1, vPos.z)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y+1, vPos.z+1))
                );
            break;
        case 1: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x-1, vPos.y+1, vPos.z)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y, vPos.z-1)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y+1, vPos.z-1))
                );
            break;
        case 2: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x-1, vPos.y, vPos.z-1)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y-1, vPos.z)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y-1, vPos.z-1))
                );
            break;
        default:  
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x-1, vPos.y-1, vPos.z)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y, vPos.z+1)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y-1, vPos.z+1))
                );            
            break; 
    }

    return getAOValue(sideCount);  
}

glm::vec3 Chunk::backAO(glm::vec3 vPos, unsigned int vertexIndex)    
{  // go counter clockwise around face indices
    int sideCount;  
    switch (vertexIndex) 
    {
        case 0: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x+1, vPos.y, vPos.z+1)), 
                getVoxel(glm::vec3(vPos.x, vPos.y+1, vPos.z+1)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y+1, vPos.z+1))
                );               
            break;
        case 1: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x, vPos.y+1, vPos.z+1)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y, vPos.z+1)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y+1, vPos.z+1))
                ); 
            break;
        case 2: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x-1, vPos.y, vPos.z+1)), 
                getVoxel(glm::vec3(vPos.x, vPos.y-1, vPos.z+1)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y-1, vPos.z+1))
                ); 
            break;
        default:  
              sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x, vPos.y-1, vPos.z+1)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y, vPos.z+1)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y-1, vPos.z+1))
                ); 
            break; 
    }
    return getAOValue(sideCount);  
}


glm::vec3 Chunk::leftAO(glm::vec3 vPos, unsigned int vertexIndex)     
{  // go counter clockwise around face indices
    int sideCount;  
    switch (vertexIndex) 
    {
        case 0: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x+1, vPos.y, vPos.z-1)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y+1, vPos.z)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y+1, vPos.z-1))
                ); 
            break;
        case 1: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x+1, vPos.y+1, vPos.z)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y, vPos.z+1)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y+1, vPos.z+1))
                ); 
            break;
        case 2: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x+1, vPos.y, vPos.z+1)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y-1, vPos.z)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y-1, vPos.z+1))
                ); 
            break;
        default:  
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x+1, vPos.y-1, vPos.z)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y, vPos.z-1)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y-1, vPos.z-1))
                ); 
            break; 
    }

    return getAOValue(sideCount);  
}

glm::vec3 Chunk::bottomAO(glm::vec3 vPos, unsigned int vertexIndex)      
{  // go counter clockwise around face indices
    int sideCount;  
    switch (vertexIndex) 
    {
        case 0: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x-1, vPos.y-1, vPos.z)), 
                getVoxel(glm::vec3(vPos.x, vPos.y-1, vPos.z-1)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y-1, vPos.z-1))
                ); 
            break;
        case 1: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x, vPos.y-1, vPos.z-1)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y-1, vPos.z)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y-1, vPos.z-1))
                ); 
            break;
        case 2: 
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x+1, vPos.y-1, vPos.z)), 
                getVoxel(glm::vec3(vPos.x, vPos.y-1, vPos.z+1)), 
                getVoxel(glm::vec3(vPos.x+1, vPos.y-1, vPos.z+1))
                );             
            break;
        default:  
            sideCount = getAOSides(
                getVoxel(glm::vec3(vPos.x, vPos.y-1, vPos.z+1)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y-1, vPos.z)), 
                getVoxel(glm::vec3(vPos.x-1, vPos.y-1, vPos.z+1))
                );  
            break; 
    }

    return getAOValue(sideCount);  
}


// returns the int amount of voxels surrounding a given vertex 
int Chunk::getAOSides(char side1, char side2, char corner)  
{

    if (side1 > 0 && side2 > 0) 
        return 0; 

    return 3 - ((bool)side1 + (bool)side2 + (bool)corner); 
}

glm::vec3 Chunk::getAOValue(int numSides)
{ //takes in num of sides and returns an AO dark color value
    switch (numSides) 
    {
        case 0: 
            return glm::vec3(0.4f, 0.4f, 0.4f); 
        case 1: 
            return glm::vec3(0.55f, 0.55f, 0.55f);  
        case 2: 
            return glm::vec3(0.65f, 0.65f, 0.65f);    
        default:  
            return glm::vec3(1.0f, 1.0f, 1.0f); 
    }
}    


// returns data of the coordinate regardless if the coordinate is in the chunk or not, accesses chunk neighbors if outside chunk 
char Chunk::getVoxel(glm::vec3 coordinate)  
{


    // if the voxels exist within the chunk 
    if (coordinate.x < 32 && coordinate.x >= 0 && coordinate.z < 32 && coordinate.z >= 0 && coordinate.y >= 0 && coordinate.y <= 256)   
    {
        int voxelIndex = getVoxelIndex(coordinate); 
        return voxelArray[voxelIndex]; 
    }
        // return voxels[(int)coordinate.x][(int)coordinate.y][(int)coordinate.z];

    // convert coordinates to world coordinates 
    float xPos = (position.x * 32) + coordinate.x;  
    float yPos = (position.y * 32) + coordinate.y;   
    float zPos = (position.z * 32) + coordinate.z;    

    // converting to world coordinates 
    int xChunkPos = floor(xPos / 32); 
    int zChunkPos = floor(zPos / 32);  

    // getting chunk coordiinate world coordinate 
    glm::vec3 chunkLocation = glm::vec3(xChunkPos, 0, zChunkPos);  

    // converting world coordinates of voxels to chunk coordinates  
    int xCoord = xPos - chunkLocation.x * 32; 
    int yCoord = yPos - chunkLocation.y * 32; 
    int zCoord = zPos - chunkLocation.z * 32;  

    // left 
    if (glm::vec3(position.x + 1, 0, position.z) == chunkLocation && mLeft != nullptr)  
    {
        int voxelIndex = mLeft->getVoxelIndex(glm::vec3(xCoord, yCoord, zCoord)); 
        return mLeft->voxelArray[voxelIndex];  
    }

    // right
    else if (glm::vec3(position.x - 1, 0, position.z) == chunkLocation && mRight != nullptr)  
    {
        int voxelIndex = mRight->getVoxelIndex(glm::vec3(xCoord, yCoord, zCoord)); 
        return mRight->voxelArray[voxelIndex];   
    }

    // up 
    else if (glm::vec3(position.x, 0, position.z+1) == chunkLocation && mUp != nullptr)  
    {
        int voxelIndex = mUp->getVoxelIndex(glm::vec3(xCoord, yCoord, zCoord)); 
        return mUp->voxelArray[voxelIndex];    
    }


    // bottom  
    else if (glm::vec3(position.x, 0, position.z-1) == chunkLocation && mBot != nullptr)    
    {
        int voxelIndex = mBot->getVoxelIndex(glm::vec3(xCoord, yCoord, zCoord)); 
        return mBot->voxelArray[voxelIndex];    
    }

    // if no voxel was found 
    return '0';  
}

bool Chunk::isSolid(char voxelValue) 
{
    return (voxelValue > 0); 
}

bool Chunk::isAir(char voxelValue) 
{
    return (voxelValue == 0); 
}

glm::vec3 Chunk::calculateWorldCoordinates(int x, int y, int z) 
{
    int xPos = (position.x * 32) + x;   
    int yPos = (position.y * 32) + y;   
    int zPos = (position.z * 32) + z;     
    return glm::vec3(xPos, yPos, zPos); 
}

void Chunk::draw(Shader& shader, Frustum& frustum)  
{
    // if (frustum.chunkInFrustum(position)) // if point is in frustum   
    // {  
        glm::mat4 model; 
        model = glm::mat4(1.0f); 
        shader.setMat("model", 1, GL_FALSE, model);   
         

        glBindVertexArray(VAO);  
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); 
        glBindVertexArray(0);
    // }
}
