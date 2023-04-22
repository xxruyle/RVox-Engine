#include <cube/cube.h> 

void Cube::generatePlane()
{
    for (float i = 0.0f; i < 32.0f; i+= 0.5) // makes a row of cubes 
    {
        // float k = rand() % 10 + 1; 
        for (float j = 0.0f; j < 3.0; j += 0.5) 
        {
            for (float l = 0.0f; l < 32.0f; l += 0.5) 
            {
                positions.push_back(glm::vec3{i, j, l}); 
            }
        }
    }
}