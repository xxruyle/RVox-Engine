#include "world/chunk.h" 

void Chunk::generate(int randSeed, int startX, int startZ) 
{ // generates a chunk based in chunksize and returns a vector 
    const int xs = 32;
    const int zs = 32;
    float frequency = 0.005f; 


    FastNoiseLite mountain; 
    mountain.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    mountain.SetFractalType(FastNoiseLite::FractalType_Ridged); 
    mountain.SetFrequency(frequency); 
    mountain.SetSeed(randSeed);   
    mountain.SetFractalOctaves(3); 

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
            } 
            else if (noiseData[i][j] < 0) 
            {
                Voxel voxel(glm::vec3(startX+i, 0.0f, startZ+j), glm::vec3(37.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f));  
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
 