# RVox Engine 
<p align="center">
<img src="./res/screenshots/skeleton_logo.png" alt="skeleton model" width="200">  
</p>
<p align="center">A Voxel Engine made from scratch using OpenGL<p align="center">


## Procedural Infinite Generation
- ![1000 block render distance](res/screenshots/proceduralInfinite.png) 

## Optimizations   
- World is made up of chunks 
- Each chunk is meshed before hand 
- Interior voxels are culled for performance 
- Chunks are culled when out of frustum 
  
## Lighting 
- Basic rudimentary lighting which is calculated using the normals of each voxel  
- Fog 
- Shadow Mapping using depth buffer pass 
- Voxel Ambient Occlusion 

## Physics 
- AABB vs AABB collisions  
- One block height auto jump 

## Other 
- First person camera 
- 3rd person camera 
- Orthographic camera 

## Model Loading 
- ![teapot ply model](res/screenshots/teapotModel.png)   
- ![Human And Skeleton](res/screenshots/human%26skeleton.png) 


## Libraries Used 
- <a src="https://github.com/Auburn/FastNoiseLite">FastNoiseLite</a> - Procedural terrain  generation 
- <a src="https://github.com/nothings/stb/blob/master/stb_image.h">stb_image</a> - Image loading for textures  
- <a src="https://github.com/nmwsharp/happly">happly</a> - Parsing .ply files for model loading 
- <a src="https://glad.dav1d.de/">GLAD</a> - OpenGL  functionality  
- <a src="https://github.com/g-truc/glm">GLM</a> - Math library for OpenGL use 
- <a src="https://www.glfw.org/">GLFW</a> - Window management