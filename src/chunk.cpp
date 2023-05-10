#include "world/chunk.h" 

void Chunk::generate(int randSeed, int startX, int startZ) 
{ // generates a chunk based in chunksize and returns a vector 
    const int xs = 32;
    const int zs = 32;
    float frequency = 0.008f; 


    FastNoiseLite mountain; 
    mountain.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    mountain.SetFractalType(FastNoiseLite::FractalType_Ridged); 
    mountain.SetFrequency(frequency); 
    mountain.SetSeed(randSeed);   
    mountain.SetFractalOctaves(2); 

        // Gather noise data
    int noiseData[xs][zs];

    for (int x = 0; x < xs; x++) 
    {
        for (int z = 0; z < zs; z++)  
        {
            noiseData[x][z] = static_cast<int>((pow(mountain.GetNoise((float)(startX + x), (float)(startZ + z)) + 1.0f, 6))); // -40   
            // noiseData[x][y] = static_cast<int>(((1 - abs(mountain.GetNoise((float)x, (float)y))))); 
        }
    }


    // pushing the positions of each block and making the height map  
    /* for (int i = 0; i < xs; i++) 
    {
        for (int j = 0; j < zs; j++) 
        {
            for (int k = 0; k < noiseData[i][j]; k++) 
            {
                float color; 

                if (noiseData[i][j] < 0) 
                {
                    Voxel voxel(glm::vec3(startX+i, 0.0f, startZ+j), glm::vec3(37.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f));  
                    voxels.push_back(voxel); 
                } else if (k < 40) { // grass 
                    // the color for R value 
                    if (pow(k, 1.5) > 136) 
                        color = 136.0f; 
                    else 
                        color = 1.0f * pow(k, 1.5);

                    Voxel voxel(glm::vec3(startX+i, k, startZ+j), glm::vec3(color/255.0f, 205.0f/255.0f, 50.0f/255.0f)); 
                    voxels.push_back(voxel);                 
                } else { // snow mountains 
                    if (pow(k, 1.5) < 159) 
                        color = 159.0f;
                    else 
                        color = 1.0f * pow(k, 1.5);


                    Voxel voxel(glm::vec3(startX+i, k, startZ+j), glm::vec3(color/255.0f, 237.0f/255.0f, 252.0f/255.0f));   
                    voxels.push_back(voxel); 
                }
            } 
        }
    }  */

    for (int i = 0; i < xs; i++) 
    {
        for (int j = 0; j < zs; j++) 
        {
            float color; 

            if (i == 0 ||i == xs - 1 || j == zs - 1 || j == 0) // makes it so that the chunk borders are easily seen 
            {
                Voxel voxel(glm::vec3(startX+i, noiseData[i][j], startZ+j), glm::vec3(1.0f, 0.0f, 0.0f));   
                voxels.push_back(voxel); 
            } else if (noiseData[i][j] < 40) { // grass 
                // the color for R value 
                if (pow(noiseData[i][j], 1.5) > 136) 
                    color = 136.0f; 
                else 
                    color = 1.0f * pow(noiseData[i][j], 1.5);

                Voxel voxel(glm::vec3(startX+i, noiseData[i][j], startZ+j), glm::vec3(color/255.0f, 205.0f/255.0f, 50.0f/255.0f)); 
                voxels.push_back(voxel);                 
            } else { // snow mountains 
                if (pow(noiseData[i][j], 1.5) < 159) 
                    color = 159.0f;
                else 
                    color = 1.0f * pow(noiseData[i][j], 1.5);


                Voxel voxel(glm::vec3(startX+i, noiseData[i][j], startZ+j), glm::vec3(color/255.0f, 237.0f/255.0f, 252.0f/255.0f));   
                voxels.push_back(voxel); 
            }
        }
    } 
}
 

