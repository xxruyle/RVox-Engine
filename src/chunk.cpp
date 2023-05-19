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

    for (int i = 0; i < xs; i++) 
    {
        for (int j = 0; j < zs; j++) 
        {
            float color; 

            if (i == 0 ||i == xs - 1 || j == zs - 1 || j == 0) // makes it so that the chunk borders are easily seen 
            {
                Voxel voxel(glm::vec3(startX+i, noiseData[i][j], startZ+j), glm::vec3(1.0f, 0.0f, 0.0f), 1);   
                voxelMap.insert(std::make_pair(glm::vec3(startX+i, noiseData[i][j], startZ+j), voxel));
            } else if (noiseData[i][j] < 40) { // grass 
                // the color for R value 
                if (pow(noiseData[i][j], 1.5) > 136) 
                    color = 136.0f; 
                else 
                    color = 1.0f * pow(noiseData[i][j], 1.5);

                Voxel voxel(glm::vec3(startX+i, noiseData[i][j], startZ+j), glm::vec3(color/255.0f, 205.0f/255.0f, 50.0f/255.0f),  1); 
                voxelMap.insert(std::make_pair(glm::vec3(startX+i, noiseData[i][j], startZ+j), voxel));                
            } else { // snow mountains 
                if (pow(noiseData[i][j], 1.5) < 159) 
                    color = 159.0f;
                else 
                    color = 1.0f * pow(noiseData[i][j], 1.5);


                Voxel voxel(glm::vec3(startX+i, noiseData[i][j], startZ+j), glm::vec3(color/255.0f, 237.0f/255.0f, 252.0f/255.0f), 1);   
                voxelMap.insert(std::make_pair(glm::vec3(startX+i, noiseData[i][j], startZ+j), voxel));
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
    int stoneLimit = 1; 

    for (int x = 0; x < xs; x++) 
    {
        for (int z = 0; z < zs; z++)  
        {
            noiseData[x][z] = static_cast<int>((pow(mountain.GetNoise((float)(startX + x), (float)(startZ + z)) + 1.0f, 5))); // -40   
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

                if ((i == 0 || j == 0) && (k > surface))  
                {
                    Voxel voxel(glm::vec3(startX+i, k, startZ+j), glm::vec3(1.0f, 0.0f, 0.0f), 1);    
                    voxelMap.insert(std::make_pair(glm::vec3(startX+i, k, startZ+j), voxel));                     
                }

                else if (k < surface) // stone 
                {
                    Voxel voxel(glm::vec3(startX+i, k, startZ+j), glm::vec3(130.0f/255.0f, 136.0f/255.0f, 134.0f/255.0f), 1);   
                    voxelMap.insert(std::make_pair(glm::vec3(startX+i, k, startZ+j), voxel));
                    
                } else if (k < 35) { // water 
                    Voxel voxel(glm::vec3(startX+i, k, startZ+j), glm::vec3(62.0f/255.0f, 164.0f/255.0f, 240.0f/255.0f),  1); 
                    voxelMap.insert(std::make_pair(glm::vec3(startX+i, k, startZ+j), voxel));  
                } else if (k < 80) { // grass 
                    // the color for R value 
                    if (pow(k, 1.5) > 136) 
                        color = 136.0f; 
                    else                     
                        color = 1.0f * pow(k, 1.5);
                    
                    Voxel voxel(glm::vec3(startX+i, k, startZ+j), glm::vec3(color/255.0f, 205.0f/255.0f, 50.0f/255.0f), 1); 
                    voxelMap.insert(std::make_pair(glm::vec3(startX+i, k, startZ+j), voxel));   
                        
                    
 
                } else { // snow mountains 
                    if (pow(k, 1.5) < 159) 
                        color = 159.0f;
                    else 
                        color = 1.0f * pow(k, 1.5);
                    Voxel voxel(glm::vec3(startX+i, k, startZ+j), glm::vec3(color/255.0f, 237.0f/255.0f, 252.0f/255.0f), 1);   
                    voxelMap.insert(std::make_pair(glm::vec3(startX+i, k, startZ+j), voxel));
                }
            } 
        }
    }  
}

void Chunk::generateDebugChunk(int randSeed) 
{ // chunk for debugging purposes
    FastNoiseLite noise;  
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);  
    noise.SetSeed(randSeed); 

    int noiseArray[32][32][32];  

    for (unsigned int i = 0; i < 32; i++) 
    {
        for (unsigned int j = 0; j < 32; j++) 
        {
            for (unsigned int k = 0; k < 32; k++) 
            {

                float noiseData = noise.GetNoise((float)i, (float)j, (float)k);  
                int blockType; 
                noiseData > 0 ? blockType = 1 : blockType = 0;  
                noiseArray[i][j][k] = blockType;  
                
                Voxel voxel(glm::vec3(i, j, k), glm::vec3((float)(rand() % 255+1)/255.0f, (float)(rand() % 255+1)/255.0f, (float)(rand() % 255+1)/255.0f), 1);  
                voxelMap.insert(std::make_pair(glm::vec3(i,j,k), voxel));  
            }
        }
    }
}


