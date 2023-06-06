#include "world/chunk.h" 


 

void Chunk::generateSolidChunk(int randSeed, int startX, int startZ) 
{
    memset(voxels, 0, sizeof(voxels)); 
    const int xs = 33;
    const int zs = 33;
    float frequency = 0.005f; 



    FastNoiseLite mountain; 
    // mountain.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    // mountain.SetFractalType(FastNoiseLite::FractalType_FBm); 
    mountain.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    mountain.SetFractalType(FastNoiseLite::FractalType_Ridged);  
    mountain.SetFrequency(frequency); 
    mountain.SetSeed(randSeed);   
    mountain.SetFractalOctaves(4); 

        // Gather noise data
    int noiseData[xs][zs];
    int stoneLimit = 20; 
    int heighestPoint = -100000;  //  


    for (int x = 0; x < xs; x++) 
    {
        for (int z = 0; z < zs; z++)  
        {
            int height = static_cast<int>((pow(mountain.GetNoise((float)(startX + x), (float)(startZ + z)) + 1.0f, 6.5f)));   
            heighestPoint = std::max(heighestPoint, height); 
            heighestLocation = glm::vec3(x, heighestPoint + stoneLimit, z);   
            noiseData[x][z] = height; // -40      
        }
    }




    // pushing the positions of each block and making the height map  
    for (int i = 0; i < xs; i++) 
    {
        for (int j = 0; j < zs; j++) 
        {
            for (int k = 0; k <  stoneLimit + noiseData[i][j]; k++)   
            {

                int surface = stoneLimit + noiseData[i][j] - 2;

                if (k < surface) { // stone
                    voxels[i][k][j] = 2;
                    
                } else if (k < 80) { // grass 
                    voxels[i][k][j] = 1;
                } else { // snow mountains 
                    voxels[i][k][j] = 3;
                }
            } 
        }
    }  
}

void Chunk::generateDebugChunk(int randSeed, int startX, int startZ) 
{ // 3D noise chunk 
    memset(voxels, 0, sizeof(voxels)); 
    const int xs = 33;
    const int zs = 33;
    float frequency = 0.005f; 

    FastNoiseLite noise; 
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);  
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFrequency(.009);  
    noise.SetRotationType3D(FastNoiseLite::RotationType3D_ImproveXZPlanes);   
    noise.SetSeed(randSeed);  

    FastNoiseLite mountain; 
    mountain.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    mountain.SetFractalType(FastNoiseLite::FractalType_Ridged);  
    mountain.SetFrequency(frequency); 
    mountain.SetSeed(randSeed);   
    mountain.SetFractalOctaves(4); 

        // Gather noise data
    int noiseData[xs][zs];
    int stoneLimit = 20; 
    int heighestPoint = -100000;  //  


    for (int x = 0; x < xs; x++) 
    {
        for (int z = 0; z < zs; z++)  
        {
            int height = static_cast<int>((pow(mountain.GetNoise((float)(startX + x), (float)(startZ + z)) + 1.0f, 6.5f)));  
            heighestPoint = std::max(heighestPoint, height); 
            heighestLocation = glm::vec3(x, heighestPoint + stoneLimit, z); 
            noiseData[x][z] = height; // -40      
        }
    }

    for (int i = 0; i < xs; i++) 
    {
        for (int j = 0; j < zs; j++) 
        {
            for (int k = 0; k <  stoneLimit + noiseData[i][j]; k++)   
            {
                int surface = stoneLimit + noiseData[i][j] - 2;

                if (i == 16) 
                {
                    voxels[i][k][j] = 0;  
                }
                else if (k > 60)    
                {
                    noise.SetFrequency(0.006); 
                    float MultiNoise = noise.GetNoise((float)(startX + i), (float)(k), (float)(startZ + j));    
                    MultiNoise > -0.008f ? voxels[i][k][j] = 3 : voxels[i][k][j] = 0;   
                }
                else if (k < 50 && k > 5)   
                {
                    noise.SetFrequency(0.007); 
                    float MultiNoise = noise.GetNoise((float)(startX + i), (float)(k), (float)(startZ + j));    
                    MultiNoise > -0.008f ? voxels[i][k][j] = 2 : voxels[i][k][j] = 0;    
                }    
                else if (k < surface) { // stone
                    voxels[i][k][j] = 2;
                    
                } else if (k < 80) { // grass 
                    voxels[i][k][j] = 1;
                } else { // snow mountains 
                    voxels[i][k][j] = 3;
                }
            } 
        }
    }  


