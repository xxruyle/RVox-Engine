# May 20, 2023 
1) Need to meshing problem where deletion of chunks are not accepted by chunk neighbors 
2) Add coloring to voxels 
    3) Make another voxel array, with rgb vec3s, send it to the gpu, and then clear   
3) Add collision 
4) Add placing blocks
5) Frustum Culling (chunks only) 


# Meshing Neighboring Chunks 
Use Chunk Manager class to give chunks information about neighboring chunks if they exist 

## Checking neighboring chunks 
- One solution is to use the chunkmanager class' hashmap to figure out the neighboring chunks 
- Have the mesh function in the chunk class go from 1 < x,z < 33, y range will be as normal from 0 < y <  256
- the meshNeighbors function will add the 0 and 34 x and z values to the array 
- remove the noise generation on the borders in the generateSolidChunk chunk function 
Pseudocode
```C++
void meshNeighbors(Chunk& chunk) 
...
// for the right neighbor case 
if chunkMap[glm::vec3(chunk.position.x + 1, ..., ...)].count() // using the given chunks current position to find its neighbors 
{
    // give chunkmap chunk border info 
    // for instance, if chunk's right neighbor exists 
    for (unsigned int z = 0; z < 32; x++) // only need to go over the z axis since the x axis remains constant  
    {
        for (unsigned int y = 0; y < 256; z++) 
        {   
            chunkMap[glm::vec3(chunk.position.x + 1, ..., ...)].voxels[0][y][z] = chunk.voxels[34][y][z]   // update chunk's  neighbor's border b
        }
    }
    ...

    // finally, we mesh the chunk itself 
    chunk.mesh(); 

    // and then mesh the right neighbor as well 
    chunkMap[glm::vec3(chunk.position.x + 1, chunk.position.y, chunk.position.z)].mesh(); 
}

// repeat for the other 3 neighbors 
```

## Downsides to above solution
- Meshing the whole chunk is unecessary, could just have a function that adds vertexes to the chunk borders 
- The count function has non constant complexity. Could be slow. 
- Could be slow considering that worst case is that a chunk has all 4 neighbors, that means 4 calls to mesh per chunk 
- If both neighbors check each other, will there be some overlap or scenario where one chunk is updated but the other is not? 
  - I don't think so since we will call the meshNeighbors function each time a chunk changes and 
 

## SuperChunk Solution
- Have superchunk contain the chunkBuffer chunks\
- Mesh all these chunks as one chunk ? 


# Coloring Voxels 
## Problems 
Takes a lot of memory to store color vec3s for each voxel  



