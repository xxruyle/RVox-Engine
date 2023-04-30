#include "world/world.h"

void World::generateLand(int randSeed, bool printHeights)
{
    positions.clear(); 
    const int xs = 248;
    const int ys = 248;
    float amplitudeMultiplier = 20.0f; // 50 i like 
    float frequency = 0.02f; // .02 i like 
    
    // Create and configure FastNoise object
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm); 
    noise.SetFrequency(frequency); 
    noise.SetSeed(randSeed);

    FastNoiseLite mountain; 
    mountain.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    mountain.SetFractalType(FastNoiseLite::FractalType_Ridged); 
    mountain.SetFrequency(frequency); 
    mountain.SetSeed(randSeed);


    // Gather noise data
    int noiseData[xs][ys];

    for (int x = 0; x < xs; x++)
    {
        for (int y = 0; y < ys; y++)
        {
            noiseData[x][y] = static_cast<int>(((noise.GetNoise((float)x, (float)y) + 1.0f) * amplitudeMultiplier) - 40.0f); // -40 
            // noiseData[x][y] = static_cast<int>(((1 - std::abs(mountain.GetNoise((float)x, (float)y))) * amplitudeMultiplier)); 
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

    std::cout << randSeed << std::endl; // printing the seed
}

void World::generateSingle()
{
    positions.push_back(glm::vec3(0.0f, 0.0f, 0.0f)); 
}

void World::generatePlane() 
{
    for (int i = 0; i < 20; i++) 
    {
        for (int j = 0; j < 20; j++) 
        {
            positions.push_back(glm::vec3((float)i, 0.0f, (float)j));
        }
    }
}


void World::printCubes() 
{
    std::cout << "Size: " << positions.size() << std::endl; 
}