# ToDo 
- [X] Need to meshing problem where deletion of chunks are not accepted by chunk neighbors (**COMPLETED**: May 22, 2023)
- [X] Left chunk border problem when meshing (voxel color tied with it ?) (**COMPLETED**: May 22, 2023)
- [X] Face culling and fixing the ebo indices setup (**COMPLETED**: May 22, 2023)
- [ ] Chunk deletion problem where chunk corners do not mesh properly 
- [X] Frustum culling chunks (**COMPLETED**: May 22, 2023)
- [ ] Refactoring 
  - [X] main
  - [ ] camera and orthocamera 
  - [ ] chunkmanager and chunk
  - [X] world (deleted) 
  - [X] vao, vbo ebo
  - [ ] renderer and chunk draw functions 
  - [ ] Entity superclass over player 
  - [ ] ChunkManager
- [X] ~~Model loading with assimp~~ Model loading with happly.h for ply files  (**COMPLETED**: May 25, 2023)
  - [X] get normals for each vertex in ply file  
  - [X] Scale the models down to world voxel size   
- [X] Add coloring to voxels (**COMPLETED**: May 24, 2023) 
- [X] rendering fog (**COMPLETED**: May 24, 2023) 
- [ ] Mesh loading time optimizations using bit operations 
- [X] debugging tools (Wireframe boxes and x,y,z axis crosshair) (**COMPLETED**: May 30, 2023) 
  - [X] wireframe boxes 
  - [X] x,y,z axis crosshair 
- [X] Voxel outline when looking at a voxel (**COMPLETED**: May 29, 2023)
- [X] 3rd Person Camera (**COMPLETED**: May 31, 2023) 
- [X] Add collision (AAAB) 
  - [X] Add Gravity and Surface collisions (**COMPLETED**: June 1, 2023)  
  - [X] AABB vs ABBB (**COMPLETED**: June 1, 2023)  
  - [ ] Swept AABB possibly? (Need to do this because original AABB vs AABB isn't good)   
  - [X] One block height autojump (use a normalized raycast and check if block exists?) 
- [X] Chunk Loading problem where multiple triangles are drawn per voxel each time there is a mesh, memset the voxels array to 0 ? (this might cause problems with the memory) (**COMPLETED**: June 6, 2023, problem was that i wasn't clearing the indices vector) 
- [X] Fix chunk borders once and for all. Have each chunk have a pointer to its neighbors. This will fix ambient occlusion problems and other annoyances
- [X] Ambient Occlusion
- [ ] Multithreading
  - [ ]  meshing 
  - [ ] generating chunks 
- [ ] Fix chunk loading performance issues  
- [ ] anisotropic filtering fix 
- [ ] Make model mesh into a 3d array of blocks   
- [ ] Model hiearchies, (Player's arms are seperate model but are still attached to the main player mody)
- [ ] Model Animations  
- [ ] Placing blocks 
- [ ] Day Night Cycle 
- [ ] Shadow Mapping 
- [ ] Water, liquids 
- [ ] Memory management (Run-length encoding (RLE) ?) 
- [ ] Entities (npcs) 
- [ ] Pathfinding for entities and basic AI 
- [ ] Make gradient across voxel surface like Cube World
- [ ] Save data 

## More Game Specific 
- [ ] Better terrain generation 
  - [ ] 3D noise 
  - [ ] Lakes 
  - [ ] Caves
  - [ ] Biomes
  - [ ] Trees
  - [ ] Roads 
- [ ] Multiple realm system (player can teleport between realms)
- [ ] Player inventory
- [ ] Combat physics   
   