void Chunk::generateSolidChunk(int randSeed, int startX, int startZ) 
{
    const int xs = 32;
    const int zs = 32;
    float frequency = 0.008f; 


    FastNoiseLite mountain; 
    mountain.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    mountain.SetFractalType(FastNoiseLite::FractalType_Ridged); 
    mountain.SetFrequency(frequency); 
    mountain.SetSeed(randSeed);   
    mountain.SetFractalOctaves(2); 

        // Gather noise data
    int noiseData[xs][zs];
    int stoneLimit = 20; 

    for (int x = 0; x < xs; x++) 
    {
        for (int z = 0; z < zs; z++)  
        {
            noiseData[x][z] = static_cast<int>((pow(mountain.GetNoise((float)(startX + x), (float)(startZ + z)) + 1.0f, 6))); // -40   
            // noiseData[x][y] = static_cast<int>(((1 - abs(mountain.GetNoise((float)x, (float)y))))); 
        }
    }


    // pushing the positions of each block and making the height map  
    for (int i = 0; i < xs; i++) 
    {
        for (int j = 0; j < zs; j++) 
        {
            for (int k = 0; k <  stoneLimit + noiseData[i][j]; k++)   
            {
                float color; 

                int surface = stoneLimit + noiseData[i][j] - 4;

                if (k < surface) // stone 
                {
                    Voxel voxel(glm::vec3(startX+i, k, startZ+j), glm::vec3(130.0f/255.0f, 136.0f/255.0f, 134.0f/255.0f));   
                    voxelMap.insert(std::make_pair(glm::vec3(startX+i, k, startZ+j), voxel));
                   
                } else if (k < 35) { // water 
                    Voxel voxel(glm::vec3(startX+i, k, startZ+j), glm::vec3(62.0f/255.0f, 164.0f/255.0f, 240.0f/255.0f)); 
                    voxelMap.insert(std::make_pair(glm::vec3(startX+i, k, startZ+j), voxel));   
                } else if (k < 80) { // grass 
                    // the color for R value 
                    if (pow(k, 1.5) > 136) 
                        color = 136.0f; 
                    else 
                        color = 1.0f * pow(k, 1.5);

                    Voxel voxel(glm::vec3(startX+i, k, startZ+j), glm::vec3(color/255.0f, 205.0f/255.0f, 50.0f/255.0f)); 
                    voxelMap.insert(std::make_pair(glm::vec3(startX+i, k, startZ+j), voxel));    
                } else { // snow mountains 
                    if (pow(k, 1.5) < 159) 
                        color = 159.0f;
                    else 
                        color = 1.0f * pow(k, 1.5);


                    Voxel voxel(glm::vec3(startX+i, k, startZ+j), glm::vec3(color/255.0f, 237.0f/255.0f, 252.0f/255.0f));   
                    voxelMap.insert(std::make_pair(glm::vec3(startX+i, k, startZ+j), voxel));  
                }
            } 
        }
    }  
}

void Chunk::generateDebugChunk() 
{ // chunk for debugging purposes
    for (unsigned int i = 0; i < 4; i++) 
    {
        for (unsigned int j = 0; j < 4; j++) 
        {
            for (unsigned int k = 0; k < 4; k++) 
            {
                Voxel voxel(glm::vec3(i, j,k), glm::vec3(pow(i,5)/255.0f, pow(j,5)/255.0f, pow(k,5)/255.0f));  
                voxelMap.insert(std::make_pair(glm::vec3(i,j,k), voxel));  
            }
        }
    }

    // front = voxels[current index + coordinates.z + 1], if block is not front(x, y, z + coordinates.z + 1) then block does not exist   
}

void Chunk::checkInteriorVoxels()    
{ // checks interior voxels
    glm::vec3 faceArray[6]; 
    enum faces {
        up, down, front, back, left, right 
    };

    for (auto& voxel : voxelMap) 
    {
        faceArray[0] = glm::vec3(voxel.second.coordinates.x, voxel.second.coordinates.y + 1, voxel.second.coordinates.z);
        faceArray[1] = glm::vec3(voxel.second.coordinates.x, voxel.second.coordinates.y - 1, voxel.second.coordinates.z); 
        faceArray[2] = glm::vec3(voxel.second.coordinates.x, voxel.second.coordinates.y, voxel.second.coordinates.z - 1); 
        faceArray[3] = glm::vec3(voxel.second.coordinates.x, voxel.second.coordinates.y, voxel.second.coordinates.z + 1);
        faceArray[4] = glm::vec3(voxel.second.coordinates.x + 1, voxel.second.coordinates.y, voxel.second.coordinates.z); 
        faceArray[5] = glm::vec3(voxel.second.coordinates.x - 1, voxel.second.coordinates.y, voxel.second.coordinates.z);



        for (unsigned int i = 0; i < 6; i++) 
        {

            if ((voxelMap.count(faceArray[i])))  
            {                
                voxel.second.isInterior = true; 
            } else { 
                voxel.second.isInterior = false;  
                break; 
            }
        }

    }


    

}

void Chunk::getVoxelNeighbors() 
{

}