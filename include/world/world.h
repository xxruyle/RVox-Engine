#pragma once 
#include <iostream> 
#include <set>
#include <vector> 
#include <tuple>
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <noise/FastNoiseLite.h>


class World
{
    public: 
        std::vector<glm::vec3> positions;  

        void generateLand(int randSeed, bool printHeights); 
        void generateSingle(); 
        void generatePlane(); 
        void printCubes(); 

}; 