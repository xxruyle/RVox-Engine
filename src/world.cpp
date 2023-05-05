#include "world/world.h"

void World::generateLand(int randSeed, bool printHeights)
{
    voxelVec.clear(); 
    const int xs = 500;
    const int ys = 500;
    // float amplitudeMultiplier = 25.0f; // 50 i like 
    float frequency = 0.005f; // .02 i like 
    
    // Create and configure FastNoise object
/*     FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm); 
    noise.SetFrequency(frequency); 
    noise.SetSeed(randSeed); */

    FastNoiseLite mountain; 
    mountain.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    mountain.SetFractalType(FastNoiseLite::FractalType_Ridged); 
    mountain.SetFrequency(frequency); 
    mountain.SetSeed(randSeed);
    mountain.SetFractalOctaves(3); 

    // 1817 is a cool seed ;)


    // Gather noise data
    int noiseData[xs][ys];

    for (int x = 0; x < xs; x++)
    {
        for (int y = 0; y < ys; y++)
        {
            noiseData[x][y] = static_cast<int>((pow(mountain.GetNoise((float)x, (float)y) + 1.0f, 6.5))); // -40 
            // noiseData[x][y] = static_cast<int>(((1 - abs(mountain.GetNoise((float)x, (float)y))))); 
        }
    }


    // pushing the positions of each block and making the height map  
/*     for (int i = 0; i < xs; i++) 
    {
        for (int j = 0; j < ys; j++) 
        {
            for (int k = 0; k < noiseData[i][j]; k++) 
            {
                float color; 

                if (noiseData[i][j] < 0) 
                {
                    Voxel voxel(glm::vec3(i, 0.0f, j), glm::vec3(37.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f));  
                    voxelVec.push_back(voxel); 
                } else if (k < 40) { // grass 
                    // the color for R value 
                    if (pow(k, 1.5) > 136) 
                        color = 136.0f; 
                    else 
                        color = 1.0f * pow(k, 1.5);

                    Voxel voxel(glm::vec3(i, k, j), glm::vec3(color/255.0f, 205.0f/255.0f, 50.0f/255.0f)); 
                    voxelVec.push_back(voxel);                 
                } else { // snow mountains 
                    if (pow(k, 1.5) < 159) 
                        color = 159.0f;
                    else 
                        color = 1.0f * pow(k, 1.5);


                    Voxel voxel(glm::vec3(i, k, j), glm::vec3(color/255.0f, 237.0f/255.0f, 252.0f/255.0f));   
                    voxelVec.push_back(voxel); 
                }
            } 
        }
    }  */

    // height map implementation without "underground depth" i.e there are blocks below the highest block per x,y coordinate
    for (int i = 0; i < xs; i++) 
    {
        for (int j = 0; j < ys; j++) 
        {
            float color; 

            if (noiseData[i][j] < 0) 
            {
                Voxel voxel(glm::vec3(i, 0.0f, j), glm::vec3(37.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f));  
                voxelVec.push_back(voxel); 
            } else if (noiseData[i][j] < 40) { // grass 
                // the color for R value 
                if (pow(noiseData[i][j], 1.5) > 136) 
                    color = 136.0f; 
                else 
                    color = 1.0f * pow(noiseData[i][j], 1.5);

                Voxel voxel(glm::vec3(i, noiseData[i][j], j), glm::vec3(color/255.0f, 205.0f/255.0f, 50.0f/255.0f)); 
                voxelVec.push_back(voxel);                 
            } else { // snow mountains 
                if (pow(noiseData[i][j], 1.5) < 159) 
                    color = 159.0f;
                else 
                    color = 1.0f * pow(noiseData[i][j], 1.5);


                Voxel voxel(glm::vec3(i, noiseData[i][j], j), glm::vec3(color/255.0f, 237.0f/255.0f, 252.0f/255.0f));   
                voxelVec.push_back(voxel); 
            }
        }
    } 


    if (printHeights)
    {
        for (int i = 0; i < xs; i++) 
        {
            for (int j = 0; j < ys; j++) 
            {
                std::cout << noiseData[i][j]; 
            }

            std::cout << "\n";  
        }
    }


    noiseData[0][0] = 0.0f;  // using the array 

    std::cout << "Seed: " << randSeed << std::endl; // printing the seed
}

void World::generateSingle()
{
    positions.push_back(glm::vec3(0.0f, 0.0f, 0.0f)); 
}

void World::generatePlane() 
{
    for (int k = 0; k < 2; k++) 
    {
        for (int i = 0; i < 10; i++) 
        {
            for (int j = 0; j < 10; j++) 
            {
                positions.push_back(glm::vec3((float)i, (float)k, (float)j));
            }
        }
    }

}


void World::printCubes() 
{
    std::cout << "Total Voxels: " << voxelVec.size() << std::endl; 
}