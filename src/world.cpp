#include "world/world.h"

void World::generatePlane(int randSeed)
{
    const int xs = 32;
    const int ys = 32;
    float amplitudeMultiplier = 50.0f; 
    float frequency = 0.02f; 
    
    // Create and configure FastNoise object
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm); 
    noise.SetFrequency(frequency); 
    noise.SetSeed(randSeed);



    // Gather noise data
    int noiseData[xs][ys];

    for (int x = 0; x < xs; x++)
    {
        for (int y = 0; y < ys; y++)
        {
            noiseData[x][y] = static_cast<int>(((noise.GetNoise((float)x, (float)y) + 1.0f) * amplitudeMultiplier) - 40.0f);
        }
    }


    // pushing the positions of each block and making the height map  
    for (int i = 0; i < xs; i++) 
    {
        for (int j = 0; j < ys; j++) 
        {
            if (noiseData[i][j] > 0)
            {
                for (int k = 0; k < noiseData[i][j]; k++)
                {
                    positions.push_back(glm::vec3(i, k, j)); 
                }
            } else if (noiseData[i][j] < 0) {
                positions.push_back(glm::vec3(i, noiseData[i][j], j));                 
            } else {
                positions.push_back(glm::vec3(i, 0.0f, j)); 
            }
            
        }
    } 

    for (int i = 0; i < xs; i++) 
    {
        for (int j = 0; j < ys; j++) 
        {
            std::cout << noiseData[i][j]; 
        }

        std::cout << "\n";  
    }

    noiseData[0][0] = 0.0f;  // using the array 
    
}

void World::generateSingle()
{
    positions.push_back(glm::vec3(0.0f, 0.0f, 0.0f)); 
}



void World::printCubes() 
{
    std::cout << "Size: " << positions.size() << std::endl; 
}