/*     FastNoiseLite noise; 
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);  
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFrequency(.005);  
    noise.SetRotationType3D(FastNoiseLite::RotationType3D_ImproveXZPlanes);   
    noise.SetSeed(randSeed);   


    for (int x = 0; x < 33; x++) 
    {
        for (int y = 0; y < 33; y++)
        {
            for (int z = 0; z < 33; z++)  
            {
                noiseData[x][y][z] = noise.GetNoise((float)(startX + x), (float)(y), (float)(startZ + z)); // -40   
            }
        }
    }

    for (int x = 0; x < 33; x++) 
    {
        for (int y = 0; y < 33; y++)
        {
            for (int z = 0; z < 33; z++)  
            {
                noiseData[x][y][z] > -0.008f ? voxels[x][y][z] = 1 : voxels[x][y][z] = 0;   
            }
        }
    } */
}


void Chunk::mesh()      
{ // checks interior voxels
    indices.clear(); // clear the indices before each mesh! Very important to fix bug that causes overlapping triangles 
    Voxel voxel; 
    for ( int x = 0; x < 33; x++) 
    {
        for (int y = 0; y < 256; y++) 
        {
            for (int z = 0; z < 33; z++) 
            {   
                if (voxels[x][y][z] > 0) 
                {
                    if ((z+1 < 33) && (z+1 >= 0)) // back 
                    {
                        if (voxels[x][y][z+1] == 0)  
                        {
                            for (unsigned int i = 0; i < 6; i++) 
                                indices.push_back(vertices.size() + backIndices[i]);   

                            for (unsigned int i = 0; i < backFace.size(); i++) 
                            {
                                voxelVertex vertex; 
                                vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + backFace[i];  
                                vertex.Normal = backNormals[i];   
                                vertex.Color = voxel.voxelColors[voxels[x][y][z]]; 
                                vertex.aoValue = glm::vec3(1, 1, 1);  


                                vertices.push_back(vertex);      
                            }
                        }   
                    }

                    if ((z-1 < 34) && (z-1 >= 0)) // front
                    {
                        if (voxels[x][y][z-1] == 0)  
                        {
                            for (unsigned int i = 0; i < 6; i++) 
                                indices.push_back(vertices.size() + frontIndices[i]); 

                            for (unsigned int i = 0; i < frontFace.size(); i++) 
                            {
                                voxelVertex vertex; 
                                vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + frontFace[i];    
                                vertex.Normal = frontNormals[i];  
                                vertex.Color = voxel.voxelColors[voxels[x][y][z]];
                                if (i == 0)         
                                {
                                    vertex.aoValue = glm::vec3(1.0f, 1.0f, 1.0f);   
                                } 
                                else if (i == 1) 
                                {
                                    vertex.aoValue = glm::vec3(0.1f, 0.1f, 0.1f); 
                                }
                                else if (i == 3) 
                                {
                                    vertex.aoValue = glm::vec3(1.0f, 1.0f, 1.0f);  
                                }
                                else 
                                {
                                    vertex.aoValue = glm::vec3(1.0f, 1.0f, 1.0f);  
                                }


                                vertices.push_back(vertex);     
                            }
                        }   
                    }

                    if ((x+1 < 33) && (x+1 >= 0)) // left 
                    {
                        if (voxels[x+1][y][z] == 0)  
                        {
                            for (unsigned int i = 0; i < 6; i++) 
                                indices.push_back(vertices.size() + leftIndices[i]); 

                            for (unsigned int i = 0; i < leftFace.size(); i++) 
                            {
                                voxelVertex vertex; 
                                vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + leftFace[i];    
                                vertex.Normal = leftNormals[i]; 
                                vertex.Color = voxel.voxelColors[voxels[x][y][z]];
                                vertex.aoValue = glm::vec3(1, 1, 1);  



                                vertices.push_back(vertex);     
                            }
                        }   
                    }

                    if ((x-1 < 34) && (x-1 >= 0)) // right 
                    {
                        if (voxels[x-1][y][z] == 0)  
                        {
                            for (unsigned int i = 0; i < 6; i++) 
                                indices.push_back(vertices.size() + rightIndices[i]); 

                            for (unsigned int i = 0; i < rightFace.size(); i++)  
                            {
                                voxelVertex vertex; 
                                vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + rightFace[i];    
                                vertex.Normal = rightNormals[i]; 
                                vertex.Color = voxel.voxelColors[voxels[x][y][z]]; 

                                if (i == 0)         
                                {
                                    vertex.aoValue = glm::vec3(1.0f, 1.0f, 1.0f);   
                                } 
                                else if (i == 1) 
                                {
                                    vertex.aoValue = glm::vec3(1.0f, 1.0f, 1.0f);  
                                }
                                else if (i == 3) 
                                {
                                    vertex.aoValue = glm::vec3(0.1f, 0.1f, 0.1f);  
                                }
                                else 
                                {
                                    vertex.aoValue = glm::vec3(1.0f, 1.0f, 1.0f);  
                                }


                                vertices.push_back(vertex);     
                            }
                        }   
                    }


                    if ((y-1 < 256) && (y-1 >= 0)) // bottom 
                    {
                        if (voxels[x][y-1][z] == 0)  
                        {
                            for (unsigned int i = 0; i < 6; i++) 
                                indices.push_back(vertices.size() + bottomIndices[i]); 

                            for (unsigned int i = 0; i < bottomFace.size(); i++)   
                            {
                                voxelVertex vertex; 
                                vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + bottomFace[i];    
                                vertex.Normal = bottomNormals[i];    
                                vertex.Color = voxel.voxelColors[voxels[x][y][z]];
                                vertex.aoValue = glm::vec3(1, 1, 1);  


                                vertices.push_back(vertex);     
                            }
                        }   
                    }

                    if ((y+1 < 256) && (y+1 >= 0)) // top  
                    {
                        if (voxels[x][y+1][z] == 0)  
                        {
                            for (unsigned int i = 0; i < 6; i++) 
                                indices.push_back(vertices.size() + topIndices[i]); 

                            for (unsigned int i = 0; i < topFace.size(); i++)   
                            {
                                voxelVertex vertex; 
                                vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + topFace[i];    
                                vertex.Normal = topNormals[i];   
                                vertex.Color = voxel.voxelColors[voxels[x][y][z]];

                                if (i == 0)         
                                {
                                    vertex.aoValue = glm::vec3(1.0f, 1.0f, 1.0f);  
                                } 
                                else if (i == 1) 
                                {
                                    vertex.aoValue = glm::vec3(0.1f, 0.1f, 0.1f); 
                                }
                                else if (i == 2) 
                                {
                                    vertex.aoValue = glm::vec3(0.6f, 0.6f, 0.6f); 
                                }
                                else 
                                {
                                    vertex.aoValue = glm::vec3(1.0f, 1.0f, 1.0f);  
                                }


                                vertices.push_back(vertex);     
                            }
                        }   
                    }
                }   
            }
        }
    }    
    setupMesh(); 
    vertices.clear();   
}

void Chunk::setupMesh() 
{
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
 
    
/*     // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)); */

    glBindVertexArray(0);   
}

void Chunk::changeVoxelColor(glm::vec3 voxelCoord) 
{
    voxels[(int)voxelCoord.x][(int)voxelCoord.y][(int)voxelCoord.z] = 3;  
    // get location of vertex in vertices  
    // change color 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(voxelVertex), &vertices[0], GL_STATIC_DRAW);  
}

void Chunk::draw(Shader& shader, Frustum& frustum, float amount)  
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
