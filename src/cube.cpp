#include <cube/cube.h> 

void Cube::generatePlane(int randSeed)
{
    int xs = 32;
    int ys = 32;
    float amplitudeMultiplier = 50.0f; 
    float frequency = 0.01f; 
    
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
    
}

void Cube::generateSingle()
{
    positions.push_back(glm::vec3(0.0f, 0.0f, 0.0f)); 
}

void Cube::getNoise()
{
    // Create and configure FastNoise object
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);


    // Gather noise data
    int noiseData[32][32];

    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 32; x++)
        {
            for (int z = 0; z < 32; z++)
            {
                noiseData[x][y] = static_cast<int>(noise.GetNoise((float)x, (float)y) * 10.0f);
                std::cout << std::to_string(noise.GetNoise((float)x, (float)y) * 10.0f) << std::endl; 
            }
        }
    }
}

void Cube::printCubes() 
{
    for (unsigned int i = 0; i < positions.size(); i++)
    {
        std::cout << positions[i].x << " " << positions[i].y << " " << positions[i].z << std::endl;
        
    }

    std::cout << "Size: " << positions.size() << std::endl; 
}