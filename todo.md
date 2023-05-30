# ToDo 
- [X] Need to meshing problem where deletion of chunks are not accepted by chunk neighbors (**COMPLETED**: May 22, 2023)
- [X] Left chunk border problem when meshing (voxel color tied with it ?) (**COMPLETED**: May 22, 2023)
- [X] Face culling and fixing the ebo indices setup (**COMPLETED**: May 22, 2023)
- [ ] Chunk Loading problem where multiple triangles are drawn per voxel each time there is a mesh, memset the voxels array to 0 ? (this might cause problems with the memory) 
- [ ] Chunk deletion problem where chunk corners do not mesh properly 
- [X] Frustum culling chunks (**COMPLETED**: May 22, 2023)
- [ ] Refactoring 
  - [X] main
  - [ ] camera and orthocamera 
  - [ ] chunkmanager and chunk
  - [X] world (deleted) 
  - [X] vao, vbo ebo
  - [ ] renderer and chunk draw functions 
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
- [ ] 3rd Person Camera 
- [ ] Add collision (AAAB)
  - [ ] Add Gravity and Surface collisions 
- [ ] Model Animations  
- [ ] Placing blocks 
- [ ] Day Night Cycle 
- [ ] Shadow Mapping 
- [ ] Ambient Occlusion
- [ ] Water 
- [ ] Memory management (Run-length encoding (RLE) ?) 
- [ ] Multithreading
- [ ] Make gradient across voxel surface like Cube World
- [ ] Better terrain generation 
  - [ ] Caves
  - [ ] Biomes
  - [ ] Trees
  - [ ] Roads 
   





