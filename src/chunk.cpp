#include "world/chunk.h" 


 

void Chunk::generateSolidChunk(int randSeed, int startX, int startZ) 
{
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

    for (int x = 0; x < xs; x++) 
    {
        for (int z = 0; z < zs; z++)  
        {
            noiseData[x][z] = static_cast<int>((pow(mountain.GetNoise((float)(startX + x), (float)(startZ + z)) + 1.0f, 6.5))); // -40    
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

/*                 if ((i == 0 || j == 0) && (k > surface))  // for chunk borders 
                {
                    voxels[i][k][j] = 1;
                    voxelColors[i][k][j] = glm::vec3(1.0f, 0.0f, 0.0f); 
/*                       Voxel voxel(glm::vec3(startX+i, k, startZ+j), glm::vec3(1.0f, 0.0f, 0.0f), 1);    
                    voxelMap.insert(std::make_pair(glm::vec3(startX+i, k, startZ+j), voxel));   */                     
                // } */

                if (k < surface) { // stone
                    voxels[i][k][j] = 1;
                    // voxelColors[i][k][j] = glm::vec3(130.0f/255.0f, 136.0f/255.0f, 134.0f/255.0f);  
/*                      Voxel voxel(glm::vec3(startX+i, k, startZ+j), glm::vec3(130.0f/255.0f, 136.0f/255.0f, 134.0f/255.0f), 1);   
                    voxelMap.insert(std::make_pair(glm::vec3(startX+i, k, startZ+j), voxel));  */ 
                    
                } else if (k < 80) { // grass 
                    // the color for R value 
                    if (pow(k - stoneLimit, 1.5) > 148)  
                        color = 148.0f; 
                    else                     
                        color = 1.0f * pow(k - stoneLimit, 1.5); 

                    voxels[i][k][j] = 1;
                    // voxelColors[i][k][j] = glm::vec3(color/255.0f, 205.0f/255.0f, 50.0f/255.0f); 
                    // voxelColors[i][k][j] = glm::vec3(color/255.0f, 242.0f/255.0f, 244.0f/255.0f);
/*                       Voxel voxel(glm::vec3(startX+i, k, startZ+j), glm::vec3(color/255.0f, 205.0f/255.0f, 50.0f/255.0f), 1); 
                    voxelMap.insert(std::make_pair(glm::vec3(startX+i, k, startZ+j), voxel));  */    
                        
                    
 
                } else { // snow mountains 
         /*            if (pow(k, 1.5) < 208) 
                        color = 208.0f; 
                    else 
                        color = 90.0f * pow(k, 1.5); */
                    voxels[i][k][j] = 1;
                    // voxelColors[i][k][j] = glm::vec3(236.0f/255.0f, 255.0f/255.0f, 253.0f/255.0f);  
/*                      Voxel voxel(glm::vec3(startX+i, k, startZ+j), glm::vec3(color/255.0f, 237.0f/255.0f, 252.0f/255.0f), 1);   
                    voxelMap.insert(std::make_pair(glm::vec3(startX+i, k, startZ+j), voxel));  */
                }
            } 
        }
    }  
}

void Chunk::generateDebugChunk(int randSeed) 
{ // chunk for debugging purposes
    FastNoiseLite noise; 
    noise.SetSeed(randSeed);   

        // Gather noise data
    float noiseData[32][32][32];

    for (int x = 0; x < 32; x++) 
    {
        for (int y = 0; y < 32; y++)
        {
            for (int z = 0; z < 32; z++)  
            {
                noiseData[x][y][z] = noise.GetNoise((float)(x), (float)(y), (float)(z)); // -40  
            }
        }
    }

    for (int x = 0; x < 32; x++) 
    {
        for (int y = 0; y < 32; y++)
        {
            for (int z = 0; z < 32; z++)  
            {
                noiseData[x][y][z] > 0.0f ? voxels[x][y][z] = 1 : voxels[x][y][z] = 0;   

                   
/*                 Voxel voxel(glm::vec3(x, y, z), glm::vec3(1.0f, 1.0f, 1.0f), 1);    
                voxelMap.insert(std::make_pair(glm::vec3(x, y, z), voxel));  */
            }
        }
    }
}


void Chunk::mesh()      
{ // checks interior voxels


    // float color; 
    // glm::vec3 colorVec;  
    // colorVec = glm::vec3(75.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f);   

    for ( int x = 0; x < 33; x++) 
    {
        for (int y = 0; y < 256; y++) 
        {
            for (int z = 0; z < 33; z++) 
            {   
                


                // color = y * 3; 


/*                 if (y < 20)  
                    colorVec = glm::vec3(130.0f/255.0f, 136.0f/255.0f, 134.0f/255.0f); 
                else if (y < 80) 
                    colorVec = glm::vec3(75.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f);
                else if (y < 256) 
                    colorVec = glm::vec3(75.0f/255.0f, 237.0f/255.0f, 252.0f/255.0f);  */
                


                if (voxels[x][y][z] == 1)
                {

                
                if ((z+1 < 33) && (z+1 >= 0)) // back 
                {
                    if (voxels[x][y][z+1] == 0)  
                    {
                        for (unsigned int i = 0; i < 6; i++) 
                            indices.push_back(vertices.size() + backIndices[i]);   

                        for (unsigned int i = 0; i < backFace.size(); i++) 
                        {
                            Vertex vertex; 
                            vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + backFace[i];  
                            vertex.Normal = backNormals[i];   
                            // vertex.Color = voxelColors[x][y][z];  
                            vertex.Color = glm::vec3(75.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f); 
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
                            Vertex vertex; 
                            vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + frontFace[i];    
                            vertex.Normal = frontNormals[i];  
                            // vertex.Color = voxelColors[x][y][z]; 
                            vertex.Color = glm::vec3(75.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f); 
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
                            Vertex vertex; 
                            vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + leftFace[i];    
                            vertex.Normal = leftNormals[i]; 
                            // vertex.Color = voxelColors[x][y][z];  
                            vertex.Color = glm::vec3(75.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f); 
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
                            Vertex vertex; 
                            vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + rightFace[i];    
                            vertex.Normal = rightNormals[i]; 
                            // vertex.Color = voxelColors[x][y][z]; 
                            vertex.Color = glm::vec3(75.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f);    
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
                            Vertex vertex; 
                            vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + bottomFace[i];    
                            vertex.Normal = bottomNormals[i];    
                            // vertex.Color = voxelColors[x][y][z];  
                            vertex.Color = glm::vec3(75.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f); 
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
                            Vertex vertex; 
                            vertex.Position = glm::vec3(position.x * 32, 0, position.z * 32) + glm::vec3(x,y,z) + topFace[i];    
                            vertex.Normal = topNormals[i];   
                            // vertex.Color = voxelColors[x][y][z];   
                            vertex.Color = glm::vec3(75.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f);  
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
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);             


    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW); 


    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));      


    // vertex colors
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color)); 
    
/*     // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)); */

    glBindVertexArray(0);   
}

void Chunk::draw(Shader& shader, Frustum& frustum)
{
     if (frustum.chunkInFrustum(position)) // if point is in frustum   
    {  
        glm::mat4 model; 
        model = glm::mat4(1.0f); 
        shader.setMat("model", 1, GL_FALSE, model);  
        // shader.setVec3("voxelColor", 75.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f);  

        glBindVertexArray(VAO);  
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); 
        glBindVertexArray(0);
    }
}
