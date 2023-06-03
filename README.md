# RVox Engine 
A Voxel Engine made from scratch using OpenGL

![Ridged Fractal Noise in Action](res/screenshots/proceduralInfinite.png)  


## Procedural Infinite Generation
![1000 block render distance](res/screenshots/proceduralGeneration.JPG) 

## Optimizations   
- World is made up of chunks 
- Each chunk is meshed before hand 
- Interior voxels are culled for performance 
- Chunks are culled when out of frustum 
  
## Lighting 
- Basic rudimentary lighting which is calculated using the normals of each voxel  
- Fog 

## Physics 
- AABB vs AABB collisions  
- One block height auto jump 

## Other 
- First person camera 
- 3rd person camera 
- Orthographic camera 

## Model Loading 
![teapot ply model](res/screenshots/teapotModel.png)   
![Human And Skeleton](res/screenshots/human%26skeleton.png) 