void Chunk::mesh()      
{ // checks interior voxels

    glm::vec3 faceArray[6]; 

    // unsigned int indice = 0; 
    for (auto& voxel : voxelMap) 
    {
        faceArray[up] = glm::vec3(voxel.second.coordinates.x, voxel.second.coordinates.y + 1, voxel.second.coordinates.z); // up
        faceArray[down] = glm::vec3(voxel.second.coordinates.x, voxel.second.coordinates.y - 1, voxel.second.coordinates.z); // down
        faceArray[front] = glm::vec3(voxel.second.coordinates.x, voxel.second.coordinates.y, voxel.second.coordinates.z - 1); // front
        faceArray[back] = glm::vec3(voxel.second.coordinates.x, voxel.second.coordinates.y, voxel.second.coordinates.z + 1); // back
        faceArray[left] = glm::vec3(voxel.second.coordinates.x + 1, voxel.second.coordinates.y, voxel.second.coordinates.z); // left
        faceArray[right] = glm::vec3(voxel.second.coordinates.x - 1, voxel.second.coordinates.y, voxel.second.coordinates.z); // right

        if ((voxelMap.find(faceArray[back]) == voxelMap.end())) // back face 
        {
            for (unsigned int i = 0; i < 6; i++) 
                indices.push_back(vertices.size() + backIndices[i]);   

            for (unsigned int i = 0; i < backFace.size(); i++) 
            {
                Vertex vertex; 
                vertex.Position = voxel.second.coordinates + backFace[i]; 
                vertex.Normal = backNormals[i];   
                vertices.push_back(vertex);      
            }
        }


        if ((voxelMap.find(faceArray[front]) == voxelMap.end())) // front face
        {
            for (unsigned int i = 0; i < 6; i++) 
                indices.push_back(vertices.size() + frontIndices[i]); 

            for (unsigned int i = 0; i < frontFace.size(); i++) 
            {
                Vertex vertex; 
                vertex.Position = voxel.second.coordinates + frontFace[i];   
                vertex.Normal = frontNormals[i];  
                vertices.push_back(vertex);     
            }
        }


        if ((voxelMap.find(faceArray[left]) == voxelMap.end())) // left face  
        {
            for (unsigned int i = 0; i < 6; i++) 
                indices.push_back(vertices.size() + leftIndices[i]); 

            for (unsigned int i = 0; i < leftFace.size(); i++) 
            {
                Vertex vertex; 
                vertex.Position = voxel.second.coordinates + leftFace[i];  
                vertex.Normal = leftNormals[i]; 
                vertices.push_back(vertex);   
            }
        }


        if ((voxelMap.find(faceArray[right]) == voxelMap.end())) // right face  
        {
            for (unsigned int i = 0; i < 6; i++) 
                indices.push_back(vertices.size() + rightIndices[i]);  
 
            for (unsigned int i = 0; i < rightFace.size(); i++) 
            {
                Vertex vertex; 
                vertex.Position = voxel.second.coordinates + rightFace[i]; 
                vertex.Normal = rightNormals[i]; 
                vertices.push_back(vertex);      
            }
        }


        if ((voxelMap.find(faceArray[down]) == voxelMap.end())) // bottom face
        {
            for (unsigned int i = 0; i < 6; i++) 
                indices.push_back(vertices.size() + bottomIndices[i]);   


            for (unsigned int i = 0; i < bottomFace.size(); i++) 
            {
                Vertex vertex; 
                vertex.Position = voxel.second.coordinates + bottomFace[i]; 
                vertex.Normal = bottomNormals[i]; 
                vertices.push_back(vertex);       
            }
        }


        if ((voxelMap.find(faceArray[up]) == voxelMap.end())) // top face 
        {
            for (unsigned int i = 0; i < 6; i++) 
                indices.push_back(vertices.size() + topIndices[i]); 

            for (unsigned int i = 0; i < topFace.size(); i++) 
            {
                Vertex vertex; 
                vertex.Position = voxel.second.coordinates + topFace[i]; 
                vertex.Normal = topNormals[i];  
                vertices.push_back(vertex);        
            }
        }
    } 
    setupMesh(); 
    vertices.clear(); 

/*     std::cout << vertices.size();
    for (unsigned int i = 0; i < vertices.size(); i++) 
    {
        std::cout << vertices[i].Position.x << ' ' << vertices[i].Position.y << ' ' << vertices[i].Position.z << std::endl;  
    }

    for (unsigned int i = 0; i < indices.size(); i++) 
    {
        std::cout << indices[i];   
    } */

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
    
/*     // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)); */

    glBindVertexArray(0);   
}

void Chunk::draw(Shader& shader, Frustum& frustum)
{
/*     if (frustum.pointInFrustum(glm::vec3(position.x + 15, position.y + 15, position.z +16))) // if point is in frustum  
    {  */
    glm::mat4 model; 
    model = glm::mat4(1.0f); 
    shader.setMat("model", 1, GL_FALSE, model);  
    shader.setVec3("voxelColor", 1.0, 1.0, 1.0);  

    glBindVertexArray(VAO);  
    // glDrawArrays(GL_TRIANGLES, 0, vertices.size());     
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); 
    glBindVertexArray(0);
    // }
}
