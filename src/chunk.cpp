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
    FastNoiseLite noise; 
    noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);  
    noise.SetFractalType(FastNoiseLite::FractalType_Ridged);
    noise.SetFrequency(.08);  
    noise.SetRotationType3D(FastNoiseLite::RotationType3D_ImproveXZPlanes);   
    noise.SetSeed(randSeed);   

        // Gather noise data
    float noiseData[33][100][33];

    for (int x = 0; x < 33; x++) 
    {
        for (int y = 0; y < 100; y++)
        {
            for (int z = 0; z < 33; z++)  
            {
                noiseData[x][y][z] = noise.GetNoise((float)(startX + x), (float)(y), (float)(startZ + z)); // -40   
            }
        }
    }

    for (int x = 0; x < 33; x++) 
    {
        for (int y = 0; y < 100; y++)
        {
            for (int z = 0; z < 33; z++)  
            {
                noiseData[x][y][z] > -0.71f ? voxels[x][y][z] = 1 : voxels[x][y][z] = 0;   
            }
        }
    }
}


void Chunk::mesh()      
{ // checks interior voxels
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
                            // vertex.Color = voxelColors[x][y][z];   
                            // vertex.Color = glm::vec3(75.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f);  
                            vertex.Color = voxel.voxelColors[voxels[x][y][z]];
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

void Chunk::draw(Shader& shader, Frustum& frustum, float renderDistance) 
{
     if (frustum.chunkInFrustum(position)) // if point is in frustum   
    {  
        glm::mat4 model; 
        model = glm::mat4(1.0f); 
        shader.setMat("model", 1, GL_FALSE, model);  
        shader.setFloat("renderDistance", renderDistance);
         

        glBindVertexArray(VAO);  
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); 
        glBindVertexArray(0);
    }
}
