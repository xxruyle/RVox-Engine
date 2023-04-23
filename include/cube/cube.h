#pragma once 
#include <iostream> 
#include <vector> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <noise/FastNoiseLite.h>


class Cube
{
    public: 
        std::vector<glm::vec3> positions = {glm::vec3(0.0f, 0.0f, 0.0f)};  
        void generatePlane(int randSeed); 
        void generateSingle(); 
        void getNoise(); 

}; 